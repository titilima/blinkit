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
#include "third_party/blink/renderer/core/paint/compositing/paint_layer_compositor.h"
#include "third_party/blink/renderer/platform/geometry/int_rect.h"
#include "third_party/blink/renderer/platform/geometry/layout_size.h"
#include "third_party/blink/renderer/platform/graphics/color.h"

namespace blink {

class DocumentLifecycle;
class FrameViewAutoSizeInfo;
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
    IntRect FrameRect(void) const { return IntRect(Location(), Size()); }
    IntPoint Location(void) const;
    int X(void) const { return Location().X(); }
    int Y(void) const { return Location().Y(); }
    int Width(void) const { return Size().Width(); }
    int Height(void) const { return Size().Height(); }
    IntSize Size(void) const { return m_frameRect.Size(); }

    void UpdateLayout(void);

    // Returns the scrollable area for the frame. For the root frame, this will
    // be the RootFrameViewport, which adds pinch-zoom semantics to scrolling.
    // For non-root frames, this will be the ScrollableArea of the LayoutView.
    ScrollableArea* GetScrollableArea(void);

    bool IsAttached(void) const { return m_isAttached; }
    bool IsSelfVisible(void) const { return m_selfVisible; }
    void SetSelfVisible(bool visible);
    bool CanHaveScrollbars(void) const { return m_canHaveScrollbars; }

    bool LayoutPending(void) const;
    bool IsInPerformLayout(void) const;
    bool NeedsLayout(void) const;
    bool CheckDoesNotNeedLayout(void) const;
    void SetNeedsLayout(void);
    bool DidFirstLayout(void) const { return !m_firstLayout; }

    LayoutRect FrameToDocument(const LayoutRect &rectInFrame) const;

    void ScheduleVisualUpdateForPaintInvalidationIfNeeded(void);
    void ScheduleRelayout(void);

    // FIXME: This should probably be renamed as the 'inSubtreeLayout' parameter
    // passed around the LocalFrameView layout methods can be true while this
    // returns false.
    bool IsSubtreeLayout(void) const { return !m_layoutSubtreeRootList.IsEmpty(); }
    void ClearLayoutSubtreeRoot(const LayoutObject &root);

    bool LayoutSizeFixedToFrameSize(void) { return m_layoutSizeFixedToFrameSize; }
    // If this is set to false, the layout size will need to be explicitly set by
    // the owner.  E.g. WebViewImpl sets its mainFrame's layout size manually
    void SetLayoutSizeFixedToFrameSize(bool isFixed);
    void SetNeedsUpdateGeometries(void) { m_needsUpdateGeometries = true; }

    IntSize GetLayoutSize(void) const { return m_layoutSize; }
    void SetLayoutSize(const IntSize &size);

    void SetInitialViewportSize(const IntSize &viewportSize);

    // Viewport size that should be used for viewport units (i.e. 'vh'/'vw').
    // May include the size of browser controls. See implementation for further
    // documentation.
    FloatSize ViewportSizeForViewportUnits(void) const;

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
    RootFrameViewport* GetRootFrameViewport(void) { return m_viewportScrollableArea.get(); }

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

    void UpdateDocumentAnnotatedRegions(void) const;

    void DidAttachDocument(void);
    void HandleLoadCompleted(void);
    // Called when this view is going to be removed from its owning
    // LocalFrame.
    void WillBeRemovedFromFrame(void);

    void BeginLifecycleUpdates(void);

    void ClearFragmentAnchor(void);

    void UpdateCountersAfterStyleChange(void);

    void IncrementLayoutObjectCount(void) {} // Just a placeholder
    void IncrementVisuallyNonEmptyCharacterCount(unsigned count);
    bool IsVisuallyNonEmpty(void) const { return m_isVisuallyNonEmpty; }
    void SetIsVisuallyNonEmpty(void) { m_isVisuallyNonEmpty = true; }
private:
    explicit LocalFrameView(LocalFrame &frame, const IntRect &frameRect);

    ScrollingCoordinator* GetScrollingCoordinator(void) const;
    void SetLayoutSizeInternal(const IntSize &size);
    void SetNeedsCompositingUpdate(CompositingUpdateType updateType);

    void ClearLayoutSubtreeRootsAndMarkContainingBlocks(void);

    bool WasViewportResized(void);

    DocumentLifecycle& Lifecycle(void) const;

    void PerformPreLayoutTasks(void);
    void PerformLayout(bool inSubtreeLayout);
    void PerformPostLayoutTasks(void);

    bool CheckLayoutInvalidationIsAllowed(void) const;

    void Show(void) override;

    LayoutSize m_size;

    Member<LocalFrame> m_frame;
    IntRect m_frameRect;
    bool m_isAttached = false;
    bool m_selfVisible = false;
    bool m_canHaveScrollbars = true;

    bool m_hasPendingLayout = false;
    LayoutSubtreeRootList m_layoutSubtreeRootList;

    bool m_layoutSchedulingEnabled = true;
    unsigned m_nestedLayoutCount = 0;

    Member<Node> m_fragmentAnchor;
    std::unique_ptr<ScrollableAreaSet> m_scrollableAreas;
    std::unique_ptr<ScrollableAreaSet> m_animatingScrollableAreas;
    std::unique_ptr<ResizerAreaSet> m_resizerAreas;
    std::unique_ptr<ViewportConstrainedObjectSet> m_viewportConstrainedObjects;
    IntSize m_layoutSize;
    IntSize m_initialViewportSize;

    bool m_firstLayout = true;
    Color m_baseBackgroundColor;
    IntSize m_lastViewportSize;
    float m_lastZoomFactor = 1.0;

    unsigned m_visuallyNonEmptyCharacterCount = 0;
    bool m_isVisuallyNonEmpty = false;

    Member<FrameViewAutoSizeInfo> m_autoSizeInfo;

    bool m_layoutSizeFixedToFrameSize = true;
    bool m_needsUpdateGeometries = false;
    // Exists only on root frame.
    // TODO(bokan): crbug.com/484188. We should specialize LocalFrameView for the
    // main frame.
    std::unique_ptr<RootFrameViewport> m_viewportScrollableArea;

    // BKTODO: Check if necessary.
    // The following members control rendering pipeline throttling for this
    // frame. They are only updated in response to intersection observer
    // notifications, i.e., not in the middle of the lifecycle.
    bool m_hiddenForThrottling = false;
    bool m_subtreeThrottled = false;
    bool m_lifecycleUpdatesThrottled = false;

    DocumentLifecycle::LifecycleState m_currentUpdateLifecyclePhasesTargetState = DocumentLifecycle::kUninitialized;

    bool m_suppressAdjustViewSize = false;
#if DCHECK_IS_ON()
    // In DCHECK on builds, this is set to false when we're running lifecycle
    // phases past layout to ensure that phases after layout don't dirty layout.
    bool m_allowsLayoutInvalidationAfterLayoutClean = true;
#endif
    bool m_needsForcedCompositingUpdate = false;

#if DCHECK_IS_ON()
    // Verified when finalizing.
    bool m_hasBeenDisposed = false;
#endif
};

} // namespace blink

#endif // BLINKIT_BLINK_LOCAL_FRAME_VIEW_H
