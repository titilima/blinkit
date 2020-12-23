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
#include "third_party/blink/renderer/core/frame/layout_subtree_root_list.h"
#include "third_party/blink/renderer/core/layout/depth_ordered_layout_object_list.h"
#include "third_party/blink/renderer/core/paint/compositing/paint_layer_compositor.h"
#include "third_party/blink/renderer/core/paint/paint_phase.h"
#include "third_party/blink/renderer/platform/geometry/int_rect.h"
#include "third_party/blink/renderer/platform/geometry/layout_size.h"
#include "third_party/blink/renderer/platform/graphics/color.h"

namespace blink {

class ChromeClient;
class CullRect;
class DocumentLifecycle;
class FrameViewAutoSizeInfo;
class GraphicsContext;
class LayoutBox;
class LayoutView;
class PaintLayerScrollableArea;
class RootFrameViewport;
class ScrollableArea;
class ScrollingCoordinator;

class LocalFrameView final : public FrameView
{
public:
    static LocalFrameView* Create(LocalFrame &frame, const IntSize &initialSize);
    void Trace(Visitor *visitor) override;
    void Dispose(void);

    LocalFrame& GetFrame(void) const
    {
        ASSERT(m_frame);
        return *m_frame;
    }
    Page* GetPage(void) const;
    LayoutView* GetLayoutView(void) const;
    // The window that hosts the LocalFrameView. The LocalFrameView will
    // communicate scrolls and repaints to the host window in the window's
    // coordinate space.
    ChromeClient* GetChromeClient(void) const;
    IntRect FrameRect(void) const { return IntRect(Location(), Size()); }
    void SetFrameRect(const IntRect &unclampedFrameRect);
    IntPoint Location(void) const;
    int X(void) const { return Location().X(); }
    int Y(void) const { return Location().Y(); }
    int Width(void) const { return Size().Width(); }
    int Height(void) const { return Size().Height(); }
    IntSize Size(void) const { return m_frameRect.Size(); }
    void Resize(const IntSize &size) { SetFrameRect(IntRect(m_frameRect.Location(), size)); }

    void ScheduleAnimation(void);

    // Run all needed lifecycle stages. After calling this method, all frames will
    // be in the lifecycle state PaintClean.  If lifecycle throttling is allowed
    // (see DocumentLifecycle::AllowThrottlingScope), some frames may skip the
    // lifecycle update (e.g., based on visibility) and will not end up being
    // PaintClean.
    void UpdateAllLifecyclePhases(void);

    // Computes the style, layout, compositing and pre-paint lifecycle stages
    // if needed.
    // After calling this method, all frames will be in a lifecycle
    // state >= PrePaintClean, unless the frame was throttled or inactive.
    // Returns whether the lifecycle was successfully updated to the
    // desired state.
    bool UpdateAllLifecyclePhasesExceptPaint(void);

    void AdjustViewSize(void);
    void SetLayoutOverflowSize(const IntSize &size);
    void UpdateLayout(void);

    // Paints, and also updates the lifecycle to in-paint and paint clean
    // beforehand.  Call this for painting use-cases outside of the lifecycle.
    void PaintWithLifecycleUpdate(GraphicsContext &context, const GlobalPaintFlags globalPaintFlags,
        const CullRect &cullRect);

    // Returns the scrollable area for the frame. For the root frame, this will
    // be the RootFrameViewport, which adds pinch-zoom semantics to scrolling.
    // For non-root frames, this will be the ScrollableArea of the LayoutView.
    ScrollableArea* GetScrollableArea(void);

    void AddScrollbar(Scrollbar *scrollbar);
    void RemoveScrollbar(Scrollbar *scrollbar);

    bool IsAttached(void) const { return m_isAttached; }
    bool IsSelfVisible(void) const { return m_selfVisible; }
    bool IsVisible(void) const { return IsSelfVisible(); }
    void SetSelfVisible(bool visible);
    bool CanHaveScrollbars(void) const { return m_canHaveScrollbars; }

    bool LayoutPending(void) const;
    bool IsInPerformLayout(void) const;
    bool NeedsLayout(void) const;
    bool CheckDoesNotNeedLayout(void) const;
    void SetNeedsLayout(void);
    bool DidFirstLayout(void) const { return !m_firstLayout; }

    void ResetNeedsForcedCompositingUpdate(void) { m_needsForcedCompositingUpdate = false; }

    LayoutRect FrameToDocument(const LayoutRect &rectInFrame) const;

