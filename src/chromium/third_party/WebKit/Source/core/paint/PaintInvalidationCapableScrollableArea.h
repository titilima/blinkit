// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: PaintInvalidationCapableScrollableArea.h
// Description: PaintInvalidationCapableScrollableArea Class
//      Author: Ziming Li
//     Created: 2019-05-14
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PaintInvalidationCapableScrollableArea_h
#define PaintInvalidationCapableScrollableArea_h

#include "core/CoreExport.h"
#include "platform/geometry/LayoutRect.h"
#include "platform/scroll/ScrollableArea.h"

namespace blink {

class LayoutBox;
class LayoutBoxModelObject;
class LayoutScrollbarPart;
class PaintInvalidationContainer;
class PaintInvalidationState;

// Base class of FrameView and PaintLayerScrollableArea to share paint invalidation code.
// TODO(wangxianzhu): Combine this into PaintLayerScrollableArea when root-layer-scrolls launches.
class CORE_EXPORT PaintInvalidationCapableScrollableArea : public ScrollableArea {
public:
    PaintInvalidationCapableScrollableArea()
        : m_horizontalScrollbarPreviouslyWasOverlay(false)
        , m_verticalScrollbarPreviouslyWasOverlay(false) { }

#ifndef BLINKIT_CRAWLER_ONLY
    void willRemoveScrollbar(Scrollbar&, ScrollbarOrientation) override;
#endif

    void invalidatePaintOfScrollControlsIfNeeded(const PaintInvalidationState&, const LayoutBoxModelObject& paintInvalidationContainer);

    // Should be called when the previous paint invalidation rects are no longer valid.
    void clearPreviousPaintInvalidationRects();

private:
    virtual LayoutBox& boxForScrollControlPaintInvalidation() const = 0;
    virtual LayoutScrollbarPart* scrollCorner() const = 0;
    virtual LayoutScrollbarPart* resizer() const = 0;

    bool m_horizontalScrollbarPreviouslyWasOverlay;
    bool m_verticalScrollbarPreviouslyWasOverlay;
    LayoutRect m_horizontalScrollbarPreviousPaintInvalidationRect;
    LayoutRect m_verticalScrollbarPreviousPaintInvalidationRect;
    LayoutRect m_scrollCornerPreviousPaintInvalidationRect;
};

} // namespace blink

#endif // PaintInvalidationCapableScrollableArea_h
