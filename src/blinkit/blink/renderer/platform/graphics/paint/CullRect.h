#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CullRect.h
// Description: CullRect Class
//      Author: Ziming Li
//     Created: 2022-01-23
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CullRect_h
#define CullRect_h

#include <limits>
#include "blinkit/blink/renderer/platform/geometry/int_rect.h"
#include "blinkit/blink/renderer/platform/transforms/AffineTransform.h"
#include "blinkit/blink/renderer/wtf/HashMap.h"
#include "blinkit/blink/renderer/wtf/ListHashSet.h"


namespace blink {

class FloatRect;
class LayoutBoxModelObject;
class LayoutInline;
class LayoutObject;
class LayoutRect;
class LayoutUnit;
class PaintInvalidationState;

class CullRect
{
    DISALLOW_NEW();
public:
    explicit CullRect(const IntRect& rect) : m_rect(rect) { }
    CullRect(const CullRect&, const IntPoint& offset);
    CullRect(const CullRect&, const IntSize& offset);

    bool intersectsCullRect(const AffineTransform&, const FloatRect& boundingBox) const;
    void updateCullRect(const AffineTransform& localToParentTransform);
    bool intersectsCullRect(const IntRect&) const;
    bool intersectsCullRect(const LayoutRect&) const;
    bool intersectsHorizontalRange(LayoutUnit lo, LayoutUnit hi) const;
    bool intersectsVerticalRange(LayoutUnit lo, LayoutUnit hi) const;

private:
    IntRect m_rect;

    // TODO(chrishtr): temporary while we implement CullRect everywhere.
    friend class FramePainter;
    friend class GridPainter;
    friend class SVGInlineTextBoxPainter;
    friend class ScrollbarThemeMacNonOverlayAPI;
    friend class ReplicaPainter;
    friend class SVGPaintContext;
    friend class SVGRootInlineBoxPainter;
    friend class SVGShapePainter;
    friend class TableSectionPainter;
    friend class ThemePainterMac;
    friend class WebPluginContainerImpl;
};

} // namespace blink

#endif // CullRect_h
