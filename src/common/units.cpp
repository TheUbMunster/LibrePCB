/*
 * EDA4U - Professional EDA for everyone!
 * Copyright (C) 2013 Urban Bruhin
 * http://eda4u.ubruhin.ch/
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

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/

#include <QtCore>
#include <limits>
#include "units.h"
#include "exceptions.h"

/*****************************************************************************************
 *  Class LengthUnit
 ****************************************************************************************/

// Static Variables

LengthUnit::LengthUnit_t LengthUnit::sDefaultUnit = LengthUnit::LengthUnit_millimeters;

// General Methods

QString LengthUnit::toString() const noexcept
{
    switch (mUnit)
    {
        case LengthUnit_millimeters:
            return QString("millimeters");
        case LengthUnit_micrometers:
            return QString("micrometers");
        case LengthUnit_nanometers:
            return QString("nanometers");
        case LengthUnit_inches:
            return QString("inches");
        case LengthUnit_mils:
            return QString("mils");
        default:
            qCritical() << "invalid length unit:" << mUnit;
            Q_ASSERT(false);
            return QString();
    }
}

QString LengthUnit::toStringTr() const noexcept
{
    switch (mUnit)
    {
        case LengthUnit_millimeters:
            return QCoreApplication::translate("LengthUnit", "Millimeters");
        case LengthUnit_micrometers:
            return QCoreApplication::translate("LengthUnit", "Micrometers");
        case LengthUnit_nanometers:
            return QCoreApplication::translate("LengthUnit", "Nanometers");
        case LengthUnit_inches:
            return QCoreApplication::translate("LengthUnit", "Inches");
        case LengthUnit_mils:
            return QCoreApplication::translate("LengthUnit", "Mils");
        default:
            qCritical() << "invalid length unit:" << mUnit;
            Q_ASSERT(false);
            return QString();
    }
}

QString LengthUnit::toShortStringTr() const noexcept
{
    switch (mUnit)
    {
        case LengthUnit_millimeters:
            return QCoreApplication::translate("LengthUnit", "mm");
        case LengthUnit_micrometers:
            return QCoreApplication::translate("LengthUnit", "μm");
        case LengthUnit_nanometers:
            return QCoreApplication::translate("LengthUnit", "nm");
        case LengthUnit_inches:
            return QCoreApplication::translate("LengthUnit", "″");
        case LengthUnit_mils:
            return QCoreApplication::translate("LengthUnit", "mils");
        default:
            qCritical() << "invalid length unit:" << mUnit;
            Q_ASSERT(false);
            return QString();
    }
}

qreal LengthUnit::convertToUnit(const Length& length) const noexcept
{
    switch (mUnit)
    {
        case LengthUnit_millimeters:
            return length.toMm();
        case LengthUnit_micrometers:
            return length.toMm() * (qreal)1000;
        case LengthUnit_nanometers:
            return (qreal)length.toNm();
        case LengthUnit_inches:
            return length.toInch();
        case LengthUnit_mils:
            return length.toMil();
        default:
            qCritical() << "invalid length unit:" << mUnit;
            Q_ASSERT(false);
            return 0;
    }
}

QPointF LengthUnit::convertToUnit(const Point& point) const noexcept
{
    switch (mUnit)
    {
        case LengthUnit_millimeters:
            return point.toMmQPointF();
        case LengthUnit_micrometers:
            return point.toMmQPointF() * (qreal)1000;
        case LengthUnit_nanometers:
            return point.toMmQPointF() * (qreal)1000000;
        case LengthUnit_inches:
            return point.toInchQPointF();
        case LengthUnit_mils:
            return point.toMilQPointF();
        default:
            qCritical() << "invalid length unit:" << mUnit;
            Q_ASSERT(false);
            return QPointF();
    }
}

Length LengthUnit::convertFromUnit(qreal length) const noexcept
{
    switch (mUnit)
    {
        case LengthUnit_millimeters:
            return Length::fromMm(length);
        case LengthUnit_micrometers:
            return Length::fromMm(length / (qreal)1000);
        case LengthUnit_nanometers:
            return Length::fromMm(length / (qreal)1000000);
        case LengthUnit_inches:
            return Length::fromInch(length);
        case LengthUnit_mils:
            return Length::fromMil(length);
        default:
            qCritical() << "invalid length unit:" << mUnit;
            Q_ASSERT(false);
            return Length(0);
    }
}

Point LengthUnit::convertFromUnit(const QPointF& point) const noexcept
{
    switch (mUnit)
    {
        case LengthUnit_millimeters:
            return Point::fromMm(point);
        case LengthUnit_micrometers:
            return Point::fromMm(point / (qreal)1000);
        case LengthUnit_nanometers:
            return Point::fromMm(point / (qreal)1000000);
        case LengthUnit_inches:
            return Point::fromInch(point);
        case LengthUnit_mils:
            return Point::fromMil(point);
        default:
            qCritical() << "invalid length unit:" << mUnit;
            Q_ASSERT(false);
            return Point(Length(0), Length(0));
    }
}

