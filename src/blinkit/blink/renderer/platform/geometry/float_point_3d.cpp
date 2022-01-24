// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: float_point_3d.cpp
// Description: FloatPoint3D Class
//      Author: Ziming Li
//     Created: 2022-01-24
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

/*
    Copyright (C) 2004, 2005, 2006 Nikolas Zimmermann <wildfox@kde.org>
                  2004, 2005 Rob Buis <buis@kde.org>
                  2005 Eric Seidel <eric@webkit.org>

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

#include "./float_point_3d.h"

#include <cmath>

#ifndef NDEBUG
namespace zed {
template <>
void log_serializer::push<blink::FloatPoint3D>(std::vector<std::string> &dst, const blink::FloatPoint3D &point)
{
    char buf[128];
    sprintf(buf, "[FloatPoint3D (%.1f, %.1f, %.1f)]", point.x(), point.y(), point.z());
    dst.emplace_back(buf);
}
}
#endif // NDEBUG

namespace blink {

void FloatPoint3D::normalize()
{
    float tempLength = length();

    if (tempLength) {
        m_x /= tempLength;
        m_y /= tempLength;
        m_z /= tempLength;
    }
}

float FloatPoint3D::angleBetween(const FloatPoint3D& y) const
{
    float xLength = this->length();
    float yLength = y.length();

    if (xLength && yLength) {
        float cosAngle = this->dot(y) / (xLength * yLength);
        // Due to round-off |cosAngle| can have a magnitude greater than 1.  Clamp the value to [-1,
        // 1] before computing the angle.
        return acos(clampTo(cosAngle, -1.0, 1.0));
    }
    return 0;
}

} // namespace blink

