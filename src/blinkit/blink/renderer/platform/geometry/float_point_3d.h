#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: float_point_3d.h
// Description: FloatPoint3D Class
//      Author: Ziming Li
//     Created: 2021-12-05
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
    Copyright (C) 2004, 2005, 2006 Nikolas Zimmermann <wildfox@kde.org>
                  2004, 2005 Rob Buis <buis@kde.org>
                  2005 Eric Seidel <eric@webkit.org>
                  2010 Zoltan Herczeg <zherczeg@webkit.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    aint with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef FloatPoint3D_h
#define FloatPoint3D_h

#include "blinkit/blink/renderer/platform/geometry/FloatPoint.h"
#include "third_party/skia/include/core/SkPoint3.h"

namespace blink {

class FloatPoint3D
{
    DISALLOW_NEW();
public:
    FloatPoint3D()
        : m_x(0)
        , m_y(0)
        , m_z(0)
    {
    }

    FloatPoint3D(float x, float y, float z)
        : m_x(x)
        , m_y(y)
        , m_z(z)
    {
    }

    FloatPoint3D(const FloatPoint& p)
        : m_x(p.x())
        , m_y(p.y())
        , m_z(0)
    {
    }

    FloatPoint3D(const FloatPoint3D& p)
        : m_x(p.x())
        , m_y(p.y())
        , m_z(p.z())
    {
    }

    float x() const { return m_x; }
    void setX(float x) { m_x = x; }

    float y() const { return m_y; }
    void setY(float y) { m_y = y; }

    float z() const { return m_z; }
    void setZ(float z) { m_z = z; }
    void set(float x, float y, float z)
    {
        m_x = x;
        m_y = y;
        m_z = z;
    }
    void move(float dx, float dy, float dz)
    {
        m_x += dx;
        m_y += dy;
        m_z += dz;
    }
    void scale(float sx, float sy, float sz)
    {
        m_x *= sx;
        m_y *= sy;
        m_z *= sz;
    }

    bool isZero() const
    {
        return !m_x && !m_y && !m_z;
    }

    void normalize();

    float dot(const FloatPoint3D& a) const
    {
        return m_x * a.x() + m_y * a.y() + m_z * a.z();
    }

    // Compute the angle (in radians) between this and y.  If either vector is the zero vector,
    // return an angle of 0.
    float angleBetween(const FloatPoint3D& y) const;

    // Sets this FloatPoint3D to the cross product of the passed two.
    // It is safe for "this" to be the same as either or both of the
    // arguments.
    void cross(const FloatPoint3D& a, const FloatPoint3D& b)
    {
        float x = a.y() * b.z() - a.z() * b.y();
        float y = a.z() * b.x() - a.x() * b.z();
        float z = a.x() * b.y() - a.y() * b.x();
        m_x = x;
        m_y = y;
        m_z = z;
    }

    // Convenience function returning "this cross point" as a
    // stack-allocated result.
    FloatPoint3D cross(const FloatPoint3D& point) const
    {
        FloatPoint3D result;
        result.cross(*this, point);
        return result;
    }

    float lengthSquared() const { return this->dot(*this); }
    float length() const { return sqrtf(lengthSquared()); }

    float distanceTo(const FloatPoint3D& a) const;

    operator SkPoint3() const { return SkPoint3::Make(m_x, m_y, m_z); }

private:
    float m_x;
    float m_y;
    float m_z;
};

inline FloatPoint3D& operator +=(FloatPoint3D& a, const FloatPoint3D& b)
{
    a.move(b.x(), b.y(), b.z());
    return a;
}

inline FloatPoint3D& operator -=(FloatPoint3D& a, const FloatPoint3D& b)
{
    a.move(-b.x(), -b.y(), -b.z());
    return a;
}

inline FloatPoint3D operator+(const FloatPoint3D& a, const FloatPoint3D& b)
{
    return FloatPoint3D(a.x() + b.x(), a.y() + b.y(), a.z() + b.z());
}

inline FloatPoint3D operator-(const FloatPoint3D& a, const FloatPoint3D& b)
{
    return FloatPoint3D(a.x() - b.x(), a.y() - b.y(), a.z() - b.z());
}

inline bool operator==(const FloatPoint3D& a, const FloatPoint3D& b)
{
    return zed::almost_equals(a.x(), b.x()) && zed::almost_equals(a.y(), b.y()) && zed::almost_equals(a.z(), b.z());
}

inline bool operator!=(const FloatPoint3D& a, const FloatPoint3D& b)
{
    return !zed::almost_equals(a.x(), b.x()) || !zed::almost_equals(a.y(), b.y()) || !zed::almost_equals(a.z(), b.z());
}

inline float operator*(const FloatPoint3D& a, const FloatPoint3D& b)
{
    // dot product
    return a.dot(b);
}

inline FloatPoint3D operator*(float k, const FloatPoint3D& v)
{
    return FloatPoint3D(k * v.x(), k * v.y(), k * v.z());
}

inline FloatPoint3D operator*(const FloatPoint3D& v, float k)
{
    return FloatPoint3D(k * v.x(), k * v.y(), k * v.z());
}

inline float FloatPoint3D::distanceTo(const FloatPoint3D& a) const
{
    return (*this - a).length();
}

// Redeclared here to avoid ODR issues.
// See platform/testing/GeometryPrinters.h.
void PrintTo(const FloatPoint3D&, std::ostream*);

} // namespace blink

#ifndef NDEBUG
namespace zed {
template <>
void log_serializer::push<blink::FloatPoint3D>(std::vector<std::string> &dst, const blink::FloatPoint3D &point);
}
#endif

#endif // FloatPoint3D_h
