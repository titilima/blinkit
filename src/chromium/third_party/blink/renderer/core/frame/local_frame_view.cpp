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
#include "third_party/blink/renderer/core/editing/drag_caret.h"
#include "third_party/blink/renderer/core/editing/frame_selection.h"
#include "third_party/blink/renderer/core/frame/browser_controls.h"
#include "third_party/blink/renderer/core/frame/link_highlights.h"
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
#include "third_party/blink/renderer/core/paint/frame_painter.h"
#include "third_party/blink/renderer/core/paint/paint_layer_scrollable_area.h"
#include "third_party/blink/renderer/core/paint/pre_paint_tree_walk.h"
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

void LocalFrameView::DidFinishForcedLayout(void)
{
    ASSERT(false); // BKTODO:
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

void LocalFrameView::FlushAnyPendingPostLayoutTasks(void)
{
    ASSERT(false); // BKTODO:
}

// Call function for each non-throttled frame view in pre tree order.
template <typename Function>
void LocalFrameView::ForAllNonThrottledLocalFrameViews(const Function &function)
{
    if (ShouldThrottleRendering())
        return;

    function(*this);

#if 0 // BKTODO: Check if necessary.
    for (Frame* child = frame_->Tree().FirstChild(); child;
        child = child->Tree().NextSibling()) {
        if (!child->IsLocalFrame())
            continue;
        if (LocalFrameView* child_view = ToLocalFrame(child)->View())
            child_view->ForAllNonThrottledLocalFrameViews(function);
    }
#endif
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

ChromeClient* LocalFrameView::GetChromeClient(void) const
{
    if (Page *page = GetFrame().GetPage())
        return &page->GetChromeClient();
    return nullptr;
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

void LocalFrameView::NotifyFrameRectsChangedIfNeeded(void)
{
    if (m_rootLayerDidScroll)
    {
        m_rootLayerDidScroll = false;
        FrameRectsChanged();
    }
}

void LocalFrameView::NotifyFrameRectsChangedIfNeededRecursive(void)
{
    ForAllNonThrottledLocalFrameViews(
        [](LocalFrameView &frameView)
        {
            frameView.NotifyFrameRectsChangedIfNeeded();
        }
    );
}

void LocalFrameView::NotifyPageThatContentAreaWillPaint(void) const
{
    Page *page = m_frame->GetPage();
    if (nullptr == page)
        return;

    if (nullptr == m_scrollableAreas)
        return;

    for (const auto &scrollable_area : *m_scrollableAreas)
    {
        if (!scrollable_area->ScrollbarsCanBeActive())
            continue;

        scrollable_area->ContentAreaWillPaint();
    }
}

void LocalFrameView::NotifyResizeObservers(void)
{
  // Controller exists only if ResizeObserver was created.
    if (!GetFrame().GetDocument()->GetResizeObserverController())
        return;

    ASSERT(false); // BKTODO:
#if 0
    ResizeObserverController& resize_controller =
        frame_->GetDocument()->EnsureResizeObserverController();

    DCHECK(Lifecycle().GetState() >= DocumentLifecycle::kLayoutClean);

    size_t min_depth = 0;
    for (min_depth = resize_controller.GatherObservations(0);
        min_depth != ResizeObserverController::kDepthBottom;
        min_depth = resize_controller.GatherObservations(min_depth)) {
        resize_controller.DeliverObservations();
        GetFrame().GetDocument()->UpdateStyleAndLayout();
    }

    if (resize_controller.SkippedObservations()) {
        resize_controller.ClearObservations();
        ErrorEvent* error = ErrorEvent::Create(
            "ResizeObserver loop limit exceeded",
            SourceLocation::Capture(frame_->GetDocument()), nullptr);
        // We're using |kSharableCrossOrigin| as the error is made by blink itself.
        // TODO(yhirano): Reconsider this.
        frame_->GetDocument()->DispatchErrorEvent(error, kSharableCrossOrigin);
        // Ensure notifications will get delivered in next cycle.
        ScheduleAnimation();
    }
#endif

    ASSERT(!GetLayoutView()->NeedsLayout());
}

void LocalFrameView::PaintTree(void)
{
    LayoutView *layoutView = GetLayoutView();
    ASSERT(nullptr != layoutView);

    ForAllNonThrottledLocalFrameViews(
        [](LocalFrameView &frameView)
        {
            frameView.Lifecycle().AdvanceTo(DocumentLifecycle::kInPaint);
        }
    );

    if (RuntimeEnabledFeatures::SlimmingPaintV2Enabled())
    {
        ASSERT(false); // BKTODO:
#if 0
        if (!paint_controller_)
            paint_controller_ = PaintController::Create();

        if (GetLayoutView()->Layer()->NeedsRepaint()) {
            GraphicsContext graphics_context(*paint_controller_);
            if (RuntimeEnabledFeatures::PrintBrowserEnabled())
                graphics_context.SetPrinting(true);

            if (Settings* settings = frame_->GetSettings()) {
                HighContrastSettings high_contrast_settings;
                high_contrast_settings.mode = settings->GetHighContrastMode();
                high_contrast_settings.grayscale = settings->GetHighContrastGrayscale();
                high_contrast_settings.contrast = settings->GetHighContrastContrast();
                high_contrast_settings.image_policy =
                    settings->GetHighContrastImagePolicy();
                graphics_context.SetHighContrast(high_contrast_settings);
            }

            PaintInternal(graphics_context, kGlobalPaintNormalPhase,
                CullRect(LayoutRect::InfiniteIntRect()));
            paint_controller_->CommitNewDisplayItems();
        }
#endif
    }
    else
    {
        // A null graphics layer can occur for painting of SVG images that are not
        // parented into the main frame tree, or when the LocalFrameView is the main
        // frame view of a page overlay. The page overlay is in the layer tree of
        // the host page and will be painted during painting of the host page.
        if (GraphicsLayer *rootGraphicsLayer = layoutView->Compositor()->PaintRootGraphicsLayer())
            ASSERT(false); // BKTODO: PaintGraphicsLayerRecursively(rootGraphicsLayer);

        // TODO(sataya.m):Main frame doesn't create RootFrameViewport in some
        // webkit_unit_tests (http://crbug.com/644788).
        if (m_viewportScrollableArea)
        {
            if (GraphicsLayer *layerForHorizontalScrollbar = m_viewportScrollableArea->LayerForHorizontalScrollbar())
                ASSERT(false); // BKTODO: PaintGraphicsLayerRecursively(layer_for_horizontal_scrollbar);
            if (GraphicsLayer *layerForVerticalScrollbar = m_viewportScrollableArea->LayerForVerticalScrollbar())
                ASSERT(false); // BKTODO: PaintGraphicsLayerRecursively(layer_for_vertical_scrollbar);
            if (GraphicsLayer *layerForScrollCorner = m_viewportScrollableArea->LayerForScrollCorner())
                ASSERT(false); // BKTODO: PaintGraphicsLayerRecursively(layer_for_scroll_corner);
        }
    }

    Page *page = m_frame->GetPage();
    // TODO(chrishtr): Link highlights don't currently paint themselves,
    // it's still driven by cc. Fix this.
    // This uses an invalidation approach based on graphics layer raster
    // invalidation so it must be after paint. This adds/removes link highlight
    // layers so it must be before |CollectDrawableLayersForLayerListRecursively|.
    page->GetLinkHighlights().UpdateGeometry();

#if 0 // BKTODO: Check the logic
    page->GetValidationMessageClient().PaintOverlay();
    page->PaintPageColorOverlay();
#endif

    ForAllNonThrottledLocalFrameViews(
        [](LocalFrameView &frameView)
        {
            frameView.Lifecycle().AdvanceTo(DocumentLifecycle::kPaintClean);
            if (LayoutView *layoutView = frameView.GetLayoutView())
                layoutView->Layer()->ClearNeedsRepaintRecursively();
        }
    );

#if 0 // BKTODO: Check if necessary.
    // Devtools overlays query the inspected page's paint data so this update
    // needs to be after the lifecycle advance to kPaintClean. Because devtools
    // overlays can add layers, this needs to be before layers are collected.
    if (auto* web_local_frame_impl = WebLocalFrameImpl::FromFrame(frame_))
        web_local_frame_impl->UpdateDevToolsOverlays();
#endif

    if (RuntimeEnabledFeatures::BlinkGenPropertyTreesEnabled())
    {
        ASSERT(false); // BKTODO:
#if 0
        // BlinkGenPropertyTrees just needs a transient PaintController to
        // collect the foreign layers which doesn't need caching. It also
        // shouldn't affect caching status of DisplayItemClients because it's
        // FinishCycle() is not synchronized with other PaintControllers.
        paint_controller_ = PaintController::Create(PaintController::kTransient);

        GraphicsContext context(*paint_controller_);
        // Note: Some blink unit tests run without turning on compositing which
        // means we don't create viewport layers. OOPIFs also don't have their own
        // viewport layers.
        if (GetLayoutView()->Compositor()->InCompositingMode() &&
            GetFrame() == GetPage()->MainFrame()) {
            // TODO(bokan): We should eventually stop creating layers for the visual
            // viewport. At that point, we can remove this. However, for now, CC
            // still has some dependencies on the viewport scale and scroll layers.
            CollectViewportLayersForLayerList(context,
                frame_->GetPage()->GetVisualViewport());
        }
        // With BlinkGenPropertyTrees, |PaintRootGraphicsLayer| is the ancestor of
        // all drawable layers (see: PaintLayerCompositor::PaintRootGraphicsLayer)
        // so we do not need to collect scrollbars separately.
        auto* root = layout_view->Compositor()->PaintRootGraphicsLayer();
        CollectDrawableLayersForLayerListRecursively(context, root);
        // Link highlights paint after all other layers.
        if (!frame_->GetPage()->GetLinkHighlights().IsEmpty())
            CollectLinkHighlightLayersForLayerListRecursively(context, root);
        paint_controller_->CommitNewDisplayItems();
#endif
    }
}

void LocalFrameView::PaintWithLifecycleUpdate(
    GraphicsContext &context,
    const GlobalPaintFlags globalPaintFlags,
    const CullRect &cullRect)
{
    ForAllNonThrottledLocalFrameViews(
        [](LocalFrameView &frameView)
        {
            frameView.Lifecycle().AdvanceTo(DocumentLifecycle::kInPaint);
        }
    );

    FramePainter(*this).Paint(context, globalPaintFlags, cullRect);

    ForAllNonThrottledLocalFrameViews(
        [](LocalFrameView &frameView)
        {
            frameView.Lifecycle().AdvanceTo(DocumentLifecycle::kPaintClean);
        }
    );
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

void LocalFrameView::PerformScrollAnchoringAdjustments(void)
{
    // Adjust() will cause a scroll which could end up causing a layout and
    // reentering this method. Copy and clear the queue so we don't modify it
    // during iteration.
    AnchoringAdjustmentQueue queueCopy = m_anchoringAdjustmentQueue;
    m_anchoringAdjustmentQueue.clear();
    for (WeakMember<ScrollableArea> &scroller : queueCopy)
    {
        if (scroller)
        {
            ASSERT(scroller->GetScrollAnchor());
            scroller->GetScrollAnchor()->Adjust();
        }
    }
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

bool LocalFrameView::RunCompositingLifecyclePhase(DocumentLifecycle::LifecycleState targetState)
{
    LayoutView *layoutView = GetLayoutView();
    ASSERT(nullptr != layoutView);

    if (!RuntimeEnabledFeatures::SlimmingPaintV2Enabled())
    {
        layoutView->Compositor()->UpdateIfNeededRecursive(targetState);
    }
    else
    {
        ForAllNonThrottledLocalFrameViews([](
            LocalFrameView &frameView)
            {
                frameView.GetLayoutView()->Layer()->UpdateDescendantDependentFlags();
                frameView.GetLayoutView()->CommitPendingSelection();
            }
        );
    }

    // We may be in kCompositingInputsClean clean, which does not need to notify
    // the global root scroller controller.
    if (targetState >= DocumentLifecycle::kCompositingClean)
        m_frame->GetPage()->GlobalRootScrollerController().DidUpdateCompositing(*this);

    // We need to run more phases only if the target is beyond kCompositingClean.
    if (targetState > DocumentLifecycle::kCompositingClean)
    {
        // TODO(vmpstr): Why is composited selection only updated if we're moving
        // past kCompositingClean?
        UpdateCompositedSelectionIfNeeded();
        return true;
    }
    return false;
}

void LocalFrameView::RunPaintLifecyclePhase(void)
{
    PaintTree();

    if (!RuntimeEnabledFeatures::SlimmingPaintV2Enabled())
    {
        if (GetLayoutView()->Compositor()->InCompositingMode())
            ASSERT(false); // BKTODO: GetScrollingCoordinator()->UpdateAfterPaint(this);
    }

    if (RuntimeEnabledFeatures::SlimmingPaintV2Enabled() || RuntimeEnabledFeatures::BlinkGenPropertyTreesEnabled())
    {
        ASSERT(false); // BKTODO:
#if 0
        if (!print_mode_enabled)
        {
            base::Optional<CompositorElementIdSet> composited_element_ids =
                CompositorElementIdSet();
            PushPaintArtifactToCompositor(composited_element_ids.value());
            // TODO(wkorman): Add call to UpdateCompositorScrollAnimations here.
            ForAllNonThrottledLocalFrameViews(
                [&composited_element_ids](LocalFrameView& frame_view) {
                    DocumentAnimations::UpdateAnimations(
                        frame_view.GetLayoutView()->GetDocument(),
                        DocumentLifecycle::kPaintClean, composited_element_ids);
                });

            // Notify the controller that the artifact has been pushed and some
            // lifecycle state can be freed (such as raster invalidations).
            paint_controller_->FinishCycle();
            // PaintController for BlinkGenPropertyTrees is transient.
            if (RuntimeEnabledFeatures::BlinkGenPropertyTreesEnabled())
                paint_controller_ = nullptr;
        }
#endif
    }
}

bool LocalFrameView::RunPrePaintLifecyclePhase(DocumentLifecycle::LifecycleState targetState)
{
    ForAllNonThrottledLocalFrameViews(
        [](LocalFrameView &frameView)
        {
            frameView.Lifecycle().AdvanceTo(DocumentLifecycle::kInPrePaint);
            if (frameView.CanThrottleRendering())
            {
                ASSERT(false); // BKTODO:
#if 0
                // This frame can be throttled but not throttled, meaning we are not in an
                // AllowThrottlingScope. Now this frame may contain dirty paint flags, and
                // we need to propagate the flags into the ancestor chain so that
                // PrePaintTreeWalk can reach this frame.
                frameView.SetNeedsPaintPropertyUpdate();
                if (auto *owner = frameView.GetFrame().OwnerLayoutObject())
                    owner->SetShouldCheckForPaintInvalidation();
#endif
            }
        }
    );

#if 0 // BKTODO: Check the logic later.
    {
        SCOPED_UMA_AND_UKM_TIMER(LocalFrameUkmAggregator::kPrePaint);

        PrePaintTreeWalk().WalkTree(*this);
    }
#else
    PrePaintTreeWalk().WalkTree(*this);
#endif

    ForAllNonThrottledLocalFrameViews(
        [](LocalFrameView &frameView)
        {
            frameView.Lifecycle().AdvanceTo(DocumentLifecycle::kPrePaintClean);
        }
    );

    return targetState > DocumentLifecycle::kPrePaintClean;
}

bool LocalFrameView::RunStyleAndLayoutLifecyclePhases(DocumentLifecycle::LifecycleState targetState)
{
    UpdateStyleAndLayoutIfNeededRecursive();
    ASSERT(Lifecycle().GetState() >= DocumentLifecycle::kLayoutClean);

    m_frame->GetDocument()->GetRootScrollerController().PerformRootScrollerSelection();

    // PerformRootScrollerSelection can dirty layout if an effective root
    // scroller is changed so make sure we get back to LayoutClean.
    if (RuntimeEnabledFeatures::ImplicitRootScrollerEnabled() || RuntimeEnabledFeatures::SetRootScrollerEnabled())
    {
        auto callback = [](LocalFrameView &frameView)
        {
            if (frameView.NeedsLayout())
                frameView.UpdateLayout();

            ASSERT(frameView.Lifecycle().GetState() >= DocumentLifecycle::kLayoutClean);
        };
        ForAllNonThrottledLocalFrameViews(callback);
    }

    if (DocumentLifecycle::kLayoutClean == targetState)
        return false;

    // This will be reset by AutoReset in the calling function
    // (UpdateLifecyclePhases()).
    m_pastLayoutLifecycleUpdate = true;

    // After layout and the |past_layout_lifecycle_update_| update, the value of
    // ShouldThrottleRendering() can change. OOPIF local frame roots that are
    // throttled can return now that layout is clean. This situation happens if
    // the throttling was disabled due to required intersection observation, which
    // can now be run.
    if (ShouldThrottleRendering())
        return false;

    // Now we can run post layout steps in preparation for further phases.
    ForAllNonThrottledLocalFrameViews(
        [](LocalFrameView &frameView)
        {
            frameView.PerformScrollAnchoringAdjustments();
        }
    );

#if 0 // BKTODO: Check the logic below
    m_frame->GetPage()->GetValidationMessageClient().LayoutOverlay();
    m_frame->GetPage()->UpdatePageColorOverlay();
#endif

    if (DocumentLifecycle::kPaintClean == targetState)
    {
        ForAllNonThrottledLocalFrameViews(
            [](LocalFrameView &frameView)
            {
                frameView.NotifyResizeObservers();
            }
        );
        NotifyFrameRectsChangedIfNeededRecursive();
    }
    // If we exceed the number of re-layouts during ResizeObserver notifications,
    // then we shouldn't continue with the lifecycle updates. At that time, we
    // have scheduled an animation and we'll try again.
    ASSERT(Lifecycle().GetState() >= DocumentLifecycle::kLayoutClean
        || Lifecycle().GetState() == DocumentLifecycle::kVisualUpdatePending);
    return Lifecycle().GetState() >= DocumentLifecycle::kLayoutClean;
}

void LocalFrameView::ScheduleAnimation(void)
{
    if (auto* client = GetChromeClient())
        client->ScheduleAnimation(this);
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

void LocalFrameView::SetIntersectionObservationState(IntersectionObservationState state)
{
    if (m_intersectionObservationState >= state)
        return;
    m_intersectionObservationState = state;
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

void LocalFrameView::UpdateAllLifecyclePhases(void)
{
    UpdateLifecyclePhases(DocumentLifecycle::kPaintClean);
}

bool LocalFrameView::UpdateAllLifecyclePhasesExceptPaint(void)
{
    return UpdateLifecyclePhases(DocumentLifecycle::kPrePaintClean);
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

void LocalFrameView::UpdateCompositedSelectionIfNeeded(void)
{
    if (!RuntimeEnabledFeatures::CompositedSelectionUpdateEnabled())
        return;

    ASSERT(false); // BKTODO:
#if 0
    Page* page = GetFrame().GetPage();
    DCHECK(page);

    LocalFrame* focused_frame = page->GetFocusController().FocusedFrame();
    LocalFrame* local_frame =
        (focused_frame &&
            (focused_frame->LocalFrameRoot() == frame_->LocalFrameRoot()))
        ? focused_frame
        : nullptr;

    if (local_frame) {
        const cc::LayerSelection& selection = ComputeLayerSelection(*local_frame);
        if (selection != cc::LayerSelection()) {
            page->GetChromeClient().UpdateLayerSelection(local_frame, selection);
            return;
        }
    }

    if (!local_frame) {
        // Clearing the mainframe when there is no focused frame (and hence
        // no localFrame) is legacy behaviour, and implemented here to
        // satisfy WebFrameTest.CompositedSelectionBoundsCleared's
        // first check that the composited selection has been cleared even
        // though no frame has focus yet. If this is not desired, then the
        // expectation needs to be removed from the test.
        local_frame = &frame_->LocalFrameRoot();
    }
    DCHECK(local_frame);
    page->GetChromeClient().ClearLayerSelection(local_frame);
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

// TODO(leviw): We don't assert lifecycle information from documents in child
// WebPluginContainerImpls.
bool LocalFrameView::UpdateLifecyclePhases(DocumentLifecycle::LifecycleState targetState)
{
    // If the lifecycle is postponed, which can happen if the inspector requests
    // it, then we shouldn't update any lifecycle phases.
    Document *document = m_frame->GetDocument();
    if (nullptr != document && document->Lifecycle().LifecyclePostponed())
        return false;

    // Prevent reentrance.
    // TODO(vmpstr): Should we just have a DCHECK instead here?
    if (m_currentUpdateLifecyclePhasesTargetState != DocumentLifecycle::kUninitialized)
    {
        NOTREACHED(); // reentrance!
        return false;
    }

    // Only the following target states are supported.
    ASSERT(DocumentLifecycle::kLayoutClean == targetState
        || DocumentLifecycle::kCompositingInputsClean == targetState
        || DocumentLifecycle::kCompositingClean == targetState
        || DocumentLifecycle::kPrePaintClean == targetState
        || DocumentLifecycle::kPaintClean == targetState);

    // If the document is not active then it is either not yet initialized, or it
    // is stopping. In either case, we can't reach one of the supported target
    // states.
    if (!document->IsActive())
        return false;

    // This is used to guard against reentrance. It is also used in conjunction
    // with the current lifecycle state to determine which phases are yet to run
    // in this cycle.
    base::AutoReset<DocumentLifecycle::LifecycleState> targetStateScope(&m_currentUpdateLifecyclePhasesTargetState, targetState);
    // This is used to check if we're within a lifecycle update but have passed
    // the layout update phase. Note there is a bit of a subtlety here: it's not
    // sufficient for us to check the current lifecycle state, since it can be
    // past kLayoutClean but the function to run style and layout phase has not
    // actually been run yet. Since this bool affects throttling, and throttling,
    // in turn, determines whether style and layout function will run, we need a
    // separate bool.
    base::AutoReset<bool> pastLayoutLifecycleResetter(&m_pastLayoutLifecycleUpdate, false);

    // If we're throttling, then we don't need to update lifecycle phases, only
    // the throttling status.
    if (ShouldThrottleRendering())
    {
        ASSERT(false); // BKTODO: UpdateThrottlingStatusForSubtree();
        return Lifecycle().GetState() == targetState;
    }

    // Run the lifecycle updates.
    UpdateLifecyclePhasesInternal(targetState);

    // Update intersection observations if needed.
    if (DocumentLifecycle::kPaintClean == targetState)
        UpdateViewportIntersectionsForSubtree();

    UpdateThrottlingStatusForSubtree();

    return Lifecycle().GetState() == targetState;
}

void LocalFrameView::UpdateLifecyclePhasesInternal(DocumentLifecycle::LifecycleState targetState)
{
    bool runMoreLifecyclePhases = RunStyleAndLayoutLifecyclePhases(targetState);
    if (!runMoreLifecyclePhases)
        return;

    ASSERT(Lifecycle().GetState() >= DocumentLifecycle::kLayoutClean);

    if (nullptr == GetLayoutView())
        return;

#if DCHECK_IS_ON()
    DisallowLayoutInvalidationScope disallowLayoutInvalidation(*this);
#endif
    {
        runMoreLifecyclePhases = RunCompositingLifecyclePhase(targetState);
        if (!runMoreLifecyclePhases)
            return;

        // TODO(pdr): PrePaint should be under the "Paint" devtools timeline
        // step for slimming paint v2.
        runMoreLifecyclePhases = RunPrePaintLifecyclePhase(targetState);
        DCHECK(ShouldThrottleRendering() ||
            Lifecycle().GetState() >= DocumentLifecycle::kPrePaintClean);
        if (!runMoreLifecyclePhases)
            return;
    }

    ASSERT(DocumentLifecycle::kPaintClean == targetState);
    RunPaintLifecyclePhase();
    ASSERT(ShouldThrottleRendering() || Lifecycle().GetState() == DocumentLifecycle::kPaintClean);
}

void LocalFrameView::UpdateRenderThrottlingStatus(
    bool hidden,
    bool subtreeThrottled,
    ForceThrottlingInvalidationBehavior forceThrottlingInvalidationHehavior,
    NotifyChildrenBehavior notifyChildrenBehavior)
{
    BKLOG("// BKTODO: Check if necessary.");
}

void LocalFrameView::UpdateStyleAndLayoutIfNeededRecursive(void)
{
    if (ShouldThrottleRendering())
        return;

    Document *document = m_frame->GetDocument();
    if (!document->IsActive())
        return;

    // We have to crawl our entire subtree looking for any FrameViews that need
    // layout and make sure they are up to date.
    // Mac actually tests for intersection with the dirty region and tries not to
    // update layout for frames that are outside the dirty region.  Not only does
    // this seem pointless (since those frames will have set a zero timer to
    // layout anyway), but it is also incorrect, since if two frames overlap, the
    // first could be excluded from the dirty region but then become included
    // later by the second frame adding rects to the dirty region when it lays
    // out.
    document->UpdateStyleAndLayoutTree();

#if 0 // BKTODO: Check if necessary.
    // Update style for all embedded SVG documents underneath this frame, so
    // that intrinsic size computation for any embedded objects has up-to-date
    // information before layout.
    ForAllChildLocalFrameViews([](LocalFrameView& view) {
        Document& document = *view.GetFrame().GetDocument();
        if (document.IsSVGDocument())
            document.UpdateStyleAndLayoutTree();
        });
#endif

    ASSERT(!ShouldThrottleRendering());
    ASSERT(document->IsActive());
    ASSERT(0 == m_nestedLayoutCount);

    if (NeedsLayout())
        UpdateLayout();

    CheckDoesNotNeedLayout();

#if 0 // BKTODO: Check if necessary.
    // WebView plugins need to update regardless of whether the
    // LayoutEmbeddedObject that owns them needed layout.
    // TODO(leviw): This currently runs the entire lifecycle on plugin WebViews.
    // We should have a way to only run these other Documents to the same
    // lifecycle stage as this frame.
    for (const auto& plugin : plugins_) {
        plugin->UpdateAllLifecyclePhases();
    }
    CheckDoesNotNeedLayout();

    // FIXME: Calling layout() shouldn't trigger script execution or have any
    // observable effects on the frame tree but we're not quite there yet.
    HeapVector<Member<LocalFrameView>> frame_views;
    for (Frame* child = frame_->Tree().FirstChild(); child;
        child = child->Tree().NextSibling()) {
        if (!child->IsLocalFrame())
            continue;
        if (LocalFrameView* view = ToLocalFrame(child)->View())
            frame_views.push_back(view);
    }

    for (const auto& frame_view : frame_views)
        frame_view->UpdateStyleAndLayoutIfNeededRecursiveInternal();

    // These asserts ensure that parent frames are clean, when child frames
    // finished updating layout and style.
    CheckDoesNotNeedLayout();
#endif
#if DCHECK_IS_ON()
    document->GetLayoutView()->AssertLaidOut();
#endif

    UpdateGeometriesIfNeeded();

    if (Lifecycle().GetState() < DocumentLifecycle::kLayoutClean)
        Lifecycle().AdvanceTo(DocumentLifecycle::kLayoutClean);

#if 0 // BKTODO: Check if necessary.
    if (AXObjectCache* cache = GetFrame().GetDocument()->ExistingAXObjectCache())
        cache->ProcessUpdatesAfterLayout(*GetFrame().GetDocument());
#endif

    // Ensure that we become visually non-empty eventually.
    // TODO(esprehn): This should check isRenderingReady() instead.
    if (GetFrame().GetDocument()->HasFinishedParsing() &&
        GetFrame().Loader().StateMachine()->CommittedFirstRealDocumentLoad())
    {
        m_isVisuallyNonEmpty = true;
    }

    GetFrame().Selection().UpdateStyleAndLayoutIfNeeded();
    GetFrame().GetPage()->GetDragCaret().UpdateStyleAndLayoutIfNeeded();
}

void LocalFrameView::UpdateThrottlingStatusForSubtree(void)
{
#if 0 // BKTODO: Check if necessary.
    if (!GetFrame().GetDocument()->IsActive())
        return;

    // Don't throttle display:none frames (see updateRenderThrottlingStatus).
    HTMLFrameOwnerElement* owner_element = frame_->DeprecatedLocalOwner();
    if (hidden_for_throttling_ && owner_element &&
        !owner_element->GetLayoutObject()) {
        // No need to notify children because descendants of display:none frames
        // should remain throttled.
        UpdateRenderThrottlingStatus(hidden_for_throttling_, subtree_throttled_,
            kDontForceThrottlingInvalidation,
            kDontNotifyChildren);
    }

    ForAllChildLocalFrameViews([](LocalFrameView& child_view) {
        child_view.UpdateThrottlingStatusForSubtree();
        });
#endif
}

void LocalFrameView::UpdateViewportIntersectionsForSubtree(void)
{
    Document *document = GetFrame().GetDocument();
    // TODO(dcheng): Since LocalFrameView tree updates are deferred, FrameViews
    // might still be in the LocalFrameView hierarchy even though the associated
    // Document is already detached. Investigate if this check and a similar check
    // in lifecycle updates are still needed when there are no more deferred
    // LocalFrameView updates: https://crbug.com/561683
    if (!document->IsActive())
        return;

#if 0 // BKTODO: Check if necessary.
    RecordDeferredLoadingStats();
#endif
    if (!NeedsLayout())
    {
        // Notify javascript IntersectionObservers
        if (IntersectionObserverController *intersectionObserverController = document->GetIntersectionObserverController())
            ASSERT(false); // BKTODO: intersectionObserverController->ComputeTrackedIntersectionObservations();
    }

#if 0 // BKTODO: Check if necessary.
    for (Frame* child = frame_->Tree().FirstChild(); child;
        child = child->Tree().NextSibling())
    {
        child->View()->UpdateViewportIntersectionsForSubtree();
    }
#endif

    m_intersectionObservationState = kNotNeeded;
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
        int viewportWidth = m_frame->GetPage()->GetVisualViewport().Size().Width();
        if (0 != layoutSize.Width() && 0 != viewportWidth)
        {
            // TODO(bokan/eirage): BrowserControl height may need to account for the
            // zoom factor when use-zoom-for-dsf is enabled on Android. Confirm this
            // works correctly when that's turned on. https://crbug.com/737777.
            float pageScaleAtLayoutWidth = viewportWidth / layoutSize.Width();
            layoutSize.Expand(0, browserControls.TotalHeight() / pageScaleAtLayoutWidth);
        }
    }

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

void LocalFrameView::WillStartForcedLayout(void)
{
    ASSERT(false); // BKTODO:
}

} // namespace blink
