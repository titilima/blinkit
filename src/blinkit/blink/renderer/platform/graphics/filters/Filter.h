#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: Filter.h
// Description: Filter Class
//      Author: Ziming Li
//     Created: 2021-12-06
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2009 Dirk Schulze <krit@webkit.org>
 * Copyright (C) 2013 Google Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef Filter_h
#define Filter_h

#include "blinkit/blink/renderer/platform/geometry/FloatPoint3D.h"
#include "blinkit/blink/renderer/platform/geometry/FloatRect.h"
#include "blinkit/blink/renderer/platform/geometry/int_rect.h"
#include "blinkit/blink/renderer/platform/heap/Handle.h"
#include "blinkit/blink/renderer/wtf/Noncopyable.h"
#include "blinkit/blink/renderer/wtf/RefCounted.h"

namespace blink {

class SourceGraphic;
class FilterEffect;

class Filter final : public BlinKit::GCObject
{
    WTF_MAKE_NONCOPYABLE(Filter);
public:
    enum UnitScaling {
        UserSpace,
        BoundingBox
    };

    static GCRefPtr<Filter> create(const FloatRect& referenceBox, const FloatRect& filterRegion, float scale, UnitScaling);
    static GCRefPtr<Filter> create(float scale);

    ~Filter();
    DECLARE_TRACE();

    float scale() const { return m_scale; }
    FloatRect mapLocalRectToAbsoluteRect(const FloatRect&) const;
    FloatRect mapAbsoluteRectToLocalRect(const FloatRect&) const;

    float applyHorizontalScale(float value) const;
    float applyVerticalScale(float value) const;

    FloatPoint3D resolve3dPoint(const FloatPoint3D&) const;

    FloatRect absoluteFilterRegion() const { return mapLocalRectToAbsoluteRect(m_filterRegion); }

    const FloatRect& filterRegion() const { return m_filterRegion; }
    const FloatRect& referenceBox() const { return m_referenceBox; }

    void setLastEffect(const GCRefPtr<FilterEffect> &);
    FilterEffect* lastEffect(void) const;

    SourceGraphic* sourceGraphic(void) const;

private:
    Filter(const FloatRect& referenceBox, const FloatRect& filterRegion, float scale, UnitScaling);

    FloatRect m_referenceBox;
    FloatRect m_filterRegion;
    float m_scale;
    UnitScaling m_unitScaling;

    GCRefPtr<SourceGraphic> m_sourceGraphic;
    GCRefPtr<FilterEffect> m_lastEffect;
};

} // namespace blink

#endif // Filter_h
