#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: DoublePoint.h
// Description: DoublePoint Class
//      Author: Ziming Li
//     Created: 2021-12-05
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DoublePoint_h
#define DoublePoint_h

#include <algorithm>
#include "blinkit/blink/renderer/platform/geometry/DoubleSize.h"
#include "blinkit/blink/renderer/platform/geometry/FloatPoint.h"

namespace blink {

class LayoutPoint;

class DoublePoint {
    DISALLOW_NEW();
public:
    DoublePoint()
        : m_x(0)
        , m_y(0)
    {
    }
    DoublePoint(double x, double y)
        : m_x(x)
        , m_y(y)
    {
    }
    DoublePoint(const IntPoint& p)
        : m_x(p.x())
        , m_y(p.y())
    {
    }
    DoublePoint(const FloatPoint& p)
        : m_x(p.x())
        , m_y(p.y())
    {
    }
    explicit DoublePoint(const LayoutPoint&);

    explicit DoublePoint(const IntSize& size)
        : m_x(size.width()), m_y(size.height())
    {
    }

    explicit DoublePoint(const FloatSize&);

    explicit DoublePoint(const DoubleSize& size)
        : m_x(size.width()), m_y(size.height())
    {
    }

    static DoublePoint zero() { return DoublePoint(); }

    DoublePoint expandedTo(const DoublePoint& other) const
    {
        return DoublePoint(std::max(m_x, other.m_x), std::max(m_y, other.m_y));
    }

    DoublePoint shrunkTo(const DoublePoint& other) const
    {
        return DoublePoint(std::min(m_x, other.m_x), std::min(m_y, other.m_y));
    }

    double x() const { return m_x; }
    double y() const { return m_y; }
    void setX(double x) { m_x = x; }
    void setY(double y) { m_y = y; }

    void move(const DoubleSize& s)
    {
        m_x += s.width();
        m_y += s.height();
    }

    void move(double x, double y)
    {
        m_x += x;
        m_y += y;
    }

    void moveBy(const DoublePoint& p)
    {
        m_x += p.x();
        m_y += p.y();
    }

    void scale(float sx, float sy)
    {
        m_x *= sx;
        m_y *= sy;
    }

    DoublePoint scaledBy(float scale) const
    {
        return DoublePoint(m_x * scale, m_y * scale);
    }

private:
    double m_x, m_y;
};

inline bool operator==(const DoublePoint& a, const DoublePoint& b)
{
    return zed::almost_equals(a.x(), b.x()) && zed::almost_equals(a.y(), b.y());
}

inline bool operator!=(const DoublePoint& a, const DoublePoint& b)
{
    return !zed::almost_equals(a.x(), b.x()) || !zed::almost_equals(a.y(), b.y());
}

inline DoublePoint& operator+=(DoublePoint& a, const DoubleSize& b)
{
    a.setX(a.x() + b.width());
    a.setY(a.y() + b.height());
    return a;
}

inline DoublePoint& operator-=(DoublePoint& a, const DoubleSize& b)
{
    a.setX(a.x() - b.width());
    a.setY(a.y() - b.height());
    return a;
}

inline DoublePoint operator+(const DoublePoint& a, const DoubleSize& b)
{
    return DoublePoint(a.x() + b.width(), a.y() + b.height());
}

inline DoubleSize operator-(const DoublePoint& a, const DoublePoint& b)
{
    return DoubleSize(a.x() - b.x(), a.y() - b.y());
}

inline DoublePoint operator-(const DoublePoint& a)
{
    return DoublePoint(-a.x(), -a.y());
}

inline DoublePoint operator-(const DoublePoint& a, const DoubleSize& b)
{
    return DoublePoint(a.x() - b.width(), a.y() - b.height());
}

inline IntPoint roundedIntPoint(const DoublePoint& p)
{
    return IntPoint(clampTo<int>(roundf(p.x())), clampTo<int>(roundf(p.y())));
}

inline IntPoint ceiledIntPoint(const DoublePoint& p)
{
    return IntPoint(clampTo<int>(ceil(p.x())), clampTo<int>(ceil(p.y())));
}

inline IntPoint flooredIntPoint(const DoublePoint& p)
{
    return IntPoint(clampTo<int>(floor(p.x())), clampTo<int>(floor(p.y())));
}

inline FloatPoint toFloatPoint(const DoublePoint& a)
{
    return FloatPoint(a.x(), a.y());
}

inline DoubleSize toDoubleSize(const DoublePoint& a)
{
    return DoubleSize(a.x(), a.y());
}

}

#endif
