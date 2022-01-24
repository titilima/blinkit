#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSCursorImageValue.h
// Description: CSSCursorImageValue Class
//      Author: Ziming Li
//     Created: 2021-10-19
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006 Rob Buis <buis@kde.org>
 * Copyright (C) 2008 Apple Inc. All right reserved.
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

#ifndef CSSCursorImageValue_h
#define CSSCursorImageValue_h

#include "blinkit/blink/renderer/core/css/CSSImageValue.h"
#include "blinkit/blink/renderer/platform/geometry/int_point.h"
#include "blinkit/blink/renderer/wtf/HashSet.h"

namespace blink {

class Element;
class SVGElement;

class CSSCursorImageValue : public CSSValue {
public:
    static GCRefPtr<CSSCursorImageValue> create(const GCRefPtr<CSSValue> &imageValue, bool hotSpotSpecified, const IntPoint& hotSpot)
    {
        return BlinKit::GCWrapShared(new CSSCursorImageValue(imageValue, hotSpotSpecified, hotSpot));
    }

    ~CSSCursorImageValue();

    bool hotSpotSpecified() const { return m_hotSpotSpecified; }

    IntPoint hotSpot() const { return m_hotSpot; }

    String customCSSText() const;

    bool updateIfSVGCursorIsUsed(Element*);
    bool isCachePending(float deviceScaleFactor) const;
    StyleImage* cachedImage(float deviceScaleFactor) const;
    StyleImage* cacheImage(Document*, float deviceScaleFactor);

#if !ENABLE(OILPAN)
    void removeReferencedElement(SVGElement*);
#endif

    bool equals(const CSSCursorImageValue&) const;

    DECLARE_TRACE_AFTER_DISPATCH();

private:
    CSSCursorImageValue(const GCRefPtr<CSSValue> &imageValue, bool hotSpotSpecified, const IntPoint& hotSpot);

    bool isSVGCursor() const;
    String cachedImageURL();
    void clearImageResource();

    GCRefPtr<CSSValue> m_imageValue;

    bool m_hotSpotSpecified;
    IntPoint m_hotSpot;
    bool m_isCachePending;
    RefPtrWillBeMember<StyleImage> m_cachedImage;

#if !ENABLE(OILPAN)
    HashSet<SVGElement*> m_referencedElements;
#endif
};

DEFINE_CSS_VALUE_TYPE_CASTS(CSSCursorImageValue, isCursorImageValue());

} // namespace blink

#endif // CSSCursorImageValue_h