    void ScheduleVisualUpdateForPaintInvalidationIfNeeded(void);
    void ScheduleRelayout(void);

    // FIXME: This should probably be renamed as the 'inSubtreeLayout' parameter
    // passed around the LocalFrameView layout methods can be true while this
    // returns false.
    bool IsSubtreeLayout(void) const { return !m_layoutSubtreeRootList.IsEmpty(); }
    void ClearLayoutSubtreeRoot(const LayoutObject &root);

    bool HasOrthogonalWritingModeRoots(void) const { return !m_orthogonalWritingModeRootList.IsEmpty(); }

    bool LayoutSizeFixedToFrameSize(void) { return m_layoutSizeFixedToFrameSize; }
    // If this is set to false, the layout size will need to be explicitly set by
    // the owner.  E.g. WebViewImpl sets its mainFrame's layout size manually
    void SetLayoutSizeFixedToFrameSize(bool isFixed);
    void SetNeedsUpdateGeometries(void) { m_needsUpdateGeometries = true; }

    // Marks this frame, and ancestor frames, as needing one intersection
    // observervation. This overrides throttling for one frame, up to
    // kLayoutClean. The order of these enums is important - they must proceed
    // from "least required to most required".
    enum IntersectionObservationState {
        // The next painting frame does not need an intersection observation.
        kNotNeeded = 0,
        // The next painting frame needs an intersection observation.
        kDesired = 1,
        // The next painting frame must be generated up to intersection observation
        // (even if frame is throttled).
        kRequired = 2
    };
    // Sets the internal IntersectionObservationState to the max of the
    // current value and the provided one.
    void SetIntersectionObservationState(IntersectionObservationState);

    IntSize GetLayoutSize(void) const { return m_layoutSize; }
    void SetLayoutSize(const IntSize &size);

    void SetInitialViewportSize(const IntSize &viewportSize);

    // Viewport size that should be used for viewport units (i.e. 'vh'/'vw').
    // May include the size of browser controls. See implementation for further
    // documentation.
    FloatSize ViewportSizeForViewportUnits(void) const;

    Color BaseBackgroundColor(void) const { return m_baseBackgroundColor; }
    void SetBaseBackgroundColor(const Color &backgroundColor);
    void UpdateBaseBackgroundColorRecursively(const Color &baseBackgroundColor);

    // Returns the ScrollableArea of the LayoutView, a.k.a. the layout viewport.
    // In the root frame, this is the "outer" viewport in the pinch-zoom dual
    // viewport model.  Callers that need awareness of both inner and outer
    // viewports should use GetScrollableArea() instead.
    PaintLayerScrollableArea* LayoutViewport(void) const;

    // If this is the main frame, this will return the RootFrameViewport used
    // to scroll the main frame. Otherwise returns nullptr. Unless you need a
    // unique method on RootFrameViewport, you should probably use
    // getScrollableArea.
    RootFrameViewport* GetRootFrameViewport(void) { return m_viewportScrollableArea.Get(); }

    using ScrollableAreaSet = std::unordered_set<PaintLayerScrollableArea *>;
    void AddScrollableArea(PaintLayerScrollableArea *scrollableArea);
    void RemoveScrollableArea(PaintLayerScrollableArea *scrollableArea);
    const ScrollableAreaSet* ScrollableAreas(void) const { return m_scrollableAreas.get(); }

    void AddAnimatingScrollableArea(PaintLayerScrollableArea *scrollableArea);
    void RemoveAnimatingScrollableArea(PaintLayerScrollableArea *scrollableArea);
    const ScrollableAreaSet* AnimatingScrollableAreas(void) const { return m_animatingScrollableAreas.get(); }

    // With CSS style "resize:" enabled, a little resizer handle will appear at
    // the bottom right of the object. We keep track of these resizer areas for
    // checking if touches (implemented using Scroll gesture) are targeting the
    // resizer.
    typedef std::unordered_set<LayoutBox *> ResizerAreaSet;
    void AddResizerArea(LayoutBox &resizerBox);
    void RemoveResizerArea(LayoutBox &resizerBox);
    const ResizerAreaSet* ResizerAreas(void) const { return m_resizerAreas.get(); }

    void EnqueueScrollAnchoringAdjustment(ScrollableArea *scrollableArea);
    void DequeueScrollAnchoringAdjustment(ScrollableArea *scrollableArea);
    void PerformScrollAnchoringAdjustments(void);

