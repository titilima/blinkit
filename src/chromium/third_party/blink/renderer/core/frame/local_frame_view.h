// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: local_frame_view.h
// Description: LocalFrameView Class
//      Author: Ziming Li
//     Created: 2020-09-20
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_LOCAL_FRAME_VIEW_H
#define BLINKIT_BLINK_LOCAL_FRAME_VIEW_H

#pragma once

#include <unordered_set>
#include "third_party/blink/renderer/core/frame/frame_view.h"
#include "third_party/blink/renderer/core/paint/compositing/paint_layer_compositor.h"
#include "third_party/blink/renderer/platform/geometry/int_rect.h"
#include "third_party/blink/renderer/platform/geometry/layout_size.h"
#include "third_party/blink/renderer/platform/graphics/color.h"

namespace blink {

class LayoutBox;
class LayoutView;
class PaintLayerScrollableArea;
class ScrollingCoordinator;

class LocalFrameView final : public FrameView
{
public:
    static std::shared_ptr<LocalFrameView> Create(LocalFrame &frame, const IntSize &initialSize);

    LocalFrame& GetFrame(void) const
    {
        ASSERT(m_frame);
        return *m_frame;
    }
    Page* GetPage(void) const;
    LayoutView* GetLayoutView(void) const;
    IntRect FrameRect(void) const { return IntRect(Location(), Size()); }
    IntPoint Location(void) const;
    int X(void) const { return Location().X(); }
    int Y(void) const { return Location().Y(); }
    int Width(void) const { return Size().Width(); }
    int Height(void) const { return Size().Height(); }
    IntSize Size(void) const { return m_frameRect.Size(); }
    bool IsSelfVisible(void) const { return m_selfVisible; }
    void SetSelfVisible(bool visible);
    bool CanHaveScrollbars(void) const { return m_canHaveScrollbars; }

    // If this is set to false, the layout size will need to be explicitly set by
    // the owner.  E.g. WebViewImpl sets its mainFrame's layout size manually
    void SetLayoutSizeFixedToFrameSize(bool isFixed);
    void SetNeedsUpdateGeometries(void) { m_needsUpdateGeometries = true; }

    void SetInitialViewportSize(const IntSize &viewportSize);

    void SetBaseBackgroundColor(const Color &backgroundColor);
    void UpdateBaseBackgroundColorRecursively(const Color &baseBackgroundColor);

    // Returns the ScrollableArea of the LayoutView, a.k.a. the layout viewport.
    // In the root frame, this is the "outer" viewport in the pinch-zoom dual
    // viewport model.  Callers that need awareness of both inner and outer
    // viewports should use GetScrollableArea() instead.
    PaintLayerScrollableArea* LayoutViewport(void) const;

    // With CSS style "resize:" enabled, a little resizer handle will appear at
    // the bottom right of the object. We keep track of these resizer areas for
    // checking if touches (implemented using Scroll gesture) are targeting the
    // resizer.
    typedef std::unordered_set<LayoutBox *> ResizerAreaSet;
    void AddResizerArea(LayoutBox &resizerBox);
    void RemoveResizerArea(LayoutBox &resizerBox);
    const ResizerAreaSet* ResizerAreas(void) const { return m_resizerAreas.get(); }

    void DidAttachDocument(void);
    // Called when this view is going to be removed from its owning
    // LocalFrame.
    void WillBeRemovedFromFrame(void);

    void IncrementLayoutObjectCount(void) {} // Just a placeholder
private:
    explicit LocalFrameView(LocalFrame &frame, const IntRect &frameRect);

    ScrollingCoordinator* GetScrollingCoordinator(void) const;
    void SetLayoutSizeInternal(const IntSize &size);
    void SetNeedsCompositingUpdate(CompositingUpdateType updateType);

    void Show(void) override;

    Member<LocalFrame> m_frame;
    IntRect m_frameRect;
    bool m_selfVisible = false;
    bool m_canHaveScrollbars = true;
    std::unique_ptr<ResizerAreaSet> m_resizerAreas;
    IntSize m_layoutSize;
    IntSize m_initialViewportSize;
    Color m_baseBackgroundColor;
    bool m_layoutSizeFixedToFrameSize = true;
    bool m_needsUpdateGeometries = false;
};

} // namespace blink

#endif // BLINKIT_BLINK_LOCAL_FRAME_VIEW_H
