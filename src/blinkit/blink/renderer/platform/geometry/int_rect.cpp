// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: int_rect.cpp
// Description: IntRect Class
//      Author: Ziming Li
//     Created: 2022-01-09
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2003, 2006, 2009 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "./int_rect.h"

#include <algorithm>
#include "blinkit/blink/renderer/platform/geometry/FloatRect.h"
#include "blinkit/blink/renderer/platform/geometry/LayoutRect.h"
#include "third_party/skia/include/core/SkRect.h"

#ifndef NDEBUG
namespace zed {

template <>
void log_serializer::push<blink::IntPoint>(std::vector<std::string> &dst, const blink::IntPoint &point)
{
    char buf[64];
    sprintf(buf, "[IntPoint (%d, %d)]", point.x(), point.y());
    dst.emplace_back(buf);
}

template <>
void log_serializer::push<blink::IntRect>(std::vector<std::string> &dst, const blink::IntRect &rect)
{
    char buf[64];
    sprintf(buf, "[IntRect (%d, %d) of %dx%d]", rect.x(), rect.y(), rect.width(), rect.height());
    dst.emplace_back(buf);
}

template <>
void log_serializer::push<blink::IntSize>(std::vector<std::string> &dst, const blink::IntSize &size)
{
    char buf[64];
    sprintf(buf, "[IntSize %dx%d]", size.width(), size.height());
    dst.emplace_back(buf);
}

}
#endif // NDEBUG

namespace blink {

IntRect::IntRect(const FloatRect& r)
    : m_location(clampTo<int>(r.x()), clampTo<int>(r.y()))
    , m_size(clampTo<int>(r.width()), clampTo<int>(r.height()))
{
}

IntRect::IntRect(const LayoutRect& r)
    : m_location(r.x(), r.y())
    , m_size(r.width(), r.height())
{
}

bool IntRect::intersects(const IntRect& other) const
{
    // Checking emptiness handles negative widths as well as zero.
    return !isEmpty() && !other.isEmpty()
        && x() < other.maxX() && other.x() < maxX()
        && y() < other.maxY() && other.y() < maxY();
}

bool IntRect::contains(const IntRect& other) const
{
    return x() <= other.x() && maxX() >= other.maxX()
        && y() <= other.y() && maxY() >= other.maxY();
}

void IntRect::intersect(const IntRect& other)
{
    int left = std::max(x(), other.x());
    int top = std::max(y(), other.y());
    int right = std::min(maxX(), other.maxX());
    int bottom = std::min(maxY(), other.maxY());

    // Return a clean empty rectangle for non-intersecting cases.
    if (left >= right || top >= bottom) {
        left = 0;
        top = 0;
        right = 0;
        bottom = 0;
    }

    m_location.setX(left);
    m_location.setY(top);
    m_size.setWidth(right - left);
    m_size.setHeight(bottom - top);
}

void IntRect::unite(const IntRect& other)
{
    // Handle empty special cases first.
    if (other.isEmpty())
        return;
    if (isEmpty()) {
        *this = other;
        return;
    }

    uniteEvenIfEmpty(other);
}

void IntRect::uniteIfNonZero(const IntRect& other)
{
    // Handle empty special cases first.
    if (!other.width() && !other.height())
        return;
    if (!width() && !height()) {
        *this = other;
        return;
    }

    uniteEvenIfEmpty(other);
}

void IntRect::uniteEvenIfEmpty(const IntRect& other)
{
    int left = std::min(x(), other.x());
    int top = std::min(y(), other.y());
    int right = std::max(maxX(), other.maxX());
    int bottom = std::max(maxY(), other.maxY());

    m_location.setX(left);
    m_location.setY(top);
    m_size.setWidth(right - left);
    m_size.setHeight(bottom - top);
}

void IntRect::scale(float s)
{
    m_location.setX((int)(x() * s));
    m_location.setY((int)(y() * s));
    m_size.setWidth((int)(width() * s));
    m_size.setHeight((int)(height() * s));
}

static inline int distanceToInterval(int pos, int start, int end)
{
    if (pos < start)
        return start - pos;
    if (pos > end)
        return end - pos;
    return 0;
}

IntSize IntRect::differenceToPoint(const IntPoint& point) const
{
    int xdistance = distanceToInterval(point.x(), x(), maxX());
    int ydistance = distanceToInterval(point.y(), y(), maxY());
    return IntSize(xdistance, ydistance);
}

IntRect::operator SkIRect() const
{
    SkIRect rect = { x(), y(), maxX(), maxY() };
    return rect;
}

IntRect::operator SkRect() const
{
    SkRect rect;
    rect.set(SkIntToScalar(x()), SkIntToScalar(y()), SkIntToScalar(maxX()), SkIntToScalar(maxY()));
    return rect;
}

IntRect unionRect(const Vector<IntRect>& rects)
{
    IntRect result;

    size_t count = rects.size();
    for (size_t i = 0; i < count; ++i)
        result.unite(rects[i]);

    return result;
}

IntRect unionRectEvenIfEmpty(const Vector<IntRect>& rects)
{
    size_t count = rects.size();
    if (!count)
        return IntRect();

    IntRect result = rects[0];
    for (size_t i = 1; i < count; ++i)
        result.uniteEvenIfEmpty(rects[i]);

    return result;
}

#ifndef NDEBUG
    // Prints the rect to the screen.
void IntRect::show() const
{
    LayoutRect(*this).show();
}
#endif

} // namespace blink