    // Fixed-position objects.
    typedef std::unordered_set<LayoutObject *> ViewportConstrainedObjectSet;
    void AddViewportConstrainedObject(LayoutObject &object);
    void RemoveViewportConstrainedObject(LayoutObject &object);
    bool HasViewportConstrainedObjects(void) const { return m_viewportConstrainedObjects && !m_viewportConstrainedObjects->empty(); }
    const ViewportConstrainedObjectSet* ViewportConstrainedObjects(void) const { return m_viewportConstrainedObjects.get(); }
    void MarkViewportConstrainedObjectsForLayout(bool widthChanged, bool heightChanged);

    // Returns true if this frame should not render or schedule visual updates.
    bool ShouldThrottleRendering(void) const;
    // Returns true if this frame could potentially skip rendering and avoid
    // scheduling visual updates.
    bool CanThrottleRendering(void) const;
    void SetupRenderThrottling(void);

    enum ForceThrottlingInvalidationBehavior {
        kDontForceThrottlingInvalidation,
        kForceThrottlingInvalidation
    };
    enum NotifyChildrenBehavior { kDontNotifyChildren, kNotifyChildren };
    void UpdateRenderThrottlingStatus(bool hidden, bool subtreeThrottled,
        ForceThrottlingInvalidationBehavior forceThrottlingInvalidationBehavior = kDontForceThrottlingInvalidation,
        NotifyChildrenBehavior notifyChildrenBehavior = kNotifyChildren);

    // FIXME: Remove this method once plugin loading is decoupled from layout.
    void FlushAnyPendingPostLayoutTasks(void);

    void UpdateDocumentAnnotatedRegions(void) const;

    void NotifyPageThatContentAreaWillPaint(void) const;
    void NotifyResizeObservers(void);

    void DidAttachDocument(void);
    void HandleLoadCompleted(void);
    // Called when this view is going to be removed from its owning
    // LocalFrame.
    void WillBeRemovedFromFrame(void);

    // Methods to capture forced layout metrics.
    void WillStartForcedLayout(void);
    void DidFinishForcedLayout(void);

    // Called when our frame rect changes (or the rect/scroll offset of an
    // ancestor changes).
    void FrameRectsChanged(void);
    void ViewportSizeChanged(bool widthChanged, bool heightChanged);

    void BeginLifecycleUpdates(void);

    void ClearFragmentAnchor(void);

    void UpdateCountersAfterStyleChange(void);

    void IncrementLayoutObjectCount(void) {} // Just a placeholder
    void IncrementVisuallyNonEmptyCharacterCount(unsigned count);
    bool IsVisuallyNonEmpty(void) const { return m_isVisuallyNonEmpty; }
    void SetIsVisuallyNonEmpty(void) { m_isVisuallyNonEmpty = true; }
protected:
    void NotifyFrameRectsChangedIfNeeded(void);
private:
#if DCHECK_IS_ON()
    class DisallowLayoutInvalidationScope {
    public:
        DisallowLayoutInvalidationScope(LocalFrameView &view) : m_localFrameView(view) {
            m_localFrameView.m_allowsLayoutInvalidationAfterLayoutClean = false;
        }
        ~DisallowLayoutInvalidationScope(void) {
            m_localFrameView.m_allowsLayoutInvalidationAfterLayoutClean = true;
        }
    private:
        LocalFrameView &m_localFrameView;
    };
#endif
    explicit LocalFrameView(LocalFrame &frame, const IntRect &frameRect);

    template <typename Function>
    void ForAllNonThrottledLocalFrameViews(const Function &function);

    void UpdateViewportIntersectionsForSubtree(void);
    void UpdateThrottlingStatusForSubtree();

    // Returns whether the lifecycle was succesfully updated to the
    // target state.
    bool UpdateLifecyclePhases(DocumentLifecycle::LifecycleState targetState);
    // The internal version that does the work after the proper context and checks
    // have passed in the above function call.
    void UpdateLifecyclePhasesInternal(DocumentLifecycle::LifecycleState targetState);
    // Four lifecycle phases helper functions corresponding to StyleAndLayout,
    // Compositing, PrePaint, and Paint phases. If the return value is true, it
    // means further lifecycle phases need to be run. This is used to abort
    // earlier if we don't need to run future lifecycle phases.
    bool RunStyleAndLayoutLifecyclePhases(DocumentLifecycle::LifecycleState targetState);
    bool RunCompositingLifecyclePhase(DocumentLifecycle::LifecycleState targetState);
    bool RunPrePaintLifecyclePhase(DocumentLifecycle::LifecycleState targetState);
    void RunPaintLifecyclePhase(void);
    void UpdateStyleAndLayoutIfNeededRecursive(void);
    void PaintTree(void);

