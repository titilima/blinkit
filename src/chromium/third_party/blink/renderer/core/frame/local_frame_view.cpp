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

#include "third_party/blink/renderer/core/css/font_face_set_document.h"
#include "third_party/blink/renderer/core/editing/frame_selection.h"
#include "third_party/blink/renderer/core/frame/browser_controls.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_frame_client.h"
#include "third_party/blink/renderer/core/frame/root_frame_viewport.h"
#include "third_party/blink/renderer/core/frame/settings.h"
#include "third_party/blink/renderer/core/frame/visual_viewport.h"
#include "third_party/blink/renderer/core/input/event_handler.h"
#include "third_party/blink/renderer/core/layout/layout_view.h"
#include "third_party/blink/renderer/core/layout/text_autosizer.h"
#include "third_party/blink/renderer/core/paint/paint_layer.h"
#include "third_party/blink/renderer/core/page/chrome_client.h"
#include "third_party/blink/renderer/core/page/page.h"
#include "third_party/blink/renderer/core/page/scrolling/snap_coordinator.h"
#include "third_party/blink/renderer/core/page/scrolling/top_document_root_scroller_controller.h"
#include "third_party/blink/renderer/core/paint/paint_layer_scrollable_area.h"
#include "third_party/blink/renderer/platform/bindings/script_forbidden_scope.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_fetcher.h"

// Used to check for dirty layouts violating document lifecycle rules.
// If arg evaluates to true, the program will continue. If arg evaluates to
// false, program will crash if DCHECK_IS_ON() or return false from the current
// function.
#define CHECK_FOR_DIRTY_LAYOUT(arg) \
    do {                            \
        if (!(arg)) {               \
            NOTREACHED();           \
            return false;           \
        }                           \
    } while (false)

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

void LocalFrameView::AddScrollbar(Scrollbar *scrollbar)
{
    ASSERT(!m_scrollbars.Contains(scrollbar));
    m_scrollbars.insert(scrollbar);
}

void LocalFrameView::AddViewportConstrainedObject(LayoutObject &object)
{
    ASSERT(false); // BKTODO:
}

void LocalFrameView::AdjustViewSize(void)
{
    if (m_suppressAdjustViewSize)
        return;

    if (LayoutView *layoutView = GetLayoutView())
    {
        ASSERT(m_frame->View() == this);
        SetLayoutOverflowSize(layoutView->DocumentRect().Size());
    }
}

void LocalFrameView::BeginLifecycleUpdates(void)
{
    // Avoid pumping frames for the initially empty document.
    if (!GetFrame().Loader().StateMachine()->CommittedFirstRealDocumentLoad())
        return;
    m_lifecycleUpdatesThrottled = false;
#if 0 // BKTODO: Check if necessary.
    if (auto* owner = GetFrame().OwnerLayoutObject())
        owner->SetShouldCheckForPaintInvalidation();
#endif

    LayoutView *layoutView = GetLayoutView();
    bool layoutViewIsEmpty = nullptr != layoutView && nullptr == layoutView->FirstChild();
    if (layoutViewIsEmpty && !DidFirstLayout() && !NeedsLayout())
    {
        // Make sure a display:none iframe gets an initial layout pass.
        layoutView->SetNeedsLayout(LayoutInvalidationReason::kAddedToLayout, kMarkOnlyThis);
    }

    SetupRenderThrottling();
    UpdateRenderThrottlingStatus(m_hiddenForThrottling, m_subtreeThrottled);
    GetFrame().GetPage()->GetChromeClient().BeginLifecycleUpdates();
}