// Static Methods

LengthUnit LengthUnit::fromIndex(int index, const LengthUnit& defaultUnit, bool* ok) noexcept
{
    if ((index >= 0) && (index < LengthUnit_COUNT))
    {
        if (ok) *ok = true;
        return LengthUnit(static_cast<LengthUnit_t>(index));
    }
    else
    {
        if (ok) *ok = false;
        return defaultUnit;
    }
}

LengthUnit LengthUnit::fromString(const QString& unitString,
                                  const LengthUnit& defaultUnit, bool* ok) noexcept
{
    if (ok) *ok = true;

    if (unitString == "millimeters")
        return LengthUnit(LengthUnit_millimeters);
    else if (unitString == "micrometers")
        return LengthUnit(LengthUnit_micrometers);
    else if (unitString == "nanometers")
        return LengthUnit(LengthUnit_nanometers);
    else if (unitString == "inches")
        return LengthUnit(LengthUnit_inches);
    else if (unitString == "mils")
        return LengthUnit(LengthUnit_mils);
    else
    {
        if (ok) *ok = false;
        return defaultUnit;
    }
}

QList<LengthUnit> LengthUnit::getAllUnits() noexcept
{
    QList<LengthUnit> list;
    for (int i = 0; i < LengthUnit_COUNT; i++)
        list.append(LengthUnit(static_cast<LengthUnit_t>(i)));
    return list;
}

// Non-Member Functions

QDataStream& operator<<(QDataStream& stream, const LengthUnit& unit)
{
    stream << unit.toString();
    return stream;
}

QDebug operator<<(QDebug stream, const LengthUnit& unit)
{
    stream << QString("LengthUnit(%1)").arg(unit.toString());
    return stream;
}

/*****************************************************************************************
 *  Class Length
 ****************************************************************************************/

// General Methods

Length Length::mappedToGrid(const Length& gridInterval) const
{
    Length length(*this);
    return length.mapToGrid(gridInterval);
}

Length& Length::mapToGrid(const Length& gridInterval)
{
    mNanometers = mapNmToGrid(mNanometers, gridInterval);
    return *this;
}

// Static Methods

Length Length::fromMm(qreal millimeters, const Length& gridInterval)
{
    Length l;
    l.setLengthMm(millimeters);
    return l.mapToGrid(gridInterval);
}

Length Length::fromMm(const QString& millimeters, const Length& gridInterval)
{
    Length l;
    l.setLengthMm(millimeters);
    return l.mapToGrid(gridInterval);
}

Length Length::fromInch(qreal inches, const Length& gridInterval)
{
    Length l;
    l.setLengthInch(inches);
    return l.mapToGrid(gridInterval);
}

Length Length::fromMil(qreal mils, const Length& gridInterval)
{
    Length l;
    l.setLengthMil(mils);
    return l.mapToGrid(gridInterval);
}

Length Length::fromPx(qreal pixels, const Length& gridInterval)
{
    Length l;
    l.setLengthPx(pixels);
    return l.mapToGrid(gridInterval);
}

// Private Methods

/**
 * @brief Set the length from a floating point number in nanometers
 *
 * This is a helper method for the setLength*() methods.
 *
 * @param nanometers    A floating point number in nanometers.
 *
 * @note The parameter is NOT an integer although we don't use numbers smaller than
 * one nanometer. This way, the range of this parameter is much greater and we can
 * compare the value with the range of an integer. If the value is outside the range
 * of an integer, we will throw an exception. If we would pass the length as an integer,
 * we couldn't detect such under-/overflows!
 */
void Length::setLengthFromFloat(qreal nanometers)
{
    LengthBase_t min = std::numeric_limits<LengthBase_t>::min();
    LengthBase_t max = std::numeric_limits<LengthBase_t>::max();
    if ((nanometers > max) || (nanometers < min))
    {
        throw RangeError(__FILE__, __LINE__, QString("value=%1; min=%2; max=%3")
                         .arg(nanometers).arg(min).arg(max),
                         QCoreApplication::translate("Length", "Range error!"));
    }

    mNanometers = qRound(nanometers);
}

// Private Static Methods

/**
 * @brief Map a length in nanometers to a grid interval in nanometers
 *
 * This is a helper function for mapToGrid().
 *
 * @param nanometers    The length we want to map to the grid
 * @param gridInterval  The grid interval
 *
 * @return  The length which is mapped to the grid (always a multiple of gridInterval)
 *
 * @todo    does this work correctly with large 64bit integers?!
 *          and maybe there is a better, integer-based method for this purpose?
 */
LengthBase_t Length::mapNmToGrid(LengthBase_t nanometers, const Length& gridInterval)
{
    if (gridInterval.mNanometers != 0)
        return qRound((qreal)nanometers / gridInterval.mNanometers) * gridInterval.mNanometers;
    else
        return nanometers;
}

