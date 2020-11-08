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
#include "third_party/blink/renderer/core/frame/root_frame_viewport.h"
#include "third_party/blink/renderer/core/frame/visual_viewport.h"
#include "third_party/blink/renderer/core/layout/layout_view.h"
#include "third_party/blink/renderer/core/layout/text_autosizer.h"
#include "third_party/blink/renderer/core/paint/paint_layer.h"
#include "third_party/blink/renderer/core/page/page.h"
#include "third_party/blink/renderer/core/page/scrolling/top_document_root_scroller_controller.h"
#include "third_party/blink/renderer/core/paint/paint_layer_scrollable_area.h"

namespace blink {

LocalFrameView::LocalFrameView(LocalFrame &frame, const IntRect &frameRect)
    : m_frame(frame), m_frameRect(frameRect), m_baseBackgroundColor(Color::kWhite)
{
}

void LocalFrameView::AddAnimatingScrollableArea(PaintLayerScrollableArea *scrollableArea)
{
    ASSERT(false); // BKTODO:
}

void LocalFrameView::AddResizerArea(LayoutBox &resizerBox)
{
    ASSERT(false); // BKTODO:
}

void LocalFrameView::AddScrollableArea(PaintLayerScrollableArea *scrollableArea)
{
    ASSERT(false); // BKTODO:
}

void LocalFrameView::AddViewportConstrainedObject(LayoutObject &object)
{
    ASSERT(false); // BKTODO:
}

void LocalFrameView::BeginLifecycleUpdates(void)
{
    // Avoid pumping frames for the initially empty document.
    if (!GetFrame().Loader().StateMachine()->CommittedFirstRealDocumentLoad())
        return;
    ASSERT(false); // BKTODO:
#if 0
    lifecycle_updates_throttled_ = false;
    if (auto* owner = GetFrame().OwnerLayoutObject())
        owner->SetShouldCheckForPaintInvalidation();

    LayoutView* layout_view = GetLayoutView();
    bool layout_view_is_empty = layout_view && !layout_view->FirstChild();
    if (layout_view_is_empty && !DidFirstLayout() && !NeedsLayout()) {
        // Make sure a display:none iframe gets an initial layout pass.
        layout_view->SetNeedsLayout(LayoutInvalidationReason::kAddedToLayout,
            kMarkOnlyThis);
    }

    SetupRenderThrottling();
    UpdateRenderThrottlingStatus(hidden_for_throttling_, subtree_throttled_);
    // The compositor will "defer commits" for the main frame until we
    // explicitly request them.
    if (GetFrame().IsMainFrame())
        GetFrame().GetPage()->GetChromeClient().BeginLifecycleUpdates();
#endif
}

void LocalFrameView::ClearFragmentAnchor(void)
{
    m_fragmentAnchor = nullptr;
}

void LocalFrameView::ClearLayoutSubtreeRoot(const LayoutObject &root)
{
    m_layoutSubtreeRootList.Remove(const_cast<LayoutObject &>(root));
}

std::shared_ptr<LocalFrameView> LocalFrameView::Create(LocalFrame &frame, const IntSize &initialSize)
{
    IntRect frameRect(IntPoint(), initialSize);
    std::shared_ptr<LocalFrameView> view = base::WrapShared(new LocalFrameView(frame, frameRect));
    view->SetLayoutSizeInternal(initialSize);
    view->Show();
    return view;
}

void LocalFrameView::DequeueScrollAnchoringAdjustment(ScrollableArea *scrollableArea)
{
    BKLOG("// BKTODO: Process while EnqueueScrollAnchoringAdjustment implemented.");
}

void LocalFrameView::DidAttachDocument(void)
{
    Page *page = m_frame->GetPage();
    ASSERT(nullptr != page);

    ASSERT(nullptr != m_frame->GetDocument());

    ScrollableArea &visualViewport = page->GetVisualViewport();
    ScrollableArea *layoutViewport = LayoutViewport();
    ASSERT(nullptr != layoutViewport);

    m_viewportScrollableArea.reset(RootFrameViewport::Create(visualViewport, *layoutViewport));

    page->GlobalRootScrollerController().InitializeViewportScrollCallback(*m_viewportScrollableArea);
}

void LocalFrameView::Dispose(void)
{
    ASSERT(!IsInPerformLayout());

    // TODO(dcheng): It's wrong that the frame can be detached before the
    // LocalFrameView. Figure out what's going on and fix LocalFrameView to be
    // disposed with the correct timing.

    // We need to clear the RootFrameViewport's animator since it gets called
    // from non-GC'd objects and RootFrameViewport will still have a pointer to
    // this class.
    if (m_viewportScrollableArea)
        m_viewportScrollableArea->ClearScrollableArea();

#if 0 // BKTODO: Check this later.
    // Destroy |m_autoSizeInfo| as early as possible, to avoid dereferencing
    // partially destroyed |this| via |m_autoSizeInfo->m_frameView|.
    m_autoSizeInfo.Clear();

    // FIXME: Do we need to do something here for OOPI?
    HTMLFrameOwnerElement* owner_element = frame_->DeprecatedLocalOwner();
    // TODO(dcheng): It seems buggy that we can have an owner element that points
    // to another EmbeddedContentView. This can happen when a plugin element loads
    // a frame (EmbeddedContentView A of type LocalFrameView) and then loads a
    // plugin (EmbeddedContentView B of type WebPluginContainerImpl). In this
    // case, the frame's view is A and the frame element's
    // OwnedEmbeddedContentView is B. See https://crbug.com/673170 for an example.
    if (owner_element && owner_element->OwnedEmbeddedContentView() == this)
        owner_element->SetEmbeddedContentView(nullptr);


    ukm_aggregator_.reset();
    jank_tracker_->Dispose();
#endif

#if DCHECK_IS_ON()
    m_hasBeenDisposed = true;
#endif
}

void LocalFrameView::EnqueueScrollAnchoringAdjustment(ScrollableArea *scrollableArea)
{
    ASSERT(false); // BKTODO:
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

void LocalFrameView::HandleLoadCompleted(void)
{
#if 0 // BKTODO:
    // Once loading has completed, allow autoSize one last opportunity to
    // reduce the size of the frame.
    if (auto_size_info_)
        auto_size_info_->AutoSizeIfNeeded();
#endif

    // If there is a pending layout, the fragment anchor will be cleared when it
    // finishes.
    if (!NeedsLayout())
        ClearFragmentAnchor();
}

bool LocalFrameView::IsInPerformLayout(void) const
{
    return Lifecycle().GetState() == DocumentLifecycle::kInPerformLayout;
}

bool LocalFrameView::LayoutPending(void) const
{
    // FIXME: This should check Document::lifecycle instead.
    return m_hasPendingLayout;
}

PaintLayerScrollableArea* LocalFrameView::LayoutViewport(void) const
{
    if (LayoutView *layoutView = GetLayoutView())
        return layoutView->GetScrollableArea();
    return nullptr;
}

DocumentLifecycle& LocalFrameView::Lifecycle(void) const
{
    ASSERT(m_frame);
    ASSERT(nullptr != m_frame->GetDocument());
    return m_frame->GetDocument()->Lifecycle();
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

bool LocalFrameView::NeedsLayout(void) const
{
    // This can return true in cases where the document does not have a body yet.
    // Document::shouldScheduleLayout takes care of preventing us from scheduling
    // layout in that case.
    LayoutView *layoutView = GetLayoutView();
    return LayoutPending() || (nullptr != layoutView && layoutView->NeedsLayout()) || IsSubtreeLayout();
}

void LocalFrameView::RemoveAnimatingScrollableArea(PaintLayerScrollableArea *scrollableArea)
{
    if (!m_animatingScrollableAreas)
        return;

    ASSERT(false); // BKTODO:
}

void LocalFrameView::RemoveResizerArea(LayoutBox &resizerBox)
{
    if (!m_resizerAreas)
        return;

    ASSERT(false); // BKTODO:
}

void LocalFrameView::RemoveScrollableArea(PaintLayerScrollableArea *scrollableArea)
{
    if (!m_scrollableAreas)
        return;

    ASSERT(false); // BKTODO:
}

void LocalFrameView::RemoveViewportConstrainedObject(LayoutObject &object)
{
    if (!m_viewportConstrainedObjects)
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

void LocalFrameView::SetLayoutSize(const IntSize &size)
{
    ASSERT(!LayoutSizeFixedToFrameSize());
    if (Document *document = m_frame->GetDocument())
    {
        if (document->Lifecycle().LifecyclePostponed())
            return;
    }

    SetLayoutSizeInternal(size);
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
