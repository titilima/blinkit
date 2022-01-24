// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: DoubleRect.cpp
// Description: DoubleRect Class
//      Author: Ziming Li
//     Created: 2022-01-24
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "./DoubleRect.h"

#include "blinkit/blink/renderer/platform/geometry/FloatRect.h"
#include "blinkit/blink/renderer/platform/geometry/LayoutRect.h"

namespace blink {

DoubleRect::DoubleRect(const IntRect& r) : m_location(r.location()), m_size(r.size())
{
}

DoubleRect::DoubleRect(const FloatRect& r) : m_location(r.location()), m_size(r.size())
{
}

DoubleRect::DoubleRect(const LayoutRect& r) : m_location(r.location()), m_size(r.size())
{
}

IntRect enclosingIntRect(const DoubleRect& rect)
{
    IntPoint location = flooredIntPoint(rect.minXMinYCorner());
    IntPoint maxPoint = ceiledIntPoint(rect.maxXMaxYCorner());

    return IntRect(location, maxPoint - location);
}

IntRect enclosedIntRect(const DoubleRect& rect)
{
    IntPoint location = ceiledIntPoint(rect.minXMinYCorner());
    IntPoint maxPoint = flooredIntPoint(rect.maxXMaxYCorner());
    IntSize size = maxPoint - location;
    size.clampNegativeToZero();

    return IntRect(location, size);
}

IntRect roundedIntRect(const DoubleRect& rect)
{
    return IntRect(roundedIntPoint(rect.location()), roundedIntSize(rect.size()));
}

void DoubleRect::scale(float sx, float sy)
{
    m_location.setX(x() * sx);
    m_location.setY(y() * sy);
    m_size.setWidth(width() * sx);
    m_size.setHeight(height() * sy);
}


} // namespace blink