    ScrollingCoordinator* GetScrollingCoordinator(void) const;
    void SetLayoutSizeInternal(const IntSize &size);

    void UpdateCompositedSelectionIfNeeded(void);
    void SetNeedsCompositingUpdate(CompositingUpdateType updateType);

    void ClearLayoutSubtreeRootsAndMarkContainingBlocks(void);

    bool WasViewportResized(void);

    DocumentLifecycle& Lifecycle(void) const;

    void PerformPreLayoutTasks(void);
    void PerformLayout(bool inSubtreeLayout);
    void PerformPostLayoutTasks(void);

    bool CheckLayoutInvalidationIsAllowed(void) const;

    void LayoutFromRootObject(LayoutObject &root);

    void UpdateGeometriesIfNeeded(void);

    void SendResizeEventIfNeeded(void);
    void NotifyFrameRectsChangedIfNeededRecursive(void);

    void Show(void) override;

    LayoutSize m_size;

    Member<LocalFrame> m_frame;
    IntRect m_frameRect;
    bool m_isAttached = false;
    bool m_selfVisible = false;
    bool m_canHaveScrollbars = true;

    bool m_hasPendingLayout = false;
    LayoutSubtreeRootList m_layoutSubtreeRootList;
    DepthOrderedLayoutObjectList m_orthogonalWritingModeRootList;

    bool m_layoutSchedulingEnabled = true;
    unsigned m_nestedLayoutCount = 0;

    Member<Node> m_fragmentAnchor;
    std::unique_ptr<ScrollableAreaSet> m_scrollableAreas;
    std::unique_ptr<ScrollableAreaSet> m_animatingScrollableAreas;
    HeapHashSet<Member<Scrollbar>> m_scrollbars;

    // TODO(bokan): This is unneeded when root-layer-scrolls is turned on.
    // crbug.com/417782.
    IntSize m_layoutOverflowSize;

    bool m_rootLayerDidScroll = false;

    std::unique_ptr<ResizerAreaSet> m_resizerAreas;
    std::unique_ptr<ViewportConstrainedObjectSet> m_viewportConstrainedObjects;

    IntSize m_layoutSize;
    IntSize m_initialViewportSize;

    bool m_needsUpdateGeometries = false;

    bool m_firstLayout = true;
    Color m_baseBackgroundColor;
    IntSize m_lastViewportSize;
    float m_lastZoomFactor = 1.0;

    unsigned m_visuallyNonEmptyCharacterCount = 0;
    bool m_isVisuallyNonEmpty = false;

    Member<FrameViewAutoSizeInfo> m_autoSizeInfo;

    bool m_layoutSizeFixedToFrameSize = true;
    // Exists only on root frame.
    // TODO(bokan): crbug.com/484188. We should specialize LocalFrameView for the
    // main frame.
    Member<RootFrameViewport> m_viewportScrollableArea;

    // BKTODO: Check if necessary.
    // The following members control rendering pipeline throttling for this
    // frame. They are only updated in response to intersection observer
    // notifications, i.e., not in the middle of the lifecycle.
    bool m_hiddenForThrottling = false;
    bool m_subtreeThrottled = false;
    bool m_lifecycleUpdatesThrottled = false;

    DocumentLifecycle::LifecycleState m_currentUpdateLifecyclePhasesTargetState = DocumentLifecycle::kUninitialized;
    bool m_pastLayoutLifecycleUpdate = false;

    using AnchoringAdjustmentQueue = HeapLinkedHashSet<WeakMember<ScrollableArea>>;
    AnchoringAdjustmentQueue m_anchoringAdjustmentQueue;

    bool m_suppressAdjustViewSize = false;
#if DCHECK_IS_ON()
    // In DCHECK on builds, this is set to false when we're running lifecycle
    // phases past layout to ensure that phases after layout don't dirty layout.
    bool m_allowsLayoutInvalidationAfterLayoutClean = true;
#endif
    IntersectionObservationState m_intersectionObservationState = kNotNeeded;
    bool m_needsForcedCompositingUpdate = false;

#if DCHECK_IS_ON()
    // Verified when finalizing.
    bool m_hasBeenDisposed = false;
#endif
};

} // namespace blink

#endif // BLINKIT_BLINK_LOCAL_FRAME_VIEW_H