/**
 * @brief Convert a length from a QString (in millimeters) to an integer (in nanometers)
 *
 * This is a helper function for Length(const QString&) and setLengthMm().
 *
 * @param millimeters   A QString which contains a floating point number with maximum
 *                      six decimals after the decimal point. The locale of the string
 *                      have to be "C"! Example: QString("-1234.56") for -1234.56mm
 *
 * @return The length in nanometers
 *
 * @todo    don't use double for this purpose!
 *          and throw an exception if a range error occurs (under-/overflow)!
 */
LengthBase_t Length::mmStringToNm(const QString& millimeters)
{
    return qRound(millimeters.toDouble() * 1e6);
}

// Non-Member Functions

QDataStream& operator<<(QDataStream& stream, const Length& length)
{
    stream << length.toMm();
    return stream;
}

QDebug operator<<(QDebug stream, const Length& length)
{
    stream << QString("Length(%1mm)").arg(length.toMm());
    return stream;
}

/*****************************************************************************************
 *  Class Angle
 ****************************************************************************************/

// Static Methods

Angle Angle::fromDeg(qreal degrees)
{
    Angle angle;
    angle.setAngleDeg(degrees);
    return angle;
}

Angle Angle::fromDeg(const QString& degrees)
{
    Angle angle;
    angle.setAngleDeg(degrees);
    return angle;
}

Angle Angle::fromRad(qreal radians)
{
    Angle angle;
    angle.setAngleRad(radians);
    return angle;
}

// Private Static Methods

/**
 * @brief Convert an angle from a QString (in degrees) to an integer (in microdegrees)
 *
 * This is a helper function for Angle(const QString&) and setAngleDeg().
 *
 * @param degrees   A QString which contains a floating point number with maximum
 *                  six decimals after the decimal point. The locale of the string
 *                  have to be "C"! Example: QString("-123.456") for -123.456 degrees
 *
 * @return The angle in microdegrees
 *
 * @todo    don't use double for this purpose!
 *          and map the angle to +/- 360 degrees BEFORE converting it to microdegrees!
 *          throw an exception on range errors!
 */
qint32 Angle::degStringToMicrodeg(const QString& degrees)
{
    return qRound(degrees.toDouble() * 1e6);
}

// Non-Member Functions

QDataStream& operator<<(QDataStream& stream, const Angle& angle)
{
    stream << angle.toDeg();
    return stream;
}

QDebug operator<<(QDebug stream, const Angle& angle)
{
    stream << QString("Angle(%1°)").arg(angle.toDeg());
    return stream;
}

/*****************************************************************************************
 *  Class Point
 ****************************************************************************************/

// General Methods

Point Point::mappedToGrid(const Length& gridInterval) const
{
    Point p(*this);
    p.mapToGrid(gridInterval);
    return p;
}

Point& Point::mapToGrid(const Length& gridInterval)
{
    mX.mapToGrid(gridInterval);
    mY.mapToGrid(gridInterval);
    return *this;
}

// Static Methods

Point Point::fromMm(qreal millimetersX, qreal millimetersY, const Length& gridInterval)
{
    Point p;
    p.mX.setLengthMm(millimetersX);
    p.mY.setLengthMm(millimetersY);
    return p.mapToGrid(gridInterval);
}

Point Point::fromMm(const QPointF& millimeters, const Length& gridInterval)
{
    return fromMm(millimeters.x(), millimeters.y(), gridInterval);
}

Point Point::fromInch(qreal inchesX, qreal inchesY, const Length& gridInterval)
{
    Point p;
    p.mX.setLengthInch(inchesX);
    p.mY.setLengthInch(inchesY);
    return p.mapToGrid(gridInterval);
}

Point Point::fromInch(const QPointF& inches, const Length& gridInterval)
{
    return fromInch(inches.x(), inches.y(), gridInterval);
}

Point Point::fromMil(qreal milsX, qreal milsY, const Length& gridInterval)
{
    Point p;
    p.mX.setLengthMil(milsX);
    p.mY.setLengthMil(milsY);
    return p.mapToGrid(gridInterval);
}

Point Point::fromMil(const QPointF& mils, const Length& gridInterval)
{
    return fromMil(mils.x(), mils.y(), gridInterval);
}

Point Point::fromPx(qreal pixelsX, qreal pixelsY, const Length& gridInterval)
{
    Point p;
    p.mX.setLengthPx(pixelsX);
    p.mY.setLengthPx(-pixelsY); // invert Y!
    return p.mapToGrid(gridInterval);
}

Point Point::fromPx(const QPointF& pixels, const Length& gridInterval)
{
    return fromPx(pixels.x(), pixels.y(), gridInterval);
}

// Non-Member Functions

QDataStream& operator<<(QDataStream& stream, const Point& point)
{
    stream << point.toMmQPointF();
    return stream;
}

QDebug operator<<(QDebug stream, const Point& point)
{
    stream << QString("Point(%1mm, %2mm)").arg(point.toMmQPointF().x()).arg(point.toMmQPointF().y());
    return stream;
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/
