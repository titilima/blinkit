// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: local_frame_view.cpp
// Description: LocalFrameView Class
//      Author: Ziming Li
//     Created: 2020-09-20
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "local_frame_view.h"

#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/visual_viewport.h"
#include "third_party/blink/renderer/core/layout/layout_view.h"
#include "third_party/blink/renderer/core/layout/text_autosizer.h"
#include "third_party/blink/renderer/core/paint/paint_layer.h"
#include "third_party/blink/renderer/core/page/page.h"
#include "third_party/blink/renderer/core/paint/paint_layer_scrollable_area.h"

namespace blink {

LocalFrameView::LocalFrameView(LocalFrame &frame, const IntRect &frameRect)
    : m_frame(frame), m_frameRect(frameRect), m_baseBackgroundColor(Color::kWhite)
{
}

void LocalFrameView::AddResizerArea(LayoutBox &resizerBox)
{
    ASSERT(false); // BKTODO:
}

std::shared_ptr<LocalFrameView> LocalFrameView::Create(LocalFrame &frame, const IntSize &initialSize)
{
    IntRect frameRect(IntPoint(), initialSize);
    std::shared_ptr<LocalFrameView> view = base::WrapShared(new LocalFrameView(frame, frameRect));
    view->SetLayoutSizeInternal(initialSize);
    view->Show();
    return view;
}

void LocalFrameView::DidAttachDocument(void)
{
    Page *page = m_frame->GetPage();
    ASSERT(nullptr != page);

    ASSERT(nullptr != m_frame->GetDocument());

    ScrollableArea &visualViewport = page->GetVisualViewport();
    ScrollableArea *layoutViewport = LayoutViewport();
    ASSERT(nullptr != layoutViewport);

    ASSERT(false); // BKTODO:
#if 0
    RootFrameViewport* root_frame_viewport =
        RootFrameViewport::Create(visual_viewport, *layout_viewport);
    viewport_scrollable_area_ = root_frame_viewport;

    page->GlobalRootScrollerController().InitializeViewportScrollCallback(
        *root_frame_viewport);
#endif
}

LayoutView* LocalFrameView::GetLayoutView(void) const
{
    return GetFrame().ContentLayoutObject();
}

Page* LocalFrameView::GetPage(void) const
{
    return GetFrame().GetPage();
}

ScrollingCoordinator* LocalFrameView::GetScrollingCoordinator(void) const
{
    if (Page *p = GetPage())
        return p->GetScrollingCoordinator();
    return nullptr;
}

PaintLayerScrollableArea* LocalFrameView::LayoutViewport(void) const
{
    if (LayoutView *layoutView = GetLayoutView())
        return layoutView->GetScrollableArea();
    return nullptr;
}

IntPoint LocalFrameView::Location(void) const
{
    IntPoint location(m_frameRect.Location());

#if 0 // BKTODO: Check if necessary.
    // As an optimization, we don't include the root layer's scroll offset in the
    // frame rect.  As a result, we don't need to recalculate the frame rect every
    // time the root layer scrolls, but we need to add it in here.
    LayoutEmbeddedContent* owner = frame_->OwnerLayoutObject();
    if (owner) {
        LayoutView* owner_layout_view = owner->View();
        DCHECK(owner_layout_view);
        if (owner_layout_view->HasOverflowClip()) {
            IntSize scroll_offset(owner_layout_view->ScrolledContentOffset());
            location.SaturatedMove(-scroll_offset.Width(), -scroll_offset.Height());
        }
    }
#endif
    return location;
}

void LocalFrameView::RemoveResizerArea(LayoutBox &resizerBox)
{
    if (!m_resizerAreas)
        return;

    ASSERT(false); // BKTODO:
}

void LocalFrameView::SetBaseBackgroundColor(const Color &backgroundColor)
{
    if (m_baseBackgroundColor == backgroundColor)
        return;

    m_baseBackgroundColor = backgroundColor;

    if (LayoutView *layoutView = GetLayoutView())
    {
        if (layoutView->Layer()->HasCompositedLayerMapping())
        {
            ASSERT(false); // BKTODO:
#if 0
            CompositedLayerMapping* composited_layer_mapping =
                layout_view->Layer()->GetCompositedLayerMapping();
            composited_layer_mapping->UpdateContentsOpaque();
            if (composited_layer_mapping->MainGraphicsLayer())
                composited_layer_mapping->MainGraphicsLayer()->SetNeedsDisplay();
            if (composited_layer_mapping->ScrollingContentsLayer())
                composited_layer_mapping->ScrollingContentsLayer()->SetNeedsDisplay();
#endif
        }
    }

    ASSERT(false); // BKTODO:
#if 0
    if (!ShouldThrottleRendering())
        GetPage()->Animator().ScheduleVisualUpdate(frame_.Get());
#endif
}

void LocalFrameView::SetInitialViewportSize(const IntSize &viewportSize)
{
    if (viewportSize == m_initialViewportSize)
        return;

    m_initialViewportSize = viewportSize;
    if (Document *document = m_frame->GetDocument())
        document->GetStyleEngine().InitialViewportChanged();
}

void LocalFrameView::SetLayoutSizeFixedToFrameSize(bool isFixed)
{
    if (m_layoutSizeFixedToFrameSize == isFixed)
        return;

    m_layoutSizeFixedToFrameSize = isFixed;
    if (isFixed)
        SetLayoutSizeInternal(Size());
}

void LocalFrameView::SetLayoutSizeInternal(const IntSize &size)
{
    if (m_layoutSize == size)
        return;
    m_layoutSize = size;

    if (Document *document = m_frame->GetDocument())
    {
        if (TextAutosizer *textAutosizer = document->GetTextAutosizer())
            textAutosizer->UpdatePageInfoInAllFrames();
    }

    ASSERT(false); // BKTODO: SetNeedsLayout();
}

void LocalFrameView::SetNeedsCompositingUpdate(CompositingUpdateType updateType)
{
    if (LayoutView *layoutView = GetLayoutView())
    {
        if (m_frame->GetDocument()->IsActive())
            layoutView->Compositor()->SetNeedsCompositingUpdate(updateType);
    }
}

void LocalFrameView::SetSelfVisible(bool visible)
{
    if (visible != m_selfVisible)
    {
        // Frame view visibility affects PLC::CanBeComposited, which in turn
        // affects compositing inputs.
        if (LayoutView *view = GetLayoutView())
            ASSERT(false); // BKTODO: view->Layer()->SetNeedsCompositingInputsUpdate();
    }
    m_selfVisible = visible;
}

void LocalFrameView::Show(void)
{
    if (!IsSelfVisible())
    {
        SetSelfVisible(true);
        if (GetScrollingCoordinator())
            ASSERT(false); // BKTODO: GetScrollingContext()->SetScrollGestureRegionIsDirty(true);
        SetNeedsCompositingUpdate(kCompositingUpdateRebuildTree);
#if 0 // BKTODO: Check if necessary.
        if (IsParentVisible()) {
            ForAllChildViewsAndPlugins(
                [](EmbeddedContentView& embedded_content_view) {
                    embedded_content_view.SetParentVisible(true);
                });
        }
#endif
    }
}

void LocalFrameView::UpdateBaseBackgroundColorRecursively(const Color &baseBackgroundColor)
{
#if 0 // BKTODO: Check the logic
    ForAllNonThrottledLocalFrameViews(
        [base_background_color](LocalFrameView& frame_view) {
            frame_view.SetBaseBackgroundColor(base_background_color);
        });
#else
    SetBaseBackgroundColor(baseBackgroundColor);
#endif
}

void LocalFrameView::WillBeRemovedFromFrame(void)
{
    ASSERT(false); // BKTODO:
}

} // namespace blink