bool LocalFrameView::CanThrottleRendering(void) const
{
    if (m_lifecycleUpdatesThrottled)
        return true;
    if (!RuntimeEnabledFeatures::RenderingPipelineThrottlingEnabled())
        return false;
    if (m_subtreeThrottled)
        return true;
    return false;
#if 0 // BKTODO:
    // We only throttle hidden cross-origin frames. This is to avoid a situation
    // where an ancestor frame directly depends on the pipeline timing of a
    // descendant and breaks as a result of throttling. The rationale is that
    // cross-origin frames must already communicate with asynchronous messages,
    // so they should be able to tolerate some delay in receiving replies from a
    // throttled peer.
    return m_hiddenForThrottling && frame_->IsCrossOriginSubframe();
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

void LocalFrameView::ClearLayoutSubtreeRootsAndMarkContainingBlocks(void)
{
    m_layoutSubtreeRootList.ClearAndMarkContainingBlocksForLayout();
}

bool LocalFrameView::CheckDoesNotNeedLayout(void) const
{
    CHECK_FOR_DIRTY_LAYOUT(!LayoutPending());
    CHECK_FOR_DIRTY_LAYOUT(nullptr == GetLayoutView() || !GetLayoutView()->NeedsLayout());
    CHECK_FOR_DIRTY_LAYOUT(!IsSubtreeLayout());
    return true;
}

bool LocalFrameView::CheckLayoutInvalidationIsAllowed(void) const
{
#if DCHECK_IS_ON()
    if (m_allowsLayoutInvalidationAfterLayoutClean)
        return true;

    // If we are updating all lifecycle phases beyond LayoutClean, we don't expect
    // dirty layout after LayoutClean.
    CHECK_FOR_DIRTY_LAYOUT(Lifecycle().GetState() < DocumentLifecycle::kLayoutClean);
#endif
    return true;
}

LocalFrameView* LocalFrameView::Create(LocalFrame &frame, const IntSize &initialSize)
{
    LocalFrameView *view = new LocalFrameView(frame, IntRect(IntPoint(), initialSize));
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

    m_viewportScrollableArea = RootFrameViewport::Create(visualViewport, *layoutViewport);

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

void LocalFrameView::FrameRectsChanged(void)
{
    if (LayoutSizeFixedToFrameSize())
        SetLayoutSizeInternal(Size());
#if 0 // BKTODO: May be useful in the future, leave it here.
    GetFrame().Client()->FrameRectsChanged(FrameRect());
#endif
}

LayoutRect LocalFrameView::FrameToDocument(const LayoutRect &rectInFrame) const
{
    ASSERT(false); // BKTODO:
    return LayoutRect();
#if 0
    return LayoutRect(FrameToDocument(rect_in_frame.Location()),
        rect_in_frame.Size());
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

ScrollableArea* LocalFrameView::GetScrollableArea(void)
{
    if (m_viewportScrollableArea)
        return m_viewportScrollableArea.Get();
    return LayoutViewport();
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

void LocalFrameView::IncrementVisuallyNonEmptyCharacterCount(unsigned count)
{
    if (m_isVisuallyNonEmpty)
        return;
    m_visuallyNonEmptyCharacterCount += count;
    // Use a threshold value to prevent very small amounts of visible content from
    // triggering didMeaningfulLayout.  The first few hundred characters rarely
    // contain the interesting content of the page.
    static const unsigned kVisualCharacterThreshold = 200;
    if (m_visuallyNonEmptyCharacterCount > kVisualCharacterThreshold)
        SetIsVisuallyNonEmpty();
}

bool LocalFrameView::IsInPerformLayout(void) const
{
    return Lifecycle().GetState() == DocumentLifecycle::kInPerformLayout;
}

void LocalFrameView::LayoutFromRootObject(LayoutObject &root)
{
    ASSERT(false); // BKTODO:
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

void LocalFrameView::MarkViewportConstrainedObjectsForLayout(bool widthChanged, bool heightChanged)
{
    if (!HasViewportConstrainedObjects() || !(widthChanged || heightChanged))
        return;

    ASSERT(false); // BKTODO:
#if 0
    for (auto* const viewport_constrained_object :
    *viewport_constrained_objects_) {
        LayoutObject* layout_object = viewport_constrained_object;
        const ComputedStyle& style = layout_object->StyleRef();
        if (width_changed) {
            if (style.Width().IsFixed() &&
                (style.Left().IsAuto() || style.Right().IsAuto())) {
                layout_object->SetNeedsPositionedMovementLayout();
            }
            else {
                layout_object->SetNeedsLayoutAndFullPaintInvalidation(
                    LayoutInvalidationReason::kSizeChanged);
            }
        }
        if (height_changed) {
            if (style.Height().IsFixed() &&
                (style.Top().IsAuto() || style.Bottom().IsAuto())) {
                layout_object->SetNeedsPositionedMovementLayout();
            }
            else {
                layout_object->SetNeedsLayoutAndFullPaintInvalidation(
                    LayoutInvalidationReason::kSizeChanged);
            }
        }
    }
#endif
}

bool LocalFrameView::NeedsLayout(void) const
{
    // This can return true in cases where the document does not have a body yet.
    // Document::shouldScheduleLayout takes care of preventing us from scheduling
    // layout in that case.
    LayoutView *layoutView = GetLayoutView();
    return LayoutPending() || (nullptr != layoutView && layoutView->NeedsLayout()) || IsSubtreeLayout();
}

void LocalFrameView::PerformLayout(bool inSubtreeLayout)
{
    ASSERT(inSubtreeLayout || m_layoutSubtreeRootList.IsEmpty());

    int contentsHeightBeforeLayout = GetLayoutView()->DocumentRect().Height();
#if 0 // BKTODO: Check if necessary.
    PrepareLayoutAnalyzer();
#endif

    ScriptForbiddenScope forbidScript;

    if (inSubtreeLayout && HasOrthogonalWritingModeRoots())
    {
        // If we're going to lay out from each subtree root, rather than once from
        // LayoutView, we need to merge the depth-ordered orthogonal writing mode
        // root list into the depth-ordered list of subtrees scheduled for
        // layout. Otherwise, during layout of one such subtree, we'd risk skipping
        // over a subtree of objects needing layout.
        ASSERT(!m_layoutSubtreeRootList.IsEmpty());
        ASSERT(false); // BKTODO: ScheduleOrthogonalWritingModeRootsForLayout();
    }

    ASSERT(!IsInPerformLayout());
    Lifecycle().AdvanceTo(DocumentLifecycle::kInPerformLayout);

    // performLayout is the actual guts of layout().
    // FIXME: The 300 other lines in layout() probably belong in other helper
    // functions so that a single human could understand what layout() is actually
    // doing.

    {
        // TODO(szager): Remove this after diagnosing crash.
        DocumentLifecycle::CheckNoTransitionScope checkNoTransition(Lifecycle());
        if (inSubtreeLayout)
        {
#if 0 // BKTODO: Check if necessary.
            if (analyzer_) {
                analyzer_->Increment(LayoutAnalyzer::kPerformLayoutRootLayoutObjects,
                    layout_subtree_root_list_.size());
            }
#endif
            for (auto &root : m_layoutSubtreeRootList.Ordered())
            {
                if (!root->NeedsLayout())
                    continue;
                LayoutFromRootObject(*root);

                // We need to ensure that we mark up all layoutObjects up to the
                // LayoutView for paint invalidation. This simplifies our code as we
                // just always do a full tree walk.
                if (LayoutObject *container = root->Container())
                    container->SetShouldCheckForPaintInvalidation();
            }
            m_layoutSubtreeRootList.Clear();
        }
        else
        {
            if (HasOrthogonalWritingModeRoots())
                ASSERT(false); // BKTODO: LayoutOrthogonalWritingModeRoots();
            GetLayoutView()->UpdateLayout();
        }
    }

    m_frame->GetDocument()->Fetcher()->UpdateAllImageResourcePriorities();

    Lifecycle().AdvanceTo(DocumentLifecycle::kAfterPerformLayout);

#if 0 // BKTODO: Check if necessary.
    FirstMeaningfulPaintDetector::From(*frame_->GetDocument())
        .MarkNextPaintAsMeaningfulIfNeeded(
            layout_object_counter_, contents_height_before_layout,
            GetLayoutView()->DocumentRect().Height(), Height());
#endif
}

void LocalFrameView::PerformPostLayoutTasks(void)
{
    // FIXME: We can reach here, even when the page is not active!
    // http/tests/inspector/elements/html-link-import.html and many other
    // tests hit that case.
    // We should DCHECK(isActive()); or at least return early if we can!

    // Always called before or after performLayout(), part of the highest-level
    // layout() call.
    ASSERT(!IsInPerformLayout());

    m_frame->Selection().DidLayout();

    ASSERT(nullptr != m_frame->GetDocument());

    FontFaceSetDocument::DidLayout(*m_frame->GetDocument());
    // Fire a fake a mouse move event to update hover state and mouse cursor, and
    // send the right mouse out/over events.
    m_frame->GetEventHandler().MayUpdateHoverWhenContentUnderMouseChanged(
        MouseEventManager::UpdateHoverReason::kLayoutOrStyleChanged);

    UpdateGeometriesIfNeeded();

    // Plugins could have torn down the page inside updateGeometries().
    if (nullptr == GetLayoutView())
        return;

#if 0 // BKTODO: Check if necessary.
    ScheduleUpdatePluginsIfNecessary();
#endif

    if (ScrollingCoordinator *scrollingCoordinator = GetScrollingCoordinator())
        ASSERT(false); // BKTODO: scrollingCoordinator->NotifyGeometryChanged(this);

    if (SnapCoordinator *snapCoordinator = m_frame->GetDocument()->GetSnapCoordinator())
        snapCoordinator->UpdateAllSnapContainerData();

    SendResizeEventIfNeeded();
}

void LocalFrameView::PerformPreLayoutTasks(void)
{
    Lifecycle().AdvanceTo(DocumentLifecycle::kInPreLayout);

    // Don't schedule more layouts, we're in one.
    base::AutoReset<bool> changeSchedulingEnabled(&m_layoutSchedulingEnabled, false);

    bool wasResized = WasViewportResized();
    Document *document = m_frame->GetDocument();
    if (wasResized)
        document->SetResizedForViewportUnits();

    // Viewport-dependent or device-dependent media queries may cause us to need
    // completely different style information.
    bool mainFrameRotation = Settings::MainFrameResizesAreOrientationChanges;
    if ((wasResized && document->GetStyleEngine().MediaQueryAffectedByViewportChange())
        || (wasResized && mainFrameRotation && document->GetStyleEngine().MediaQueryAffectedByDeviceChange()))
    {
        ASSERT(false); // BKTODO: document->MediaQueryAffectingValueChanged();
    }
    else if (wasResized)
    {
        document->EvaluateMediaQueryList();
    }

    document->UpdateStyleAndLayoutTree();
    Lifecycle().AdvanceTo(DocumentLifecycle::kStyleClean);

    if (wasResized)
        document->ClearResizedForViewportUnits();
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

void LocalFrameView::RemoveScrollbar(Scrollbar *scrollbar)
{
    ASSERT(m_scrollbars.Contains(scrollbar));
    m_scrollbars.erase(scrollbar);
}

void LocalFrameView::RemoveViewportConstrainedObject(LayoutObject &object)
{
    if (!m_viewportConstrainedObjects)
        return;

    ASSERT(false); // BKTODO:
}

void LocalFrameView::ScheduleRelayout(void)
{
    ASSERT(m_frame->View() == this);

    if (!m_layoutSchedulingEnabled)
        return;
    // TODO(crbug.com/590856): It's still broken when we choose not to crash when
    // the check fails.
    if (!CheckLayoutInvalidationIsAllowed())
        return;
    if (!NeedsLayout())
        return;
    if (!m_frame->GetDocument()->ShouldScheduleLayout())
        return;

    ClearLayoutSubtreeRootsAndMarkContainingBlocks();

    if (m_hasPendingLayout)
        return;
    m_hasPendingLayout = true;

    if (!ShouldThrottleRendering())
        GetPage()->Animator().ScheduleVisualUpdate(m_frame.Get());
}

void LocalFrameView::ScheduleVisualUpdateForPaintInvalidationIfNeeded(void)
{
    if (m_currentUpdateLifecyclePhasesTargetState < DocumentLifecycle::kPrePaintClean
        || Lifecycle().GetState() >= DocumentLifecycle::kPrePaintClean)
    {
        // Schedule visual update to process the paint invalidation in the next
        // cycle.
        GetFrame().ScheduleVisualUpdateUnlessThrottled();
    }
    // Otherwise the paint invalidation will be handled in the pre-paint
    // phase of this cycle.
}

void LocalFrameView::SendResizeEventIfNeeded(void)
{
    ASSERT(m_frame);

    LayoutView *layoutView = GetLayoutView();
    if (nullptr == layoutView)
        return;

    if (!WasViewportResized())
        return;

    m_lastViewportSize = GetLayoutSize();
    m_lastZoomFactor = layoutView->StyleRef().Zoom();

    Document *document = m_frame->GetDocument();
    document->EnqueueVisualViewportResizeEvent();
    document->EnqueueResizeEvent();
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

    if (!ShouldThrottleRendering())
        GetPage()->Animator().ScheduleVisualUpdate(m_frame.Get());
}

void LocalFrameView::SetFrameRect(const IntRect &unclampedFrameRect)
{
    const IntRect frameRect(SaturatedRect(unclampedFrameRect));

    if (frameRect == m_frameRect)
        return;

    const bool widthChanged = m_frameRect.Width() != frameRect.Width();
    const bool heightChanged = m_frameRect.Height() != frameRect.Height();
    m_frameRect = frameRect;

    FrameRectsChanged();

    if (LayoutView *layoutView = GetLayoutView())
        layoutView->SetShouldCheckForPaintInvalidation();

    if (widthChanged || heightChanged)
    {
        ViewportSizeChanged(widthChanged, heightChanged);

        m_frame->GetPage()->GetVisualViewport().MainFrameDidChangeSize();
#if 0 // BKTODO: Check if necessary.
        m_frame->Loader().RestoreScrollPositionAndViewState();
#endif
    }
}

void LocalFrameView::SetInitialViewportSize(const IntSize &viewportSize)
{
    if (viewportSize == m_initialViewportSize)
        return;

    m_initialViewportSize = viewportSize;
    if (Document *document = m_frame->GetDocument())
        document->GetStyleEngine().InitialViewportChanged();
}

void LocalFrameView::SetLayoutOverflowSize(const IntSize &size)
{
    if (size == m_layoutOverflowSize)
        return;

    m_layoutOverflowSize = size;

    if (Page *page = GetFrame().GetPage())
        page->GetChromeClient().ContentsSizeChanged(m_frame.Get(), size);
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

    SetNeedsLayout();
}

void LocalFrameView::SetNeedsCompositingUpdate(CompositingUpdateType updateType)
{
    if (LayoutView *layoutView = GetLayoutView())
    {
        if (m_frame->GetDocument()->IsActive())
            layoutView->Compositor()->SetNeedsCompositingUpdate(updateType);
    }
}

void LocalFrameView::SetNeedsLayout(void)
{
    if (LayoutView *layoutView = GetLayoutView())
    {
        if (CheckLayoutInvalidationIsAllowed())
            layoutView->SetNeedsLayout(LayoutInvalidationReason::kUnknown);
    }
}

void LocalFrameView::SetSelfVisible(bool visible)
{
    if (visible != m_selfVisible)
    {
        // Frame view visibility affects PLC::CanBeComposited, which in turn
        // affects compositing inputs.
        if (LayoutView *view = GetLayoutView())
            view->Layer()->SetNeedsCompositingInputsUpdate();
    }
    m_selfVisible = visible;
}

void LocalFrameView::SetupRenderThrottling(void)
{
    BKLOG("// BKTODO: Check if necessary.");
}

bool LocalFrameView::ShouldThrottleRendering(void) const
{
    bool throttledForGlobalReasons =
        CanThrottleRendering()
        && nullptr != m_frame->GetDocument()
        && Lifecycle().ThrottlingAllowed();
    if (!throttledForGlobalReasons || m_needsForcedCompositingUpdate)
        return false;

    ASSERT(false); // BKTODO:
    return false;
#if 0
    // Only lifecycle phases up to layout are needed to generate an
    // intersection observation.
    return intersection_observation_state_ != kRequired ||
        GetFrame().LocalFrameRoot().View()->past_layout_lifecycle_update_;
#endif
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

void LocalFrameView::Trace(Visitor *visitor)
{
    visitor->Trace(m_autoSizeInfo);
    visitor->Trace(m_scrollbars);
    visitor->Trace(m_viewportScrollableArea);
    FrameView::Trace(visitor);
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

void LocalFrameView::UpdateCountersAfterStyleChange(void)
{
    GetLayoutView()->UpdateCounters();
}

void LocalFrameView::UpdateDocumentAnnotatedRegions(void) const
{
#if 0 // BKTODO: Check if necessary.
    Document *document = m_frame->GetDocument();
    if (!document->HasAnnotatedRegions())
        return;
    Vector<AnnotatedRegionValue> new_regions;
    CollectAnnotatedRegions(*(document->GetLayoutBox()), new_regions);
    if (new_regions == document->AnnotatedRegions())
        return;
    document->SetAnnotatedRegions(new_regions);

    DCHECK(frame_->Client());
    frame_->Client()->AnnotatedRegionsChanged();
#endif
}

void LocalFrameView::UpdateGeometriesIfNeeded(void)
{
    if (!m_needsUpdateGeometries)
        return;
    m_needsUpdateGeometries = false;
#if 0 // BKTODO: Check the logic below.
    HeapVector<Member<EmbeddedContentView>> views;
    ForAllChildViewsAndPlugins(
        [&](EmbeddedContentView& view) { views.push_back(view); });

    for (const auto& view : views) {
        // Script or plugins could detach the frame so abort processing if that
        // happens.
        if (!GetLayoutView())
            break;

        view->UpdateGeometry();
    }
#endif
}

void LocalFrameView::UpdateLayout(void)
{
    // We should never layout a Document which is not in a LocalFrame.
    ASSERT(m_frame);
    ASSERT(m_frame->View() == this);
    ASSERT(nullptr != m_frame->GetPage());
    ASSERT(!m_frame->Client()->IsCrawler());

    {
        ScriptForbiddenScope forbidScript;

        if (IsInPerformLayout() || ShouldThrottleRendering() || !m_frame->GetDocument()->IsActive())
            return;

        // The actual call to UpdateGeometries is in PerformPostLayoutTasks.
        SetNeedsUpdateGeometries();

        if (m_autoSizeInfo)
            ASSERT(false); // BKTODO: m_autoSizeInfo->AutoSizeIfNeeded();

        m_hasPendingLayout = false;

        Document *document = m_frame->GetDocument();

        PerformPreLayoutTasks();

        VisualViewport &visualViewport = m_frame->GetPage()->GetVisualViewport();
        DoubleSize viewportSize(visualViewport.VisibleWidthCSSPx(), visualViewport.VisibleHeightCSSPx());

        // TODO(crbug.com/460956): The notion of a single root for layout is no
        // longer applicable. Remove or update this code.
        LayoutView *layoutView = GetLayoutView();
        LayoutObject *rootForThisLayout = layoutView;

        FontCachePurgePreventer fontCachePurgePreventer;
        {
            base::AutoReset<bool> changeSchedulingEnabled(&m_layoutSchedulingEnabled, false);
            ++m_nestedLayoutCount;

            // If the layout view was marked as needing layout after we added items in
            // the subtree roots we need to clear the roots and do the layout from the
            // layoutView.
            if (layoutView->NeedsLayout())
                ClearLayoutSubtreeRootsAndMarkContainingBlocks();
            layoutView->ClearHitTestCache();

            bool inSubtreeLayout = IsSubtreeLayout();

            // TODO(crbug.com/460956): The notion of a single root for layout is no
            // longer applicable. Remove or update this code.
            if (inSubtreeLayout)
                rootForThisLayout = m_layoutSubtreeRootList.RandomRoot();

            if (nullptr == rootForThisLayout)
            {
                // FIXME: Do we need to set m_size here?
                NOTREACHED();
                return;
            }

            if (!inSubtreeLayout)
            {
                ClearLayoutSubtreeRootsAndMarkContainingBlocks();
                Node *body = document->body();
                if (nullptr != body)
                {
                    if (LayoutObject *bodyLayoutObject = body->GetLayoutObject())
                    {
                        if (IsHTMLBodyElement(*body))
                        {
                            if (!m_firstLayout && m_size.Height() != GetLayoutSize().Height()
                                && bodyLayoutObject->EnclosingBox()->StretchesToViewport())
                            {
                                bodyLayoutObject->SetChildNeedsLayout();
                            }
                        }
                    }
                }

                if (m_firstLayout)
                {
                    m_firstLayout = false;
                    m_lastViewportSize = GetLayoutSize();
                    m_lastZoomFactor = layoutView->StyleRef().Zoom();

                    ScrollbarMode hMode;
                    ScrollbarMode vMode;
                    layoutView->CalculateScrollbarModes(hMode, vMode);
                    if (kScrollbarAuto == vMode)
                        layoutView->GetScrollableArea()->ForceVerticalScrollbarForFirstLayout();
                }

                LayoutSize oldSize = m_size;

                m_size = LayoutSize(GetLayoutSize());

                if (oldSize != m_size && !m_firstLayout)
                {
                    LayoutBox *rootLayoutObject = nullptr != document->documentElement()
                        ? document->documentElement()->GetLayoutBox()
                        : nullptr;
                    LayoutBox *bodyLayoutObject = nullptr != rootLayoutObject && nullptr != body
                        ? body->GetLayoutBox()
                        : nullptr;
                    if (nullptr != bodyLayoutObject && bodyLayoutObject->StretchesToViewport())
                        bodyLayoutObject->SetChildNeedsLayout();
                    else if (nullptr != rootLayoutObject && rootLayoutObject->StretchesToViewport())
                        rootLayoutObject->SetChildNeedsLayout();
                }
            }

            IntSize oldSize(Size());

            PerformLayout(inSubtreeLayout);

            IntSize newSize(Size());
            if (oldSize != newSize)
            {
                SetNeedsLayout();
                MarkViewportConstrainedObjectsForLayout(
                    oldSize.Width() != newSize.Width(),
                    oldSize.Height() != newSize.Height()
                );
            }

            if (NeedsLayout())
            {
                base::AutoReset<bool> suppress(&m_suppressAdjustViewSize, true);
                UpdateLayout();
            }

            ASSERT(m_layoutSubtreeRootList.IsEmpty());
        }  // Reset m_layoutSchedulingEnabled to its previous value.
        CheckDoesNotNeedLayout();

        DocumentLifecycle::Scope lifecycleScope(Lifecycle(), DocumentLifecycle::kLayoutClean);

        // FIXME: Could find the common ancestor layer of all dirty subtrees and
        // mark from there. crbug.com/462719
        layoutView->EnclosingLayer()->UpdateLayerPositionsAfterLayout();

        layoutView->Compositor()->DidLayout();

#if 0 // BKTODO: Check if necessary.
        if (AXObjectCache* cache = document->ExistingAXObjectCache()) {
            const KURL& url = document->Url();
            if (url.IsValid() && !url.IsAboutBlankURL()) {
                cache->HandleLayoutComplete(document);
                cache->ProcessUpdatesAfterLayout(*document);
            }
        }
#endif
        UpdateDocumentAnnotatedRegions();
        CheckDoesNotNeedLayout();

        if (1 == m_nestedLayoutCount)
        {
            PerformPostLayoutTasks();
            CheckDoesNotNeedLayout();
        }

        --m_nestedLayoutCount;
        if (0 != m_nestedLayoutCount)
            return;

#if DCHECK_IS_ON()
        // Post-layout assert that nobody was re-marked as needing layout during
        // layout.
        layoutView->AssertSubtreeIsLaidOut();
#endif

        // Scrollbars changing state can cause a visual viewport size change.
        DoubleSize newViewportSize(visualViewport.VisibleWidthCSSPx(), visualViewport.VisibleHeightCSSPx());
        if (newViewportSize != viewportSize)
            m_frame->GetDocument()->EnqueueVisualViewportResizeEvent();
    }  // ScriptForbiddenScope

    GetFrame().GetDocument()->LayoutUpdated();
    CheckDoesNotNeedLayout();
}

void LocalFrameView::UpdateRenderThrottlingStatus(
    bool hidden,
    bool subtreeThrottled,
    ForceThrottlingInvalidationBehavior forceThrottlingInvalidationHehavior,
    NotifyChildrenBehavior notifyChildrenBehavior)
{
    BKLOG("// BKTODO: Check if necessary.");
}

void LocalFrameView::ViewportSizeChanged(bool widthChanged, bool heightChanged)
{
    ASSERT(widthChanged || heightChanged);
    ASSERT(nullptr != m_frame->GetPage());

    Document *document = m_frame->GetDocument();
    if (nullptr != document && document->Lifecycle().LifecyclePostponed())
        return;

    LayoutView *layoutView = GetLayoutView();
    if (nullptr != layoutView)
    {
        // If this is the main frame, we might have got here by hiding/showing the
        // top controls.  In that case, layout won't be triggered, so we need to
        // clamp the scroll offset here.
        layoutView->Layer()->UpdateSize();
        layoutView->GetScrollableArea()->ClampScrollOffsetAfterOverflowChange();

        if (layoutView->UsesCompositing())
        {
            layoutView->Layer()->SetNeedsCompositingInputsUpdate();
            ASSERT(false); // BKTODO: SetNeedsPaintPropertyUpdate();
        }
    }

    if (nullptr != document)
        document->GetRootScrollerController().DidResizeFrameView();

    if (nullptr != layoutView && m_frame->GetPage()->GetBrowserControls().TotalHeight())
    {
        if (layoutView->StyleRef().HasFixedBackgroundImage())
        {
            // We've already issued a full invalidation above.
            layoutView->SetShouldDoFullPaintInvalidationOnResizeIfNeeded(widthChanged, heightChanged);
        }
        else if (heightChanged)
        {
            // If the document rect doesn't fill the full view height, hiding the
            // URL bar will expose area outside the current LayoutView so we need to
            // paint additional background. If RLS is on, we've already invalidated
            // above.
            ASSERT(nullptr != layoutView);
            if (layoutView->DocumentRect().Height() < layoutView->ViewRect().Height())
                layoutView->SetShouldDoFullPaintInvalidation(PaintInvalidationReason::kGeometry);
        }
    }

    if (nullptr != document && !IsInPerformLayout())
        MarkViewportConstrainedObjectsForLayout(widthChanged, heightChanged);
}

FloatSize LocalFrameView::ViewportSizeForViewportUnits(void) const
{
    LayoutView *layoutView = GetLayoutView();
    if (nullptr == layoutView)
        return FloatSize();

    float zoom = m_frame->PageZoomFactor();

    FloatSize layoutSize;
    layoutSize.SetWidth(layoutView->ViewWidth(kIncludeScrollbars) / zoom);
    layoutSize.SetHeight(layoutView->ViewHeight(kIncludeScrollbars) / zoom);

#if 0 // BKTODO:
    BrowserControls &browserControls = m_frame->GetPage()->GetBrowserControls();
    if (browserControls.PermittedState() != cc::BrowserControlsState::kHidden)
    {
        // We use the layoutSize rather than frameRect to calculate viewport units
        // so that we get correct results on mobile where the page is laid out into
        // a rect that may be larger than the viewport (e.g. the 980px fallback
        // width for desktop pages). Since the layout height is statically set to
        // be the viewport with browser controls showing, we add the browser
        // controls height, compensating for page scale as well, since we want to
        // use the viewport with browser controls hidden for vh (to match Safari).
        int viewport_width = frame_->GetPage()->GetVisualViewport().Size().Width();
        if (frame_->IsMainFrame() && layout_size.Width() && viewport_width) {
            // TODO(bokan/eirage): BrowserControl height may need to account for the
            // zoom factor when use-zoom-for-dsf is enabled on Android. Confirm this
            // works correctly when that's turned on. https://crbug.com/737777.
            float page_scale_at_layout_width = viewport_width / layout_size.Width();
            layout_size.Expand(
                0, browser_controls.TotalHeight() / page_scale_at_layout_width);
        }
    }
#endif

    return layoutSize;
}

bool LocalFrameView::WasViewportResized(void)
{
    ASSERT(m_frame);
    if (LayoutView *layoutView = GetLayoutView())
    {
        return GetLayoutSize() != m_lastViewportSize
            || layoutView->StyleRef().Zoom() != m_lastZoomFactor;
    }
    return false;
}

void LocalFrameView::WillBeRemovedFromFrame(void)
{
#if 0 // BKTODO: Check the logic
    if (paint_artifact_compositor_)
        paint_artifact_compositor_->WillBeRemovedFromFrame();
#endif
}

} // namespace blink
