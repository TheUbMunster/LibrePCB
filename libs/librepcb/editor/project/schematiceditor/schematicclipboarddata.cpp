/*
 * LibrePCB - Professional EDA for everyone!
 * Copyright (C) 2013 LibrePCB Developers, see AUTHORS.md for contributors.
 * https://librepcb.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*******************************************************************************
 *  Includes
 ******************************************************************************/
#include "schematicclipboarddata.h"

#include <librepcb/core/fileio/transactionaldirectory.h>
#include <librepcb/core/fileio/transactionalfilesystem.h>
#include <librepcb/core/library/librarybaseelement.h>

#include <QtCore>
#include <QtWidgets>

/*******************************************************************************
 *  Namespace
 ******************************************************************************/
namespace librepcb {
namespace editor {

/*******************************************************************************
 *  Constructors / Destructor
 ******************************************************************************/

SchematicClipboardData::SchematicClipboardData(const Uuid& schematicUuid,
                                               const Point& cursorPos) noexcept
  : mFileSystem(TransactionalFileSystem::openRW(FilePath::getRandomTempPath())),
    mSchematicUuid(schematicUuid),
    mCursorPos(cursorPos),
    mComponentInstances(),
    mSymbolInstances(),
    mNetSegments(),
    mPolygons(),
    mTexts() {
}

SchematicClipboardData::SchematicClipboardData(const QByteArray& mimeData)
  : SchematicClipboardData(Uuid::createRandom(), Point()) {
  mFileSystem->loadFromZip(mimeData);  // can throw

  SExpression root =
      SExpression::parse(mFileSystem->read("schematic.lp"), FilePath());
  mSchematicUuid = deserialize<Uuid>(root.getChild("schematic/@0"));
  mCursorPos = Point(root.getChild("cursor_position"));
  mComponentInstances.loadFromSExpression(root);
  mSymbolInstances.loadFromSExpression(root);
  mNetSegments.loadFromSExpression(root);
  mPolygons.loadFromSExpression(root);
  mTexts.loadFromSExpression(root);
}

SchematicClipboardData::~SchematicClipboardData() noexcept {
  // Clean up the temporary directory, but destroy the TransactionalFileSystem
  // object first since it has a lock on the directory.
  FilePath fp = mFileSystem->getAbsPath();
  mFileSystem.reset();
  QDir(fp.toStr()).removeRecursively();
}

/*******************************************************************************
 *  Getters
 ******************************************************************************/

std::unique_ptr<TransactionalDirectory> SchematicClipboardData::getDirectory(
    const QString& path) noexcept {
  return std::unique_ptr<TransactionalDirectory>(
      new TransactionalDirectory(mFileSystem, path));
}

/*******************************************************************************
 *  General Methods
 ******************************************************************************/

std::unique_ptr<QMimeData> SchematicClipboardData::toMimeData() const {
  SExpression root = SExpression::createList("librepcb_clipboard_schematic");
  root.ensureLineBreak();
  mCursorPos.serialize(root.appendList("cursor_position"));
  root.ensureLineBreak();
  root.appendChild("schematic", mSchematicUuid);
  root.ensureLineBreak();
  mComponentInstances.serialize(root);
  root.ensureLineBreak();
  mSymbolInstances.serialize(root);
  root.ensureLineBreak();
  mNetSegments.serialize(root);
  root.ensureLineBreak();
  mPolygons.serialize(root);
  root.ensureLineBreak();
  mTexts.serialize(root);
  root.ensureLineBreak();
  mFileSystem->write("schematic.lp", root.toByteArray());

  QByteArray zip = mFileSystem->exportToZip();

  std::unique_ptr<QMimeData> data(new QMimeData());
  data->setData(getMimeType(), zip);
  data->setData("application/zip", zip);
  return data;
}

std::unique_ptr<SchematicClipboardData> SchematicClipboardData::fromMimeData(
    const QMimeData* mime) {
  QByteArray content = mime ? mime->data(getMimeType()) : QByteArray();
  if (!content.isNull()) {
    return std::unique_ptr<SchematicClipboardData>(
        new SchematicClipboardData(content));  // can throw
  } else {
    return nullptr;
  }
}

/*******************************************************************************
 *  Private Methods
 ******************************************************************************/

QString SchematicClipboardData::getMimeType() noexcept {
  return QString("application/x-librepcb-clipboard.schematic; version=%1")
      .arg(qApp->applicationVersion());
}

/*******************************************************************************
 *  End of File
 ******************************************************************************/

}  // namespace editor
}  // namespace librepcb
