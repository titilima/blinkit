/*
 * Copyright (C) 1998, 1999 Torben Weis <weis@kde.org>
 *                     1999 Lars Knoll <knoll@kde.org>
 *                     1999 Antti Koivisto <koivisto@kde.org>
 *                     2000 Dirk Mueller <mueller@kde.org>
 * Copyright (C) 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
 *           (C) 2006 Graham Dennis (graham.dennis@gmail.com)
 *           (C) 2006 Alexey Proskuryakov (ap@nypop.com)
 * Copyright (C) 2009 Google Inc. All rights reserved.
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

#include "core/frame/FrameView.h"

#include "core/HTMLNames.h"
#include "core/MediaTypeNames.h"
#include "core/css/FontFaceSet.h"
#include "core/css/resolver/StyleResolver.h"
#include "core/dom/AXObjectCache.h"
#include "core/dom/Fullscreen.h"
#include "core/dom/IntersectionObserverController.h"
#include "core/editing/EditingUtilities.h"
#include "core/editing/FrameSelection.h"
#include "core/editing/RenderedPosition.h"
#include "core/editing/markers/DocumentMarkerController.h"
#include "core/fetch/ResourceFetcher.h"
#include "core/frame/FrameHost.h"
#include "core/frame/LocalFrame.h"
#include "core/frame/Settings.h"
#include "core/html/HTMLFrameElement.h"
#include "core/html/HTMLPlugInElement.h"
#include "core/html/HTMLTextFormControlElement.h"
#include "core/html/parser/TextResourceDecoder.h"
#include "core/input/EventHandler.h"
#include "core/inspector/InspectorInstrumentation.h"
#include "core/inspector/InspectorTraceEvents.h"
#include "core/layout/LayoutAnalyzer.h"
#include "core/layout/LayoutCounter.h"
#include "core/layout/LayoutEmbeddedObject.h"
#include "core/layout/LayoutInline.h"
#include "core/layout/LayoutListBox.h"
#include "core/layout/LayoutPart.h"
#include "core/layout/LayoutScrollbar.h"
#include "core/layout/LayoutScrollbarPart.h"
#include "core/layout/LayoutTableCell.h"
#include "core/layout/LayoutTheme.h"
#include "core/layout/LayoutView.h"
#include "core/layout/ScrollAlignment.h"
#include "core/layout/TextAutosizer.h"
#include "core/layout/TracedLayoutObject.h"
#include "core/layout/compositing/CompositedLayerMapping.h"
#include "core/layout/compositing/CompositedSelection.h"
#include "core/layout/compositing/PaintLayerCompositor.h"
#include "core/layout/svg/LayoutSVGRoot.h"
#include "core/loader/FrameLoader.h"
#include "core/loader/FrameLoaderClient.h"
#include "core/page/AutoscrollController.h"
#include "core/page/ChromeClient.h"
#include "core/page/FocusController.h"
#include "core/page/FrameTree.h"
#include "core/page/Page.h"
#include "core/page/scrolling/ScrollingCoordinator.h"
#include "core/paint/FramePainter.h"
#include "core/paint/PaintLayer.h"
#include "core/paint/PaintPropertyTreeBuilder.h"
#include "core/plugins/PluginView.h"
#include "core/style/ComputedStyle.h"
#include "core/svg/SVGDocumentExtensions.h"
#include "core/svg/SVGSVGElement.h"
#include "platform/HostWindow.h"
#include "platform/RuntimeEnabledFeatures.h"
#include "platform/ScriptForbiddenScope.h"
#include "platform/TraceEvent.h"
#include "platform/TracedValue.h"
#include "platform/fonts/FontCache.h"
#include "platform/geometry/DoubleRect.h"
#include "platform/geometry/FloatRect.h"
#include "platform/geometry/LayoutRect.h"
#include "platform/graphics/GraphicsContext.h"
#include "platform/graphics/GraphicsLayer.h"
#include "platform/graphics/GraphicsLayerDebugInfo.h"
#include "platform/graphics/paint/CullRect.h"
#include "platform/graphics/paint/PaintController.h"
#include "platform/scheduler/CancellableTaskFactory.h"
#include "platform/scroll/ScrollAnimatorBase.h"
#include "platform/scroll/ScrollbarTheme.h"
#include "platform/text/TextStream.h"
#include "public/platform/WebDisplayItemList.h"
#include "public/platform/WebFrameScheduler.h"
#include "wtf/CurrentTime.h"
#include "wtf/StdLibExtras.h"
#include "wtf/TemporaryChange.h"

namespace blink {

using namespace HTMLNames;

// The maximum number of updateWidgets iterations that should be done before returning.
static const unsigned maxUpdateWidgetsIterations = 2;
static const double resourcePriorityUpdateDelayAfterScroll = 0.250;

static bool s_initialTrackAllPaintInvalidations = false;

FrameView::FrameView(LocalFrame* frame)
    : m_frame(frame)
    , m_displayMode(WebDisplayModeBrowser)
    , m_canHaveScrollbars(true)
    , m_hasPendingLayout(false)
    , m_inSynchronousPostLayout(false)
    , m_postLayoutTasksTimer(this, &FrameView::postLayoutTimerFired)
    , m_updateWidgetsTimer(this, &FrameView::updateWidgetsTimerFired)
    , m_renderThrottlingObserverNotificationFactory(CancellableTaskFactory::create(this, &FrameView::notifyRenderThrottlingObservers))
    , m_isTransparent(false)
    , m_baseBackgroundColor(Color::white)
    , m_mediaType(MediaTypeNames::screen)
    , m_safeToPropagateScrollToParent(true)
    , m_isTrackingPaintInvalidations(false)
    , m_scrollCorner(nullptr)
    , m_inputEventsScaleFactorForEmulation(1)
    , m_layoutSizeFixedToFrameSize(true)
    , m_didScrollTimer(this, &FrameView::didScrollTimerFired)
    , m_topControlsViewportAdjustment(0)
    , m_needsUpdateWidgetGeometries(false)
    , m_needsUpdateViewportIntersection(true)
    , m_needsUpdateViewportIntersectionInSubtree(true)
#if ENABLE(ASSERT)
    , m_hasBeenDisposed(false)
#endif
    , m_horizontalScrollbarMode(ScrollbarAuto)
    , m_verticalScrollbarMode(ScrollbarAuto)
    , m_horizontalScrollbarLock(false)
    , m_verticalScrollbarLock(false)
    , m_scrollbarsAvoidingResizer(0)
    , m_scrollbarsSuppressed(false)
    , m_inUpdateScrollbars(false)
    , m_frameTimingRequestsDirty(true)
    , m_viewportIntersectionValid(false)
    , m_hiddenForThrottling(false)
    , m_crossOriginForThrottling(false)
    , m_isUpdatingAllLifecyclePhases(false)
{
    ASSERT(m_frame);
    init();
}

PassRefPtrWillBeRawPtr<FrameView> FrameView::create(LocalFrame* frame)
{
    RefPtrWillBeRawPtr<FrameView> view = adoptRefWillBeNoop(new FrameView(frame));
    view->show();
    return view.release();
}

PassRefPtrWillBeRawPtr<FrameView> FrameView::create(LocalFrame* frame, const IntSize& initialSize)
{
    RefPtrWillBeRawPtr<FrameView> view = adoptRefWillBeNoop(new FrameView(frame));
    view->Widget::setFrameRect(IntRect(view->location(), initialSize));
    view->setLayoutSizeInternal(initialSize);

    view->show();
    return view.release();
}

FrameView::~FrameView()
{
    ASSERT(m_hasBeenDisposed);
#if !ENABLE(OILPAN)
    // Verify that the LocalFrame has a different FrameView or
    // that it is being detached and destructed.
    ASSERT(frame().view() != this || !layoutView());
#endif
}

DEFINE_TRACE(FrameView)
{
#if ENABLE(OILPAN)
    visitor->trace(m_frame);
    visitor->trace(m_nodeToDraw);
    visitor->trace(m_scrollAnchor);
    visitor->trace(m_scrollableAreas);
    visitor->trace(m_animatingScrollableAreas);
    visitor->trace(m_autoSizeInfo);
    visitor->trace(m_horizontalScrollbar);
    visitor->trace(m_verticalScrollbar);
    visitor->trace(m_children);
    visitor->trace(m_viewportScrollableArea);
#endif
    Widget::trace(visitor);
    ScrollableArea::trace(visitor);
}

void FrameView::reset()
{
    m_hasPendingLayout = false;
    m_doFullPaintInvalidation = false;
    m_layoutSchedulingEnabled = true;
    m_inSynchronousPostLayout = false;
    m_layoutCount = 0;
    m_nestedLayoutCount = 0;
    m_postLayoutTasksTimer.stop();
    m_updateWidgetsTimer.stop();
    m_firstLayout = true;
    m_safeToPropagateScrollToParent = true;
    m_lastViewportSize = IntSize();
    m_lastZoomFactor = 1.0f;
    m_isTrackingPaintInvalidations = s_initialTrackAllPaintInvalidations;
    m_isPainting = false;
    m_visuallyNonEmptyCharacterCount = 0;
    m_visuallyNonEmptyPixelCount = 0;
    m_isVisuallyNonEmpty = false;
    clearScrollAnchor();
    m_viewportConstrainedObjects.clear();
    m_layoutSubtreeRootList.clear();
}

// Call function for each non-throttled frame view in pre tree order.
// Note it needs a null check of the frame's layoutView to access it in case of detached frames.
template <typename Function>
void FrameView::forAllNonThrottledFrameViews(Function function)
{
    if (shouldThrottleRendering())
        return;

    function(*this);

    for (Frame* child = m_frame->tree().firstChild(); child; child = child->tree().nextSibling()) {
        if (!child->isLocalFrame())
            continue;
        if (FrameView* childView = toLocalFrame(child)->view())
            childView->forAllNonThrottledFrameViews(function);
    }
}

void FrameView::removeFromAXObjectCache()
{
    if (AXObjectCache* cache = axObjectCache()) {
        cache->remove(this);
        cache->childrenChanged(m_frame->pagePopupOwner());
    }
}

void FrameView::init()
{
    reset();

    m_size = LayoutSize();

    // Propagate the marginwidth/height and scrolling modes to the view.
    if (m_frame->owner() && m_frame->owner()->scrollingMode() == ScrollbarAlwaysOff)
        setCanHaveScrollbars(false);
}

void FrameView::dispose()
{
    RELEASE_ASSERT(!isInPerformLayout());

    if (ScrollAnimatorBase* scrollAnimator = existingScrollAnimator())
        scrollAnimator->cancelAnimation();
    cancelProgrammaticScrollAnimation();

    detachScrollbars();

    // When the view is no longer associated with a frame, it needs to be removed from the ax object cache
    // right now, otherwise it won't be able to reach the topDocument()'s axObject cache later.
    removeFromAXObjectCache();

    if (ScrollingCoordinator* scrollingCoordinator = this->scrollingCoordinator())
        scrollingCoordinator->willDestroyScrollableArea(this);

    // Destroy |m_autoSizeInfo| as early as possible, to avoid dereferencing
    // partially destroyed |this| via |m_autoSizeInfo->m_frameView|.
    m_autoSizeInfo.clear();

    if (m_postLayoutTasksTimer.isActive())
        m_postLayoutTasksTimer.stop();

    if (m_didScrollTimer.isActive())
        m_didScrollTimer.stop();
    m_renderThrottlingObserverNotificationFactory->cancel();

    // FIXME: Do we need to do something here for OOPI?
    HTMLFrameOwnerElement* ownerElement = m_frame->deprecatedLocalOwner();
    // TODO(dcheng): It seems buggy that we can have an owner element that
    // points to another Widget.
    if (ownerElement && ownerElement->ownedWidget() == this)
        ownerElement->setWidget(nullptr);

#if ENABLE(ASSERT)
    m_hasBeenDisposed = true;
#endif
}

void FrameView::detachScrollbars()
{
    // Previously, we detached custom scrollbars as early as possible to prevent
    // Document::detach() from messing with the view such that its scroll bars
    // won't be torn down. However, scripting in Document::detach() is forbidden
    // now, so it's not clear if these edge cases can still happen.
    // However, for Oilpan, we still need to remove the native scrollbars before
    // we lose the connection to the HostWindow, so we just unconditionally
    // detach any scrollbars now.
    setHasHorizontalScrollbar(false);
    setHasVerticalScrollbar(false);

    if (m_scrollCorner) {
        m_scrollCorner->destroy();
        m_scrollCorner = nullptr;
    }
}

void FrameView::recalculateCustomScrollbarStyle()
{
    bool didStyleChange = false;
    if (m_horizontalScrollbar && m_horizontalScrollbar->isCustomScrollbar()) {
        m_horizontalScrollbar->styleChanged();
        didStyleChange = true;
    }
    if (m_verticalScrollbar && m_verticalScrollbar->isCustomScrollbar()) {
        m_verticalScrollbar->styleChanged();
        didStyleChange = true;
    }
    if (didStyleChange) {
        updateScrollbarGeometry();
        updateScrollCorner();
        positionScrollbarLayers();
    }
}

void FrameView::invalidateAllCustomScrollbarsOnActiveChanged()
{
    bool usesWindowInactiveSelector = m_frame->document()->styleEngine().usesWindowInactiveSelector();

    const ChildrenWidgetSet* viewChildren = children();
    for (const RefPtrWillBeMember<Widget>& child : *viewChildren) {
        Widget* widget = child.get();
        if (widget->isFrameView())
            toFrameView(widget)->invalidateAllCustomScrollbarsOnActiveChanged();
        else if (usesWindowInactiveSelector && widget->isScrollbar() && toScrollbar(widget)->isCustomScrollbar())
            toScrollbar(widget)->styleChanged();
        if (widget->isScrollbar())
            toScrollbar(widget)->windowActiveChangedForSnowLeopardOnly();
    }
    if (usesWindowInactiveSelector)
        recalculateCustomScrollbarStyle();
}

void FrameView::recalculateScrollbarOverlayStyle()
{
    ScrollbarOverlayStyle oldOverlayStyle = scrollbarOverlayStyle();
    ScrollbarOverlayStyle overlayStyle = ScrollbarOverlayStyleDefault;

    Color backgroundColor = documentBackgroundColor();
    // Reduce the background color from RGB to a lightness value
    // and determine which scrollbar style to use based on a lightness
    // heuristic.
    double hue, saturation, lightness;
    backgroundColor.getHSL(hue, saturation, lightness);
    if (lightness <= .5)
        overlayStyle = ScrollbarOverlayStyleLight;

    if (oldOverlayStyle != overlayStyle)
        setScrollbarOverlayStyle(overlayStyle);
}

void FrameView::clear()
{
    reset();
    setScrollbarsSuppressed(true);
}

bool FrameView::didFirstLayout() const
{
    return !m_firstLayout;
}

void FrameView::invalidateRect(const IntRect& rect)
{
    if (!parent()) {
        if (HostWindow* window = hostWindow())
            window->invalidateRect(rect);
        return;
    }

    LayoutPart* layoutObject = m_frame->ownerLayoutObject();
    if (!layoutObject)
        return;

    IntRect paintInvalidationRect = rect;
    paintInvalidationRect.move(layoutObject->borderLeft() + layoutObject->paddingLeft(),
        layoutObject->borderTop() + layoutObject->paddingTop());
    // FIXME: We should not allow paint invalidation out of paint invalidation state. crbug.com/457415
    DisablePaintInvalidationStateAsserts paintInvalidationAssertDisabler;
    layoutObject->invalidatePaintRectangleNotInvalidatingDisplayItemClients(LayoutRect(paintInvalidationRect));
}

void FrameView::setFrameRect(const IntRect& newRect)
{
    IntRect oldRect = frameRect();
    if (newRect == oldRect)
        return;

    Widget::setFrameRect(newRect);

    updateScrollbars(scrollOffsetDouble());
    frameRectsChanged();

    updateScrollableAreaSet();

    if (LayoutView* layoutView = this->layoutView()) {
        if (layoutView->usesCompositing())
            layoutView->compositor()->frameViewDidChangeSize();
    }

    viewportSizeChanged(newRect.width() != oldRect.width(), newRect.height() != oldRect.height());

    if (oldRect.size() != newRect.size()) {
        if (m_frame->isMainFrame())
            m_frame->host()->visualViewport().mainFrameDidChangeSize();
        frame().loader().restoreScrollPositionAndViewState();
    }
}

Page* FrameView::page() const
{
    return frame().page();
}

LayoutView* FrameView::layoutView() const
{
    return frame().contentLayoutObject();
}

ScrollingCoordinator* FrameView::scrollingCoordinator() const
{
    Page* p = page();
    return p ? p->scrollingCoordinator() : 0;
}

void FrameView::setCanHaveScrollbars(bool canHaveScrollbars)
{
    m_canHaveScrollbars = canHaveScrollbars;

    ScrollbarMode newVerticalMode = m_verticalScrollbarMode;
    if (canHaveScrollbars && m_verticalScrollbarMode == ScrollbarAlwaysOff)
        newVerticalMode = ScrollbarAuto;
    else if (!canHaveScrollbars)
        newVerticalMode = ScrollbarAlwaysOff;

    ScrollbarMode newHorizontalMode = m_horizontalScrollbarMode;
    if (canHaveScrollbars && m_horizontalScrollbarMode == ScrollbarAlwaysOff)
        newHorizontalMode = ScrollbarAuto;
    else if (!canHaveScrollbars)
        newHorizontalMode = ScrollbarAlwaysOff;

    setScrollbarModes(newHorizontalMode, newVerticalMode);
}

bool FrameView::shouldUseCustomScrollbars(Element*& customScrollbarElement, LocalFrame*& customScrollbarFrame) const
{
    customScrollbarElement = nullptr;
    customScrollbarFrame = nullptr;

    if (Settings* settings = m_frame->settings()) {
        if (!settings->allowCustomScrollbarInMainFrame() && m_frame->isMainFrame())
            return false;
    }

    // FIXME: We need to update the scrollbar dynamically as documents change (or as doc elements and bodies get discovered that have custom styles).
    Document* doc = m_frame->document();

    // Try the <body> element first as a scrollbar source.
    Element* body = doc ? doc->body() : 0;
    if (body && body->layoutObject() && body->layoutObject()->style()->hasPseudoStyle(SCROLLBAR)) {
        customScrollbarElement = body;
        return true;
    }

    // If the <body> didn't have a custom style, then the root element might.
    Element* docElement = doc ? doc->documentElement() : 0;
    if (docElement && docElement->layoutObject() && docElement->layoutObject()->style()->hasPseudoStyle(SCROLLBAR)) {
        customScrollbarElement = docElement;
        return true;
    }

    // If we have an owning ipage/LocalFrame element, then it can set the custom scrollbar also.
    LayoutPart* frameLayoutObject = m_frame->ownerLayoutObject();
    if (frameLayoutObject && frameLayoutObject->style()->hasPseudoStyle(SCROLLBAR)) {
        customScrollbarFrame = m_frame.get();
        return true;
    }

    return false;
}

PassRefPtrWillBeRawPtr<Scrollbar> FrameView::createScrollbar(ScrollbarOrientation orientation)
{
    Element* customScrollbarElement = nullptr;
    LocalFrame* customScrollbarFrame = nullptr;
    if (shouldUseCustomScrollbars(customScrollbarElement, customScrollbarFrame))
        return LayoutScrollbar::createCustomScrollbar(this, orientation, customScrollbarElement, customScrollbarFrame);

    // Nobody set a custom style, so we just use a native scrollbar.
    return Scrollbar::create(this, orientation, RegularScrollbar, &frame().page()->chromeClient());
}

void FrameView::setContentsSize(const IntSize& size)
{
    if (size == contentsSize())
        return;

    m_contentsSize = size;
    updateScrollbars(scrollOffsetDouble());
    ScrollableArea::contentsResized();

    Page* page = frame().page();
    if (!page)
        return;

    updateScrollableAreaSet();

    page->chromeClient().contentsSizeChanged(m_frame.get(), size);
    frame().loader().restoreScrollPositionAndViewState();
}

void FrameView::adjustViewSize()
{
    LayoutView* layoutView = this->layoutView();
    if (!layoutView)
        return;

    ASSERT(m_frame->view() == this);

    const IntRect rect = layoutView->documentRect();
    const IntSize& size = rect.size();
    setScrollOrigin(IntPoint(-rect.x(), -rect.y()), !m_frame->document()->printing(), size == contentsSize());

    setContentsSize(size);
}

void FrameView::calculateScrollbarModesFromOverflowStyle(const ComputedStyle* style, ScrollbarMode& hMode, ScrollbarMode& vMode)
{
    hMode = vMode = ScrollbarAuto;

    EOverflow overflowX = style->overflowX();
    EOverflow overflowY = style->overflowY();

    if (!shouldIgnoreOverflowHidden()) {
        if (overflowX == OHIDDEN)
            hMode = ScrollbarAlwaysOff;
        if (overflowY == OHIDDEN)
            vMode = ScrollbarAlwaysOff;
    }

    if (overflowX == OSCROLL)
        hMode = ScrollbarAlwaysOn;
    if (overflowY == OSCROLL)
        vMode = ScrollbarAlwaysOn;
}

void FrameView::calculateScrollbarModes(ScrollbarMode& hMode, ScrollbarMode& vMode, ScrollbarModesCalculationStrategy strategy)
{
#define RETURN_SCROLLBAR_MODE(mode) \
    { \
        hMode = vMode = mode; \
        return; \
    }

    // Setting scrolling="no" on an iframe element disables scrolling.
    if (m_frame->owner() && m_frame->owner()->scrollingMode() == ScrollbarAlwaysOff)
        RETURN_SCROLLBAR_MODE(ScrollbarAlwaysOff);

    // Framesets can't scroll.
    Node* body = m_frame->document()->body();
    if (isHTMLFrameSetElement(body) && body->layoutObject())
        RETURN_SCROLLBAR_MODE(ScrollbarAlwaysOff);

    // Scrollbars can be disabled by FrameView::setCanHaveScrollbars.
    if (!m_canHaveScrollbars && strategy != RulesFromWebContentOnly)
        RETURN_SCROLLBAR_MODE(ScrollbarAlwaysOff);

    // This will be the LayoutObject for either the body element or the html element
    // (see Document::viewportDefiningElement).
    LayoutObject* viewport = viewportLayoutObject();
    if (!viewport || !viewport->style())
        RETURN_SCROLLBAR_MODE(ScrollbarAuto);

    if (viewport->isSVGRoot()) {
        // Don't allow overflow to affect <img> and css backgrounds
        if (toLayoutSVGRoot(viewport)->isEmbeddedThroughSVGImage())
            RETURN_SCROLLBAR_MODE(ScrollbarAuto);

        // FIXME: evaluate if we can allow overflow for these cases too.
        // Overflow is always hidden when stand-alone SVG documents are embedded.
        if (toLayoutSVGRoot(viewport)->isEmbeddedThroughFrameContainingSVGDocument())
            RETURN_SCROLLBAR_MODE(ScrollbarAlwaysOff);
    }

    calculateScrollbarModesFromOverflowStyle(viewport->style(), hMode, vMode);

#undef RETURN_SCROLLBAR_MODE
}

void FrameView::updateAcceleratedCompositingSettings()
{
    if (LayoutView* layoutView = this->layoutView())
        layoutView->compositor()->updateAcceleratedCompositingSettings();
}

void FrameView::recalcOverflowAfterStyleChange()
{
    LayoutView* layoutView = this->layoutView();
    RELEASE_ASSERT(layoutView);
    if (!layoutView->needsOverflowRecalcAfterStyleChange())
        return;

    layoutView->recalcOverflowAfterStyleChange();

    // Changing overflow should notify scrolling coordinator to ensures that it
    // updates non-fast scroll rects even if there is no layout.
    if (ScrollingCoordinator* scrollingCoordinator = this->scrollingCoordinator())
        scrollingCoordinator->notifyOverflowUpdated();

    IntRect documentRect = layoutView->documentRect();
    if (scrollOrigin() == -documentRect.location() && contentsSize() == documentRect.size())
        return;

    if (needsLayout())
        return;

    InUpdateScrollbarsScope inUpdateScrollbarsScope(this);

    bool shouldHaveHorizontalScrollbar = false;
    bool shouldHaveVerticalScrollbar = false;
    computeScrollbarExistence(shouldHaveHorizontalScrollbar, shouldHaveVerticalScrollbar, documentRect.size());

    bool hasHorizontalScrollbar = horizontalScrollbar();
    bool hasVerticalScrollbar = verticalScrollbar();
    if (hasHorizontalScrollbar != shouldHaveHorizontalScrollbar
        || hasVerticalScrollbar != shouldHaveVerticalScrollbar) {
        setNeedsLayout();
        return;
    }

    adjustViewSize();
    updateScrollbarGeometry();
}

bool FrameView::usesCompositedScrolling() const
{
    LayoutView* layoutView = this->layoutView();
    if (!layoutView)
        return false;
    if (m_frame->settings() && m_frame->settings()->preferCompositingToLCDTextEnabled())
        return layoutView->compositor()->inCompositingMode();
    return false;
}

bool FrameView::shouldScrollOnMainThread() const
{
    if (ScrollingCoordinator* sc = scrollingCoordinator()) {
        if (sc->shouldUpdateScrollLayerPositionOnMainThread())
            return true;
    }
    return ScrollableArea::shouldScrollOnMainThread();
}

GraphicsLayer* FrameView::layerForScrolling() const
{
    LayoutView* layoutView = this->layoutView();
    if (!layoutView)
        return nullptr;
    return layoutView->compositor()->frameScrollLayer();
}

GraphicsLayer* FrameView::layerForHorizontalScrollbar() const
{
    LayoutView* layoutView = this->layoutView();
    if (!layoutView)
        return nullptr;
    return layoutView->compositor()->layerForHorizontalScrollbar();
}

GraphicsLayer* FrameView::layerForVerticalScrollbar() const
{
    LayoutView* layoutView = this->layoutView();
    if (!layoutView)
        return nullptr;
    return layoutView->compositor()->layerForVerticalScrollbar();
}

GraphicsLayer* FrameView::layerForScrollCorner() const
{
    LayoutView* layoutView = this->layoutView();
    if (!layoutView)
        return nullptr;
    return layoutView->compositor()->layerForScrollCorner();
}

bool FrameView::isEnclosedInCompositingLayer() const
{
    // FIXME: It's a bug that compositing state isn't always up to date when this is called. crbug.com/366314
    DisableCompositingQueryAsserts disabler;

    LayoutObject* frameOwnerLayoutObject = m_frame->ownerLayoutObject();
    return frameOwnerLayoutObject && frameOwnerLayoutObject->enclosingLayer()->enclosingLayerForPaintInvalidationCrossingFrameBoundaries();
}

void FrameView::countObjectsNeedingLayout(unsigned& needsLayoutObjects, unsigned& totalObjects, bool& isSubtree)
{
    needsLayoutObjects = 0;
    totalObjects = 0;
    isSubtree = isSubtreeLayout();
    if (isSubtree)
        m_layoutSubtreeRootList.countObjectsNeedingLayout(needsLayoutObjects, totalObjects);
    else
        LayoutSubtreeRootList::countObjectsNeedingLayoutInRoot(layoutView(), needsLayoutObjects, totalObjects);
}

inline void FrameView::forceLayoutParentViewIfNeeded()
{
    LayoutPart* ownerLayoutObject = m_frame->ownerLayoutObject();
    if (!ownerLayoutObject || !ownerLayoutObject->frame())
        return;

    LayoutBox* contentBox = embeddedContentBox();
    if (!contentBox)
        return;

    LayoutSVGRoot* svgRoot = toLayoutSVGRoot(contentBox);
    if (svgRoot->everHadLayout() && !svgRoot->needsLayout())
        return;

    // If the embedded SVG document appears the first time, the ownerLayoutObject has already finished
    // layout without knowing about the existence of the embedded SVG document, because LayoutReplaced
    // embeddedContentBox() returns 0, as long as the embedded document isn't loaded yet. Before
    // bothering to lay out the SVG document, mark the ownerLayoutObject needing layout and ask its
    // FrameView for a layout. After that the LayoutEmbeddedObject (ownerLayoutObject) carries the
    // correct size, which LayoutSVGRoot::computeReplacedLogicalWidth/Height rely on, when laying
    // out for the first time, or when the LayoutSVGRoot size has changed dynamically (eg. via <script>).
    RefPtrWillBeRawPtr<FrameView> frameView = ownerLayoutObject->frame()->view();

    // Mark the owner layoutObject as needing layout.
    ownerLayoutObject->setNeedsLayoutAndPrefWidthsRecalcAndFullPaintInvalidation(LayoutInvalidationReason::Unknown);

    // Synchronously enter layout, to layout the view containing the host object/embed/iframe.
    ASSERT(frameView);
    frameView->layout();
}

void FrameView::performPreLayoutTasks()
{
    TRACE_EVENT0("blink,benchmark", "FrameView::performPreLayoutTasks");
    lifecycle().advanceTo(DocumentLifecycle::InPreLayout);

    // Don't schedule more layouts, we're in one.
    TemporaryChange<bool> changeSchedulingEnabled(m_layoutSchedulingEnabled, false);

    if (!m_nestedLayoutCount && !m_inSynchronousPostLayout && m_postLayoutTasksTimer.isActive()) {
        // This is a new top-level layout. If there are any remaining tasks from the previous layout, finish them now.
        m_inSynchronousPostLayout = true;
        performPostLayoutTasks();
        m_inSynchronousPostLayout = false;
    }

    bool wasResized = wasViewportResized();
    Document* document = m_frame->document();

    // Viewport-dependent or device-dependent media queries may cause us to need completely different style information.
    if (!document->styleResolver()
        || (wasResized && document->styleResolver()->mediaQueryAffectedByViewportChange())
        || (wasResized && m_frame->settings() && m_frame->settings()->resizeIsDeviceSizeChange() && document->styleResolver()->mediaQueryAffectedByDeviceChange())) {
        document->mediaQueryAffectingValueChanged();
    } else if (wasResized) {
        document->evaluateMediaQueryList();
    }

    document->updateLayoutTreeIfNeeded();
    lifecycle().advanceTo(DocumentLifecycle::StyleClean);

    if (m_frame->isMainFrame() && !m_viewportScrollableArea) {
        ScrollableArea& visualViewport = m_frame->host()->visualViewport();
        ScrollableArea* layoutViewport = layoutViewportScrollableArea();
        ASSERT(layoutViewport);
        m_viewportScrollableArea = RootFrameViewport::create(visualViewport, *layoutViewport);
    }
}

static inline void layoutFromRootObject(LayoutObject& root)
{
    LayoutState layoutState(root);
    root.layout();
}

void FrameView::prepareLayoutAnalyzer()
{
    bool isTracing = false;
    TRACE_EVENT_CATEGORY_GROUP_ENABLED(TRACE_DISABLED_BY_DEFAULT("blink.debug.layout"), &isTracing);
    if (!isTracing) {
        m_analyzer.clear();
        return;
    }
    if (!m_analyzer)
        m_analyzer = adoptPtr(new LayoutAnalyzer());
    m_analyzer->reset();
}

PassRefPtr<TracedValue> FrameView::analyzerCounters()
{
    if (!m_analyzer)
        return TracedValue::create();
    RefPtr<TracedValue> value = m_analyzer->toTracedValue();
    value->setString("host", layoutView()->document().location()->host());
    return value;
}

#define PERFORM_LAYOUT_TRACE_CATEGORIES "blink,benchmark," TRACE_DISABLED_BY_DEFAULT("blink.debug.layout")

void FrameView::performLayout(bool inSubtreeLayout)
{
    ASSERT(inSubtreeLayout || m_layoutSubtreeRootList.isEmpty());

    TRACE_EVENT_BEGIN0(PERFORM_LAYOUT_TRACE_CATEGORIES, "FrameView::performLayout");
    prepareLayoutAnalyzer();

    ScriptForbiddenScope forbidScript;

    ASSERT(!isInPerformLayout());
    lifecycle().advanceTo(DocumentLifecycle::InPerformLayout);

    // performLayout is the actual guts of layout().
    // FIXME: The 300 other lines in layout() probably belong in other helper functions
    // so that a single human could understand what layout() is actually doing.

    forceLayoutParentViewIfNeeded();

    if (inSubtreeLayout) {
        if (m_analyzer)
            m_analyzer->increment(LayoutAnalyzer::PerformLayoutRootLayoutObjects, m_layoutSubtreeRootList.size());
        while (LayoutObject* root = m_layoutSubtreeRootList.takeDeepestRoot()) {
            if (!root->needsLayout())
                continue;
            layoutFromRootObject(*root);

            // We need to ensure that we mark up all layoutObjects up to the LayoutView
            // for paint invalidation. This simplifies our code as we just always
            // do a full tree walk.
            if (LayoutObject* container = root->container())
                container->setMayNeedPaintInvalidation();
        }
    } else {
        layoutFromRootObject(*layoutView());
    }

    m_frame->document()->fetcher()->updateAllImageResourcePriorities();

    lifecycle().advanceTo(DocumentLifecycle::AfterPerformLayout);

    TRACE_EVENT_END1(PERFORM_LAYOUT_TRACE_CATEGORIES, "FrameView::performLayout",
        "counters", analyzerCounters());
}

void FrameView::scheduleOrPerformPostLayoutTasks()
{
    if (m_postLayoutTasksTimer.isActive())
        return;

    if (!m_inSynchronousPostLayout) {
        m_inSynchronousPostLayout = true;
        // Calls resumeScheduledEvents()
        performPostLayoutTasks();
        m_inSynchronousPostLayout = false;
    }

    if (!m_postLayoutTasksTimer.isActive() && (needsLayout() || m_inSynchronousPostLayout)) {
        // If we need layout or are already in a synchronous call to postLayoutTasks(),
        // defer widget updates and event dispatch until after we return. postLayoutTasks()
        // can make us need to update again, and we can get stuck in a nasty cycle unless
        // we call it through the timer here.
        m_postLayoutTasksTimer.startOneShot(0, BLINK_FROM_HERE);
        if (needsLayout())
            layout();
    }
}

void FrameView::layout()
{
    // We should never layout a Document which is not in a LocalFrame.
    ASSERT(m_frame);
    ASSERT(m_frame->view() == this);
    ASSERT(m_frame->page());

    ScriptForbiddenScope forbidScript;

    if (isInPerformLayout() || !m_frame->document()->isActive() || shouldThrottleRendering())
        return;

    TRACE_EVENT0("blink,benchmark", "FrameView::layout");
    TRACE_EVENT_SCOPED_SAMPLING_STATE("blink", "Layout");

    // Protect the view from being deleted during layout (in recalcStyle)
    RefPtrWillBeRawPtr<FrameView> protector(this);

    if (m_autoSizeInfo)
        m_autoSizeInfo->autoSizeIfNeeded();

    m_hasPendingLayout = false;
    DocumentLifecycle::Scope lifecycleScope(lifecycle(), DocumentLifecycle::LayoutClean);

    RELEASE_ASSERT(!isPainting());

    TRACE_EVENT_BEGIN1("devtools.timeline", "Layout", "beginData", InspectorLayoutEvent::beginData(this));

    performPreLayoutTasks();

#if !ENABLE(OILPAN)
    // If there is only one ref to this view left, then its going to be destroyed as soon as we exit,
    // so there's no point to continuing to layout
    if (protector->hasOneRef())
        return;
#endif

    Document* document = m_frame->document();

    // If the layout view was marked as needing layout after we added items in the subtree roots we need
    // to clear the roots and do the layout from the layoutView.
    if (layoutView()->needsLayout())
        clearLayoutSubtreeRootsAndMarkContainingBlocks();
    layoutView()->clearHitTestCache();

    bool inSubtreeLayout = isSubtreeLayout();

    // FIXME: The notion of a single root for layout is no longer applicable. Remove or update this code. crbug.com/460596
    LayoutObject* rootForThisLayout = inSubtreeLayout ? m_layoutSubtreeRootList.randomRoot() : layoutView();
    if (!rootForThisLayout) {
        // FIXME: Do we need to set m_size here?
        ASSERT_NOT_REACHED();
        return;
    }

    FontCachePurgePreventer fontCachePurgePreventer;
    {
        TemporaryChange<bool> changeSchedulingEnabled(m_layoutSchedulingEnabled, false);

        m_nestedLayoutCount++;
        if (!inSubtreeLayout) {
            clearLayoutSubtreeRootsAndMarkContainingBlocks();
            Node* body = document->body();
            if (body && body->layoutObject()) {
                if (isHTMLFrameSetElement(*body)) {
                    body->layoutObject()->setChildNeedsLayout();
                } else if (isHTMLBodyElement(*body)) {
                    if (!m_firstLayout && m_size.height() != layoutSize().height() && body->layoutObject()->enclosingBox()->stretchesToViewport())
                        body->layoutObject()->setChildNeedsLayout();
                }
            }
        }
        updateCounters();

        if (!inSubtreeLayout) {
            ScrollbarMode hMode;
            ScrollbarMode vMode;
            calculateScrollbarModes(hMode, vMode);

            // Now set our scrollbar state for the layout.
            ScrollbarMode currentHMode = horizontalScrollbarMode();
            ScrollbarMode currentVMode = verticalScrollbarMode();

            if (m_firstLayout) {
                setScrollbarsSuppressed(true);

                m_doFullPaintInvalidation = true;
                m_firstLayout = false;
                m_lastViewportSize = layoutSize(IncludeScrollbars);
                m_lastZoomFactor = layoutView()->style()->zoom();

                // Set the initial vMode to AlwaysOn if we're auto.
                if (vMode == ScrollbarAuto)
                    setVerticalScrollbarMode(ScrollbarAlwaysOn); // This causes a vertical scrollbar to appear.
                // Set the initial hMode to AlwaysOff if we're auto.
                if (hMode == ScrollbarAuto)
                    setHorizontalScrollbarMode(ScrollbarAlwaysOff); // This causes a horizontal scrollbar to disappear.

                setScrollbarModes(hMode, vMode);
                setScrollbarsSuppressed(false);
            } else if (hMode != currentHMode || vMode != currentVMode) {
                setScrollbarModes(hMode, vMode);
            }

            if (needsScrollbarReconstruction())
                updateScrollbars(scrollOffsetDouble());

            LayoutSize oldSize = m_size;

            m_size = LayoutSize(layoutSize().width(), layoutSize().height());

            if (oldSize != m_size && !m_firstLayout) {
                LayoutBox* rootLayoutObject = document->documentElement() ? document->documentElement()->layoutBox() : 0;
                LayoutBox* bodyLayoutObject = rootLayoutObject && document->body() ? document->body()->layoutBox() : 0;
                if (bodyLayoutObject && bodyLayoutObject->stretchesToViewport())
                    bodyLayoutObject->setChildNeedsLayout();
                else if (rootLayoutObject && rootLayoutObject->stretchesToViewport())
                    rootLayoutObject->setChildNeedsLayout();
            }

            // We need to set m_doFullPaintInvalidation before triggering layout as LayoutObject::checkForPaintInvalidation
            // checks the boolean to disable local paint invalidations.
            m_doFullPaintInvalidation |= layoutView()->shouldDoFullPaintInvalidationForNextLayout();
        }

        TRACE_EVENT_OBJECT_SNAPSHOT_WITH_ID(TRACE_DISABLED_BY_DEFAULT("blink.debug.layout"), "LayoutTree",
            this, TracedLayoutObject::create(*layoutView(), false));

        performLayout(inSubtreeLayout);

        ASSERT(m_layoutSubtreeRootList.isEmpty());
    } // Reset m_layoutSchedulingEnabled to its previous value.

    if (!inSubtreeLayout && !document->printing())
        adjustViewSize();

    m_frameTimingRequestsDirty = true;

    // FIXME: Could find the common ancestor layer of all dirty subtrees and mark from there. crbug.com/462719
    layoutView()->enclosingLayer()->updateLayerPositionsAfterLayout();

    TRACE_EVENT_OBJECT_SNAPSHOT_WITH_ID(TRACE_DISABLED_BY_DEFAULT("blink.debug.layout"), "LayoutTree",
        this, TracedLayoutObject::create(*layoutView(), true));

    layoutView()->compositor()->didLayout();

    m_layoutCount++;

    if (AXObjectCache* cache = document->axObjectCache()) {
        const KURL& url = document->url();
        if (url.isValid() && !url.isAboutBlankURL())
            cache->handleLayoutComplete(document);
    }
    updateDocumentAnnotatedRegions();

    scheduleOrPerformPostLayoutTasks();

    // FIXME: The notion of a single root for layout is no longer applicable. Remove or update this code. crbug.com/460596
    TRACE_EVENT_END1("devtools.timeline", "Layout", "endData", InspectorLayoutEvent::endData(rootForThisLayout));
    InspectorInstrumentation::didUpdateLayout(m_frame.get());

    m_nestedLayoutCount--;
    if (m_nestedLayoutCount)
        return;

#if ENABLE(ASSERT)
    // Post-layout assert that nobody was re-marked as needing layout during layout.
    layoutView()->assertSubtreeIsLaidOut();
#endif

    frame().document()->layoutUpdated();
}

void FrameView::invalidateTreeIfNeeded(PaintInvalidationState& paintInvalidationState)
{
    if (shouldThrottleRendering())
        return;

    lifecycle().advanceTo(DocumentLifecycle::InPaintInvalidation);

    RELEASE_ASSERT(layoutView());
    LayoutView& rootForPaintInvalidation = *layoutView();
    ASSERT(!rootForPaintInvalidation.needsLayout());

    TRACE_EVENT1("blink", "FrameView::invalidateTree", "root", rootForPaintInvalidation.debugName().ascii());

    rootForPaintInvalidation.invalidateTreeIfNeeded(paintInvalidationState);

    if (!m_frame->settings() || !m_frame->settings()->rootLayerScrolls()) {
        paintInvalidationState.setViewClippingAndScrollOffsetDisabled(true);
        invalidatePaintOfScrollControlsIfNeeded(paintInvalidationState, paintInvalidationState.paintInvalidationContainer());
        paintInvalidationState.setViewClippingAndScrollOffsetDisabled(false);
    }

#if ENABLE(ASSERT)
    layoutView()->assertSubtreeClearedPaintInvalidationState();
#endif

    if (m_frame->selection().isCaretBoundsDirty())
        m_frame->selection().invalidateCaretRect();

    m_doFullPaintInvalidation = false;
    lifecycle().advanceTo(DocumentLifecycle::PaintInvalidationClean);

    // Temporary callback for crbug.com/487345,402044
    // TODO(ojan): Make this more general to be used by PositionObserver
    // and rAF throttling.
    IntRect visibleRect = rootFrameToContents(computeVisibleArea());
    rootForPaintInvalidation.sendMediaPositionChangeNotifications(visibleRect);
}

IntRect FrameView::computeVisibleArea()
{
    // Return our clipping bounds in the root frame.
    IntRect us(frameRect());
    if (FrameView* parent = parentFrameView()) {
        us = parent->contentsToRootFrame(us);
        IntRect parentRect = parent->computeVisibleArea();
        if (parentRect.isEmpty())
            return IntRect();

        us.intersect(parentRect);
    }

    return us;
}

DocumentLifecycle& FrameView::lifecycle() const
{
    return m_frame->document()->lifecycle();
}

LayoutBox* FrameView::embeddedContentBox() const
{
    LayoutView* layoutView = this->layoutView();
    if (!layoutView)
        return nullptr;

    LayoutObject* firstChild = layoutView->firstChild();
    if (!firstChild || !firstChild->isBox())
        return nullptr;

    // Curently only embedded SVG documents participate in the size-negotiation logic.
    if (firstChild->isSVGRoot())
        return toLayoutBox(firstChild);

    return nullptr;
}


void FrameView::addPart(LayoutPart* object)
{
    m_parts.add(object);
}

void FrameView::removePart(LayoutPart* object)
{
    m_parts.remove(object);
}

void FrameView::updateWidgetGeometries()
{
    Vector<RefPtr<LayoutPart>> parts;
    copyToVector(m_parts, parts);

    // Script or plugins could detach the frame so abort processing if that happens.
    for (size_t i = 0; i < parts.size() && layoutView(); ++i)
        parts[i]->updateWidgetGeometry();
}

void FrameView::addPartToUpdate(LayoutEmbeddedObject& object)
{
    ASSERT(isInPerformLayout());
    // Tell the DOM element that it needs a widget update.
    Node* node = object.node();
    ASSERT(node);
    if (isHTMLObjectElement(*node) || isHTMLEmbedElement(*node))
        toHTMLPlugInElement(node)->setNeedsWidgetUpdate(true);

    m_partUpdateSet.add(&object);
}

void FrameView::setDisplayMode(WebDisplayMode mode)
{
    if (mode == m_displayMode)
        return;

    m_displayMode = mode;

    if (m_frame->document())
        m_frame->document()->mediaQueryAffectingValueChanged();
}

void FrameView::setMediaType(const AtomicString& mediaType)
{
    ASSERT(m_frame->document());
    m_frame->document()->mediaQueryAffectingValueChanged();
    m_mediaType = mediaType;
}

AtomicString FrameView::mediaType() const
{
    // See if we have an override type.
    if (m_frame->settings() && !m_frame->settings()->mediaTypeOverride().isEmpty())
        return AtomicString(m_frame->settings()->mediaTypeOverride());
    return m_mediaType;
}

void FrameView::adjustMediaTypeForPrinting(bool printing)
{
    if (printing) {
        if (m_mediaTypeWhenNotPrinting.isNull())
            m_mediaTypeWhenNotPrinting = mediaType();
            setMediaType(MediaTypeNames::print);
    } else {
        if (!m_mediaTypeWhenNotPrinting.isNull())
            setMediaType(m_mediaTypeWhenNotPrinting);
        m_mediaTypeWhenNotPrinting = nullAtom;
    }
}

bool FrameView::contentsInCompositedLayer() const
{
    LayoutView* layoutView = this->layoutView();
    return layoutView && layoutView->compositingState() == PaintsIntoOwnBacking;
}

void FrameView::addBackgroundAttachmentFixedObject(LayoutObject* object)
{
    ASSERT(!m_backgroundAttachmentFixedObjects.contains(object));

    m_backgroundAttachmentFixedObjects.add(object);
    if (ScrollingCoordinator* scrollingCoordinator = this->scrollingCoordinator())
        scrollingCoordinator->frameViewHasBackgroundAttachmentFixedObjectsDidChange(this);
}

void FrameView::removeBackgroundAttachmentFixedObject(LayoutObject* object)
{
    ASSERT(m_backgroundAttachmentFixedObjects.contains(object));

    m_backgroundAttachmentFixedObjects.remove(object);
    if (ScrollingCoordinator* scrollingCoordinator = this->scrollingCoordinator())
        scrollingCoordinator->frameViewHasBackgroundAttachmentFixedObjectsDidChange(this);
}

void FrameView::addViewportConstrainedObject(LayoutObject* object)
{
    if (!m_viewportConstrainedObjects)
        m_viewportConstrainedObjects = adoptPtr(new ViewportConstrainedObjectSet);

    if (!m_viewportConstrainedObjects->contains(object)) {
        m_viewportConstrainedObjects->add(object);

        if (ScrollingCoordinator* scrollingCoordinator = this->scrollingCoordinator())
            scrollingCoordinator->frameViewFixedObjectsDidChange(this);
    }
}

void FrameView::removeViewportConstrainedObject(LayoutObject* object)
{
    if (m_viewportConstrainedObjects && m_viewportConstrainedObjects->contains(object)) {
        m_viewportConstrainedObjects->remove(object);

        if (ScrollingCoordinator* scrollingCoordinator = this->scrollingCoordinator())
            scrollingCoordinator->frameViewFixedObjectsDidChange(this);
    }
}

void FrameView::viewportSizeChanged(bool widthChanged, bool heightChanged)
{
    if (m_frame->settings() && m_frame->settings()->rootLayerScrolls()) {
        // The background must be repainted when the FrameView is resized, even if the initial
        // containing block does not change (so we can't rely on layout to issue the invalidation).
        // This is because the background fills the main GraphicsLayer, which takes the size of the
        // layout viewport.
        // TODO(skobes): Paint non-fixed backgrounds into the scrolling contents layer and avoid
        // this invalidation (http://crbug.com/568847).
        if (LayoutView* lv = layoutView())
            lv->setShouldDoFullPaintInvalidation();
    }

    if (!hasViewportConstrainedObjects())
        return;

    for (const auto& viewportConstrainedObject : *m_viewportConstrainedObjects) {
        LayoutObject* layoutObject = viewportConstrainedObject;
        const ComputedStyle& style = layoutObject->styleRef();
        if (widthChanged) {
            if (style.width().isFixed() && (style.left().isAuto() || style.right().isAuto()))
                layoutObject->setNeedsPositionedMovementLayout();
            else
                layoutObject->setNeedsLayoutAndFullPaintInvalidation(LayoutInvalidationReason::SizeChanged);
        }
        if (heightChanged) {
            if (style.height().isFixed() && (style.top().isAuto() || style.bottom().isAuto()))
                layoutObject->setNeedsPositionedMovementLayout();
            else
                layoutObject->setNeedsLayoutAndFullPaintInvalidation(LayoutInvalidationReason::SizeChanged);
        }
    }
}

IntPoint FrameView::lastKnownMousePosition() const
{
    return m_frame->eventHandler().lastKnownMousePosition();
}

bool FrameView::shouldSetCursor() const
{
    Page* page = frame().page();
    return page && page->visibilityState() != PageVisibilityStateHidden && page->focusController().isActive() && page->settings().deviceSupportsMouse();
}

void FrameView::scrollContentsIfNeededRecursive()
{
    forAllNonThrottledFrameViews([](FrameView& frameView) {
        frameView.scrollContentsIfNeeded();
    });
}

void FrameView::invalidateBackgroundAttachmentFixedObjects()
{
    for (const auto& layoutObject : m_backgroundAttachmentFixedObjects)
        layoutObject->setShouldDoFullPaintInvalidation();
}

bool FrameView::invalidateViewportConstrainedObjects()
{
    for (const auto& viewportConstrainedObject : *m_viewportConstrainedObjects) {
        LayoutObject* layoutObject = viewportConstrainedObject;
        ASSERT(layoutObject->style()->hasViewportConstrainedPosition());
        ASSERT(layoutObject->hasLayer());
        PaintLayer* layer = toLayoutBoxModelObject(layoutObject)->layer();

        if (layer->isPaintInvalidationContainer())
            continue;

        if (layer->subtreeIsInvisible())
            continue;

        // If the fixed layer has a blur/drop-shadow filter applied on at least one of its parents, we cannot
        // scroll using the fast path, otherwise the outsets of the filter will be moved around the page.
        if (layer->hasAncestorWithFilterOutsets())
            return false;

        TRACE_EVENT_INSTANT1(
            TRACE_DISABLED_BY_DEFAULT("devtools.timeline.invalidationTracking"),
            "ScrollInvalidationTracking",
            TRACE_EVENT_SCOPE_THREAD,
            "data",
            InspectorScrollInvalidationTrackingEvent::data(*layoutObject));

        layoutObject->setShouldDoFullPaintInvalidationIncludingNonCompositingDescendants();
    }
    return true;
}

bool FrameView::scrollContentsFastPath(const IntSize& scrollDelta)
{
    if (!contentsInCompositedLayer())
        return false;

    invalidateBackgroundAttachmentFixedObjects();

    if (!m_viewportConstrainedObjects || m_viewportConstrainedObjects->isEmpty()) {
        InspectorInstrumentation::didUpdateLayout(m_frame.get());
        return true;
    }

    if (!invalidateViewportConstrainedObjects())
        return false;

    InspectorInstrumentation::didUpdateLayout(m_frame.get());
    return true;
}

void FrameView::scrollContentsSlowPath(const IntRect& updateRect)
{
    TRACE_EVENT0("blink", "FrameView::scrollContentsSlowPath");
    // We need full invalidation during slow scrolling. For slimming paint, full invalidation
    // of the LayoutView is not enough. We also need to invalidate all of the objects.
    // FIXME: Find out what are enough to invalidate in slow path scrolling. crbug.com/451090#9.
    ASSERT(layoutView());
    if (contentsInCompositedLayer())
        layoutView()->layer()->compositedLayerMapping()->setContentsNeedDisplay();
    else
        layoutView()->setShouldDoFullPaintInvalidationIncludingNonCompositingDescendants();


    if (contentsInCompositedLayer()) {
        IntRect updateRect = visibleContentRect();
        ASSERT(layoutView());
        // FIXME: We should not allow paint invalidation out of paint invalidation state. crbug.com/457415
        DisablePaintInvalidationStateAsserts disabler;
        layoutView()->invalidatePaintRectangle(LayoutRect(updateRect));
    }
    if (LayoutPart* frameLayoutObject = m_frame->ownerLayoutObject()) {
        if (isEnclosedInCompositingLayer()) {
            LayoutRect rect(frameLayoutObject->borderLeft() + frameLayoutObject->paddingLeft(),
                frameLayoutObject->borderTop() + frameLayoutObject->paddingTop(),
                visibleWidth(), visibleHeight());
            // FIXME: We should not allow paint invalidation out of paint invalidation state. crbug.com/457415
            DisablePaintInvalidationStateAsserts disabler;
            frameLayoutObject->invalidatePaintRectangle(rect);
            return;
        }
    }

    hostWindow()->invalidateRect(updateRect);
}

void FrameView::restoreScrollbar()
{
    setScrollbarsSuppressed(false);
}

bool FrameView::processUrlFragment(const KURL& url, UrlFragmentBehavior behavior)
{
    // If our URL has no ref, then we have no place we need to jump to.
    // OTOH If CSS target was set previously, we want to set it to 0, recalc
    // and possibly paint invalidation because :target pseudo class may have been
    // set (see bug 11321).
    // Similarly for svg, if we had a previous svgView() then we need to reset
    // the initial view if we don't have a fragment.
    if (!url.hasFragmentIdentifier() && !m_frame->document()->cssTarget() && !m_frame->document()->isSVGDocument())
        return false;

    String fragmentIdentifier = url.fragmentIdentifier();
    if (processUrlFragmentHelper(fragmentIdentifier, behavior))
        return true;

    // Try again after decoding the ref, based on the document's encoding.
    if (m_frame->document()->encoding().isValid())
        return processUrlFragmentHelper(decodeURLEscapeSequences(fragmentIdentifier, m_frame->document()->encoding()), behavior);

    return false;
}

bool FrameView::processUrlFragmentHelper(const String& name, UrlFragmentBehavior behavior)
{
    ASSERT(m_frame->document());

    if (behavior == UrlFragmentScroll && !m_frame->document()->isRenderingReady()) {
        m_frame->document()->setGotoAnchorNeededAfterStylesheetsLoad(true);
        return false;
    }

    m_frame->document()->setGotoAnchorNeededAfterStylesheetsLoad(false);

    Element* anchorNode = m_frame->document()->findAnchor(name);

    // Setting to null will clear the current target.
    m_frame->document()->setCSSTarget(anchorNode);

    if (m_frame->document()->isSVGDocument()) {
        if (SVGSVGElement* svg = SVGDocumentExtensions::rootElement(*m_frame->document())) {
            svg->setupInitialView(name, anchorNode);
            if (!anchorNode)
                return true;
        }
    }

    // Implement the rule that "" and "top" both mean top of page as in other browsers.
    if (!anchorNode && !(name.isEmpty() || equalIgnoringCase(name, "top")))
        return false;

    if (behavior == UrlFragmentScroll)
        maintainScrollPositionAtAnchor(anchorNode ? static_cast<Node*>(anchorNode) : m_frame->document());

    // If the anchor accepts keyboard focus, move focus there to aid users
    // relying on keyboard navigation.
    // If anchorNode is not focusable, clear focus, which matches the behavior
    // of other browsers.
    if (anchorNode) {
        m_frame->document()->updateLayoutIgnorePendingStylesheets();
        if (anchorNode->isFocusable())
            anchorNode->focus();
        else
            m_frame->document()->clearFocusedElement();
    }
    return true;
}

void FrameView::maintainScrollPositionAtAnchor(Node* anchorNode)
{
    ASSERT(anchorNode);
    m_scrollAnchor = anchorNode;

    // We need to update the layout tree before scrolling.
    m_frame->document()->updateLayoutTreeIfNeeded();

    // If layout is needed, we will scroll in performPostLayoutTasks. Otherwise, scroll immediately.
    LayoutView* layoutView = this->layoutView();
    if (layoutView && layoutView->needsLayout())
        layout();
    else
        scrollToAnchor();
}

void FrameView::clearScrollAnchor()
{
    m_scrollAnchor = nullptr;
}

void FrameView::setScrollPosition(const DoublePoint& scrollPoint, ScrollType scrollType, ScrollBehavior scrollBehavior)
{
    DoublePoint newScrollPosition = clampScrollPosition(scrollPoint);
    if (newScrollPosition == scrollPositionDouble())
        return;

    if (scrollBehavior == ScrollBehaviorAuto)
        scrollBehavior = scrollBehaviorStyle();

    ScrollableArea::setScrollPosition(newScrollPosition, scrollType, scrollBehavior);
}

void FrameView::didUpdateElasticOverscroll()
{
    Page* page = frame().page();
    if (!page)
        return;
    FloatSize elasticOverscroll = page->chromeClient().elasticOverscroll();
    if (m_horizontalScrollbar) {
        float delta = elasticOverscroll.width() - m_horizontalScrollbar->elasticOverscroll();
        if (delta != 0) {
            m_horizontalScrollbar->setElasticOverscroll(elasticOverscroll.width());
            scrollAnimator().notifyContentAreaScrolled(FloatSize(delta, 0));
            setScrollbarNeedsPaintInvalidation(HorizontalScrollbar);
        }
    }
    if (m_verticalScrollbar) {
        float delta = elasticOverscroll.height() - m_verticalScrollbar->elasticOverscroll();
        if (delta != 0) {
            m_verticalScrollbar->setElasticOverscroll(elasticOverscroll.height());
            scrollAnimator().notifyContentAreaScrolled(FloatSize(0, delta));
            setScrollbarNeedsPaintInvalidation(VerticalScrollbar);
        }
    }
}

IntSize FrameView::layoutSize(IncludeScrollbarsInRect scrollbarInclusion) const
{
    return scrollbarInclusion == ExcludeScrollbars ? excludeScrollbars(m_layoutSize) : m_layoutSize;
}

void FrameView::setLayoutSize(const IntSize& size)
{
    ASSERT(!layoutSizeFixedToFrameSize());

    setLayoutSizeInternal(size);
}

void FrameView::scrollPositionChanged()
{
    Document* document = m_frame->document();
    document->enqueueScrollEventForNode(document);

    m_frame->eventHandler().dispatchFakeMouseMoveEventSoon();

    if (LayoutView* layoutView = document->layoutView()) {
        if (layoutView->usesCompositing())
            layoutView->compositor()->frameViewDidScroll();
    }

    if (m_didScrollTimer.isActive())
        m_didScrollTimer.stop();
    m_didScrollTimer.startOneShot(resourcePriorityUpdateDelayAfterScroll, BLINK_FROM_HERE);

    if (AXObjectCache* cache = m_frame->document()->existingAXObjectCache())
        cache->handleScrollPositionChanged(this);

    layoutView()->clearHitTestCache();
    frame().loader().saveScrollState();
}

void FrameView::didScrollTimerFired(Timer<FrameView>*)
{
    if (m_frame->document() && m_frame->document()->layoutView())
        m_frame->document()->fetcher()->updateAllImageResourcePriorities();
}

void FrameView::updateLayersAndCompositingAfterScrollIfNeeded()
{
    // Nothing to do after scrolling if there are no fixed position elements.
    if (!hasViewportConstrainedObjects())
        return;

    RefPtrWillBeRawPtr<FrameView> protect(this);

    // If there fixed position elements, scrolling may cause compositing layers to change.
    // Update widget and layer positions after scrolling, but only if we're not inside of
    // layout.
    if (!m_nestedLayoutCount) {
        updateWidgetGeometries();
        if (LayoutView* layoutView = this->layoutView())
            layoutView->layer()->setNeedsCompositingInputsUpdate();
    }
}

bool FrameView::computeCompositedSelection(LocalFrame& frame, CompositedSelection& selection)
{
    const VisibleSelection& visibleSelection = frame.selection().selection();
    if (!visibleSelection.isCaretOrRange())
        return false;

    // Non-editable caret selections lack any kind of UI affordance, and
    // needn't be tracked by the client.
    if (visibleSelection.isCaret() && !visibleSelection.isContentEditable())
        return false;

    VisiblePosition visibleStart(visibleSelection.visibleStart());
    RenderedPosition renderedStart(visibleStart);
    renderedStart.positionInGraphicsLayerBacking(selection.start, true);
    if (!selection.start.layer)
        return false;

    VisiblePosition visibleEnd(visibleSelection.visibleEnd());
    RenderedPosition renderedEnd(visibleEnd);
    renderedEnd.positionInGraphicsLayerBacking(selection.end, false);
    if (!selection.end.layer)
        return false;

    selection.type = visibleSelection.selectionType();
    selection.isEditable = visibleSelection.isContentEditable();
    if (selection.isEditable) {
        if (HTMLTextFormControlElement* enclosingTextFormControlElement = enclosingTextFormControl(visibleSelection.rootEditableElement()))
            selection.isEmptyTextFormControl = enclosingTextFormControlElement->value().isEmpty();
    }
    selection.start.isTextDirectionRTL |= primaryDirectionOf(*visibleSelection.start().anchorNode()) == RTL;
    selection.end.isTextDirectionRTL |= primaryDirectionOf(*visibleSelection.end().anchorNode()) == RTL;

    return true;
}

void FrameView::updateCompositedSelectionIfNeeded()
{
    if (!RuntimeEnabledFeatures::compositedSelectionUpdateEnabled())
        return;

    TRACE_EVENT0("blink", "FrameView::updateCompositedSelectionIfNeeded");

    Page* page = frame().page();
    ASSERT(page);

    CompositedSelection selection;
    Frame* focusedFrame = page->focusController().focusedOrMainFrame();
    LocalFrame* localFrame = focusedFrame->isLocalFrame() ? toLocalFrame(focusedFrame) : nullptr;
    if (!localFrame || !computeCompositedSelection(*localFrame, selection)) {
        page->chromeClient().clearCompositedSelection();
        return;
    }

    page->chromeClient().updateCompositedSelection(selection);
}

HostWindow* FrameView::hostWindow() const
{
    Page* page = frame().page();
    if (!page)
        return nullptr;
    return &page->chromeClient();
}

void FrameView::contentsResized()
{
    if (m_frame->isMainFrame() && m_frame->document()) {
        if (TextAutosizer* textAutosizer = m_frame->document()->textAutosizer())
            textAutosizer->updatePageInfoInAllFrames();
    }

    ScrollableArea::contentsResized();
    setNeedsLayout();
}

void FrameView::scrollbarExistenceDidChange()
{
    // We check to make sure the view is attached to a frame() as this method can
    // be triggered before the view is attached by LocalFrame::createView(...) setting
    // various values such as setScrollBarModes(...) for example.  An ASSERT is
    // triggered when a view is layout before being attached to a frame().
    if (!frame().view())
        return;

    bool hasOverlayScrollbars = this->hasOverlayScrollbars();

    // FIXME: this call to layout() could be called within FrameView::layout(), but before performLayout(),
    // causing double-layout. See also crbug.com/429242.
    if (!hasOverlayScrollbars && needsLayout())
        layout();

    if (layoutView() && layoutView()->usesCompositing()) {
        layoutView()->compositor()->frameViewScrollbarsExistenceDidChange();

        if (!hasOverlayScrollbars)
            layoutView()->compositor()->frameViewDidChangeSize();
    }
}

void FrameView::handleLoadCompleted()
{
    // Once loading has completed, allow autoSize one last opportunity to
    // reduce the size of the frame.
    if (m_autoSizeInfo)
        m_autoSizeInfo->autoSizeIfNeeded();

    // If there is a pending layout, the scroll anchor will be cleared when it finishes.
    if (!needsLayout())
        clearScrollAnchor();
}

void FrameView::clearLayoutSubtreeRoot(const LayoutObject& root)
{
    m_layoutSubtreeRootList.removeRoot(const_cast<LayoutObject&>(root));
}

void FrameView::clearLayoutSubtreeRootsAndMarkContainingBlocks()
{
    m_layoutSubtreeRootList.clearAndMarkContainingBlocksForLayout();
}

void FrameView::scheduleRelayout()
{
    ASSERT(m_frame->view() == this);

    if (!m_layoutSchedulingEnabled)
        return;
    if (!needsLayout())
        return;
    if (!m_frame->document()->shouldScheduleLayout())
        return;
    TRACE_EVENT_INSTANT1(TRACE_DISABLED_BY_DEFAULT("devtools.timeline"), "InvalidateLayout", TRACE_EVENT_SCOPE_THREAD, "data", InspectorInvalidateLayoutEvent::data(m_frame.get()));

    clearLayoutSubtreeRootsAndMarkContainingBlocks();

    if (m_hasPendingLayout)
        return;
    m_hasPendingLayout = true;

    if (!shouldThrottleRendering())
        page()->animator().scheduleVisualUpdate(m_frame.get());
    lifecycle().ensureStateAtMost(DocumentLifecycle::StyleClean);
}

void FrameView::scheduleRelayoutOfSubtree(LayoutObject* relayoutRoot)
{
    ASSERT(m_frame->view() == this);

    // FIXME: Should this call shouldScheduleLayout instead?
    if (!m_frame->document()->isActive())
        return;

    LayoutView* layoutView = this->layoutView();
    if (layoutView && layoutView->needsLayout()) {
        if (relayoutRoot)
            relayoutRoot->markContainerChainForLayout(false);
        return;
    }

    if (relayoutRoot == layoutView)
        m_layoutSubtreeRootList.clearAndMarkContainingBlocksForLayout();
    else
        m_layoutSubtreeRootList.addRoot(*relayoutRoot);

    if (m_layoutSchedulingEnabled) {
        m_hasPendingLayout = true;

        page()->animator().scheduleVisualUpdate(m_frame.get());
        lifecycle().ensureStateAtMost(DocumentLifecycle::StyleClean);
    }
    TRACE_EVENT_INSTANT1(TRACE_DISABLED_BY_DEFAULT("devtools.timeline"), "InvalidateLayout", TRACE_EVENT_SCOPE_THREAD, "data", InspectorInvalidateLayoutEvent::data(m_frame.get()));
}

bool FrameView::layoutPending() const
{
    // FIXME: This should check Document::lifecycle instead.
    return m_hasPendingLayout;
}

bool FrameView::isInPerformLayout() const
{
    return lifecycle().state() == DocumentLifecycle::InPerformLayout;
}

bool FrameView::needsLayout() const
{
    // This can return true in cases where the document does not have a body yet.
    // Document::shouldScheduleLayout takes care of preventing us from scheduling
    // layout in that case.

    LayoutView* layoutView = this->layoutView();
    return layoutPending()
        || (layoutView && layoutView->needsLayout())
        || isSubtreeLayout();
}

void FrameView::setNeedsLayout()
{
    LayoutBox* box = embeddedContentBox();
    // It's illegal to ask for layout changes during the layout compositing or paint invalidation step.
    // FIXME: the third conditional is a hack to support embedded SVG. See FrameView::forceLayoutParentViewIfNeeded and crbug.com/442939
    RELEASE_ASSERT(!m_frame->document() || m_frame->document()->lifecycle().stateAllowsLayoutInvalidation() || (box && box->isSVGRoot()));

    if (LayoutView* layoutView = this->layoutView())
        layoutView->setNeedsLayout(LayoutInvalidationReason::Unknown);
}

bool FrameView::isTransparent() const
{
    return m_isTransparent;
}

void FrameView::setTransparent(bool isTransparent)
{
    m_isTransparent = isTransparent;
    DisableCompositingQueryAsserts disabler;
    if (layoutView() && layoutView()->layer()->hasCompositedLayerMapping())
        layoutView()->layer()->compositedLayerMapping()->updateContentsOpaque();
}

bool FrameView::hasOpaqueBackground() const
{
    return !m_isTransparent && !m_baseBackgroundColor.hasAlpha();
}

Color FrameView::baseBackgroundColor() const
{
    return m_baseBackgroundColor;
}

void FrameView::setBaseBackgroundColor(const Color& backgroundColor)
{
    m_baseBackgroundColor = backgroundColor;

    if (layoutView() && layoutView()->layer()->hasCompositedLayerMapping()) {
        CompositedLayerMapping* compositedLayerMapping = layoutView()->layer()->compositedLayerMapping();
        compositedLayerMapping->updateContentsOpaque();
        if (compositedLayerMapping->mainGraphicsLayer())
            compositedLayerMapping->mainGraphicsLayer()->setNeedsDisplay();
    }
    recalculateScrollbarOverlayStyle();
}

void FrameView::updateBackgroundRecursively(const Color& backgroundColor, bool transparent)
{
    forAllNonThrottledFrameViews([backgroundColor, transparent](FrameView& frameView) {
        frameView.setTransparent(transparent);
        frameView.setBaseBackgroundColor(backgroundColor);
    });
}

void FrameView::scrollToAnchor()
{
    RefPtrWillBeRawPtr<Node> anchorNode = m_scrollAnchor;
    if (!anchorNode)
        return;

    // Scrolling is disabled during updateScrollbars (see isProgrammaticallyScrollable).
    // Bail now to avoid clearing m_scrollAnchor before we actually have a chance to scroll.
    if (m_inUpdateScrollbars)
        return;

    if (anchorNode->layoutObject()) {
        LayoutRect rect;
        if (anchorNode != m_frame->document()) {
            rect = anchorNode->boundingBox();
        } else if (m_frame->settings() && m_frame->settings()->rootLayerScrolls()) {
            if (Element* documentElement = m_frame->document()->documentElement())
                rect = documentElement->boundingBox();
        }

        RefPtrWillBeRawPtr<Frame> boundaryFrame = m_frame->findUnsafeParentScrollPropagationBoundary();

        // FIXME: Handle RemoteFrames
        if (boundaryFrame && boundaryFrame->isLocalFrame())
            toLocalFrame(boundaryFrame.get())->view()->setSafeToPropagateScrollToParent(false);

        // Scroll nested layers and frames to reveal the anchor.
        // Align to the top and to the closest side (this matches other browsers).
        anchorNode->layoutObject()->scrollRectToVisible(rect, ScrollAlignment::alignToEdgeIfNeeded, ScrollAlignment::alignTopAlways);

        if (boundaryFrame && boundaryFrame->isLocalFrame())
            toLocalFrame(boundaryFrame.get())->view()->setSafeToPropagateScrollToParent(true);

        if (AXObjectCache* cache = m_frame->document()->existingAXObjectCache())
            cache->handleScrolledToAnchor(anchorNode.get());
    }

    // The scroll anchor should only be maintained while the frame is still loading.
    // If the frame is done loading, clear the anchor now. Otherwise, restore it
    // since it may have been cleared during scrollRectToVisible.
    m_scrollAnchor = m_frame->document()->isLoadCompleted() ? nullptr : anchorNode;
}

bool FrameView::updateWidgets()
{
    // This is always called from updateWidgetsTimerFired.
    // m_updateWidgetsTimer should only be scheduled if we have widgets to update.
    // Thus I believe we can stop checking isEmpty here, and just ASSERT isEmpty:
    // FIXME: This assert has been temporarily removed due to https://crbug.com/430344
    if (m_nestedLayoutCount > 1 || m_partUpdateSet.isEmpty())
        return true;

    // Need to swap because script will run inside the below loop and invalidate the iterator.
    EmbeddedObjectSet objects;
    objects.swap(m_partUpdateSet);

    for (const auto& embeddedObject : objects) {
        LayoutEmbeddedObject& object = *embeddedObject;
        HTMLPlugInElement* element = toHTMLPlugInElement(object.node());

        // The object may have already been destroyed (thus node cleared),
        // but FrameView holds a manual ref, so it won't have been deleted.
        if (!element)
            continue;

        // No need to update if it's already crashed or known to be missing.
        if (object.showsUnavailablePluginIndicator())
            continue;

        if (element->needsWidgetUpdate())
            element->updateWidget();
        object.updateWidgetGeometry();

        // Prevent plugins from causing infinite updates of themselves.
        // FIXME: Do we really need to prevent this?
        m_partUpdateSet.remove(&object);
    }

    return m_partUpdateSet.isEmpty();
}

void FrameView::updateWidgetsTimerFired(Timer<FrameView>*)
{
    ASSERT(!isInPerformLayout());
    RefPtrWillBeRawPtr<FrameView> protect(this);
    m_updateWidgetsTimer.stop();
    for (unsigned i = 0; i < maxUpdateWidgetsIterations; ++i) {
        if (updateWidgets())
            return;
    }
}

void FrameView::flushAnyPendingPostLayoutTasks()
{
    ASSERT(!isInPerformLayout());
    if (m_postLayoutTasksTimer.isActive())
        performPostLayoutTasks();
    if (m_updateWidgetsTimer.isActive())
        updateWidgetsTimerFired(0);
}

void FrameView::scheduleUpdateWidgetsIfNecessary()
{
    ASSERT(!isInPerformLayout());
    if (m_updateWidgetsTimer.isActive() || m_partUpdateSet.isEmpty())
        return;
    m_updateWidgetsTimer.startOneShot(0, BLINK_FROM_HERE);
}

void FrameView::performPostLayoutTasks()
{
    // FIXME: We can reach here, even when the page is not active!
    // http/tests/inspector/elements/html-link-import.html and many other
    // tests hit that case.
    // We should ASSERT(isActive()); or at least return early if we can!
    ASSERT(!isInPerformLayout()); // Always before or after performLayout(), part of the highest-level layout() call.
    TRACE_EVENT0("blink,benchmark", "FrameView::performPostLayoutTasks");
    RefPtrWillBeRawPtr<FrameView> protect(this);

    m_postLayoutTasksTimer.stop();

    m_frame->selection().setCaretRectNeedsUpdate();
    m_frame->selection().updateAppearance();

    ASSERT(m_frame->document());

    FontFaceSet::didLayout(*m_frame->document());
    // Cursor update scheduling is done by the local root, which is the main frame if there
    // are no RemoteFrame ancestors in the frame tree. Use of localFrameRoot() is
    // discouraged but will change when cursor update scheduling is moved from EventHandler
    // to PageEventHandler.
    frame().localFrameRoot()->eventHandler().scheduleCursorUpdate();

    updateWidgetGeometries();

    // Plugins could have torn down the page inside updateWidgetGeometries().
    if (!layoutView())
        return;

    scheduleUpdateWidgetsIfNecessary();

    if (ScrollingCoordinator* scrollingCoordinator = this->scrollingCoordinator())
        scrollingCoordinator->notifyGeometryChanged();

    scrollToAnchor();

    sendResizeEventIfNeeded();
}

bool FrameView::wasViewportResized()
{
    ASSERT(m_frame);
    LayoutView* layoutView = this->layoutView();
    if (!layoutView)
        return false;
    ASSERT(layoutView->style());
    return (layoutSize(IncludeScrollbars) != m_lastViewportSize || layoutView->style()->zoom() != m_lastZoomFactor);
}

void FrameView::sendResizeEventIfNeeded()
{
    ASSERT(m_frame);

    LayoutView* layoutView = this->layoutView();
    if (!layoutView || layoutView->document().printing())
        return;

    if (!wasViewportResized())
        return;

    m_lastViewportSize = layoutSize(IncludeScrollbars);
    m_lastZoomFactor = layoutView->style()->zoom();

    m_frame->document()->enqueueResizeEvent();

    if (m_frame->isMainFrame())
        InspectorInstrumentation::didResizeMainFrame(m_frame.get());
}

void FrameView::postLayoutTimerFired(Timer<FrameView>*)
{
    performPostLayoutTasks();
}

void FrameView::updateCounters()
{
    LayoutView* view = layoutView();
    if (!view->hasLayoutCounters())
        return;

    for (LayoutObject* layoutObject = view; layoutObject; layoutObject = layoutObject->nextInPreOrder()) {
        if (!layoutObject->isCounter())
            continue;

        toLayoutCounter(layoutObject)->updateCounter();
    }
}

IntRect FrameView::windowClipRect(IncludeScrollbarsInRect scrollbarInclusion) const
{
    ASSERT(m_frame->view() == this);

    LayoutRect clipRect(LayoutPoint(), LayoutSize(visibleContentSize(scrollbarInclusion)));
    layoutView()->mapToVisibleRectInAncestorSpace(&layoutView()->containerForPaintInvalidation(), clipRect, nullptr);
    return enclosingIntRect(clipRect);
}

bool FrameView::shouldUseIntegerScrollOffset() const
{
    if (m_frame->settings() && !m_frame->settings()->preferCompositingToLCDTextEnabled())
        return true;

    return ScrollableArea::shouldUseIntegerScrollOffset();
}

bool FrameView::isActive() const
{
    Page* page = frame().page();
    return page && page->focusController().isActive();
}

void FrameView::scrollTo(const DoublePoint& newPosition)
{
    DoublePoint oldPosition = m_scrollPosition;
    DoubleSize scrollDelta = newPosition - oldPosition;
    if (scrollDelta.isZero())
        return;

    if (m_frame->settings() && m_frame->settings()->rootLayerScrolls()) {
        // Don't scroll the FrameView!
        ASSERT_NOT_REACHED();
    }

    m_scrollPosition = newPosition;

    if (!scrollbarsSuppressed())
        m_pendingScrollDelta += scrollDelta;

    clearScrollAnchor();
    updateLayersAndCompositingAfterScrollIfNeeded();
    scrollPositionChanged();
    frame().loader().client()->didChangeScrollOffset();
}

void FrameView::invalidatePaintForTickmarks()
{
    if (Scrollbar* scrollbar = verticalScrollbar())
        scrollbar->setNeedsPaintInvalidation(static_cast<ScrollbarPart>(~ThumbPart));
}

void FrameView::getTickmarks(Vector<IntRect>& tickmarks) const
{
    if (!m_tickmarks.isEmpty())
        tickmarks = m_tickmarks;
    else
        tickmarks = frame().document()->markers().renderedRectsForMarkers(DocumentMarker::TextMatch);
}

IntRect FrameView::windowResizerRect() const
{
    if (Page* page = frame().page())
        return page->chromeClient().windowResizerRect();
    return IntRect();
}

void FrameView::setInputEventsTransformForEmulation(const IntSize& offset, float contentScaleFactor)
{
    m_inputEventsOffsetForEmulation = offset;
    m_inputEventsScaleFactorForEmulation = contentScaleFactor;
}

IntSize FrameView::inputEventsOffsetForEmulation() const
{
    return m_inputEventsOffsetForEmulation;
}

float FrameView::inputEventsScaleFactor() const
{
    float pageScale = m_frame->host()->visualViewport().scale();
    return pageScale * m_inputEventsScaleFactorForEmulation;
}

bool FrameView::scrollbarsCanBeActive() const
{
    if (m_frame->view() != this)
        return false;

    return !!m_frame->document();
}

void FrameView::scrollbarVisibilityChanged()
{
    if (LayoutView* view = layoutView())
        view->clearHitTestCache();
}

IntRect FrameView::scrollableAreaBoundingBox() const
{
    LayoutPart* ownerLayoutObject = frame().ownerLayoutObject();
    if (!ownerLayoutObject)
        return frameRect();

    return ownerLayoutObject->absoluteContentQuad().enclosingBoundingBox();
}


bool FrameView::isScrollable()
{
    return scrollingReasons() == Scrollable;
}

bool FrameView::isProgrammaticallyScrollable()
{
    return !m_inUpdateScrollbars;
}

FrameView::ScrollingReasons FrameView::scrollingReasons()
{
    // Check for:
    // 1) If there an actual overflow.
    // 2) display:none or visibility:hidden set to self or inherited.
    // 3) overflow{-x,-y}: hidden;
    // 4) scrolling: no;

    // Covers #1
    IntSize contentsSize = this->contentsSize();
    IntSize visibleContentSize = visibleContentRect().size();
    if ((contentsSize.height() <= visibleContentSize.height() && contentsSize.width() <= visibleContentSize.width()))
        return NotScrollableNoOverflow;

    // Covers #2.
    // FIXME: Do we need to fix this for OOPI?
    HTMLFrameOwnerElement* owner = m_frame->deprecatedLocalOwner();
    if (owner && (!owner->layoutObject() || !owner->layoutObject()->visibleToHitTesting()))
        return NotScrollableNotVisible;

    // Cover #3 and #4.
    ScrollbarMode horizontalMode;
    ScrollbarMode verticalMode;
    calculateScrollbarModes(horizontalMode, verticalMode, RulesFromWebContentOnly);
    if (horizontalMode == ScrollbarAlwaysOff && verticalMode == ScrollbarAlwaysOff)
        return NotScrollableExplicitlyDisabled;

    return Scrollable;
}

void FrameView::updateScrollableAreaSet()
{
    // That ensures that only inner frames are cached.
    FrameView* parentFrameView = this->parentFrameView();
    if (!parentFrameView)
        return;

    if (!isScrollable()) {
        parentFrameView->removeScrollableArea(this);
        return;
    }

    parentFrameView->addScrollableArea(this);
}

bool FrameView::shouldSuspendScrollAnimations() const
{
    return !m_frame->document()->loadEventFinished();
}

void FrameView::scrollbarStyleChanged()
{
    // FIXME: Why does this only apply to the main frame?
    if (!m_frame->isMainFrame())
        return;
    adjustScrollbarOpacity();
    contentsResized();
    updateScrollbars(scrollOffsetDouble());
    positionScrollbarLayers();
}

void FrameView::notifyPageThatContentAreaWillPaint() const
{
    Page* page = m_frame->page();
    if (!page)
        return;

    contentAreaWillPaint();

    if (!m_scrollableAreas)
        return;

    for (const auto& scrollableArea : *m_scrollableAreas) {
        if (!scrollableArea->scrollbarsCanBeActive())
            continue;

        scrollableArea->contentAreaWillPaint();
    }
}

bool FrameView::scrollAnimatorEnabled() const
{
    return m_frame->settings() && m_frame->settings()->scrollAnimatorEnabled();
}

void FrameView::updateDocumentAnnotatedRegions() const
{
    Document* document = m_frame->document();
    if (!document->hasAnnotatedRegions())
        return;
    Vector<AnnotatedRegionValue> newRegions;
    collectAnnotatedRegions(*(document->layoutBox()), newRegions);
    if (newRegions == document->annotatedRegions())
        return;
    document->setAnnotatedRegions(newRegions);
    if (Page* page = m_frame->page())
        page->chromeClient().annotatedRegionsChanged();
}

void FrameView::updateScrollCorner()
{
    RefPtr<ComputedStyle> cornerStyle;
    IntRect cornerRect = scrollCornerRect();
    Document* doc = m_frame->document();

    if (doc && !cornerRect.isEmpty()) {
        // Try the <body> element first as a scroll corner source.
        if (Element* body = doc->body()) {
            if (LayoutObject* layoutObject = body->layoutObject())
                cornerStyle = layoutObject->getUncachedPseudoStyle(PseudoStyleRequest(SCROLLBAR_CORNER), layoutObject->style());
        }

        if (!cornerStyle) {
            // If the <body> didn't have a custom style, then the root element might.
            if (Element* docElement = doc->documentElement()) {
                if (LayoutObject* layoutObject = docElement->layoutObject())
                    cornerStyle = layoutObject->getUncachedPseudoStyle(PseudoStyleRequest(SCROLLBAR_CORNER), layoutObject->style());
            }
        }

        if (!cornerStyle) {
            // If we have an owning ipage/LocalFrame element, then it can set the custom scrollbar also.
            if (LayoutPart* layoutObject = m_frame->ownerLayoutObject())
                cornerStyle = layoutObject->getUncachedPseudoStyle(PseudoStyleRequest(SCROLLBAR_CORNER), layoutObject->style());
        }
    }

    if (cornerStyle) {
        if (!m_scrollCorner)
            m_scrollCorner = LayoutScrollbarPart::createAnonymous(doc);
        m_scrollCorner->setStyle(cornerStyle.release());
        setScrollCornerNeedsPaintInvalidation();
    } else if (m_scrollCorner) {
        m_scrollCorner->destroy();
        m_scrollCorner = nullptr;
    }
}

Color FrameView::documentBackgroundColor() const
{
    // The LayoutView's background color is set in Document::inheritHtmlAndBodyElementStyles.
    // Blend this with the base background color of the FrameView. This should match the color
    // drawn by ViewPainter::paintBoxDecorationBackground.
    Color result = baseBackgroundColor();
    if (LayoutObject* documentLayoutObject = layoutView())
        result = result.blend(documentLayoutObject->resolveColor(CSSPropertyBackgroundColor));
    return result;
}

FrameView* FrameView::parentFrameView() const
{
    if (!parent())
        return nullptr;

    Frame* parentFrame = m_frame->tree().parent();
    if (parentFrame && parentFrame->isLocalFrame())
        return toLocalFrame(parentFrame)->view();

    return nullptr;
}

bool FrameView::isPainting() const
{
    return m_isPainting;
}

void FrameView::setNodeToDraw(Node* node)
{
    m_nodeToDraw = node;
}

void FrameView::updateWidgetGeometriesIfNeeded()
{
    if (!m_needsUpdateWidgetGeometries)
        return;

    m_needsUpdateWidgetGeometries = false;

    updateWidgetGeometries();
}

void FrameView::updateAllLifecyclePhases()
{
    frame().localFrameRoot()->view()->updateLifecyclePhasesInternal(AllPhases);
}

// TODO(chrishtr): add a scrolling update lifecycle phase.
void FrameView::updateLifecycleToCompositingCleanPlusScrolling()
{
    frame().localFrameRoot()->view()->updateLifecyclePhasesInternal(OnlyUpToCompositingCleanPlusScrolling);
}

void FrameView::updateLifecycleToLayoutClean()
{
    frame().localFrameRoot()->view()->updateLifecyclePhasesInternal(OnlyUpToLayoutClean);
}

void FrameView::scheduleVisualUpdateForPaintInvalidationIfNeeded()
{
    LocalFrame* localFrameRoot = frame().localFrameRoot();
    if (!localFrameRoot->view()->m_isUpdatingAllLifecyclePhases || lifecycle().state() >= DocumentLifecycle::PaintInvalidationClean) {
        // Schedule visual update to process the paint invalidation in the next cycle.
        localFrameRoot->scheduleVisualUpdateUnlessThrottled();
    }
    // Otherwise the paint invalidation will be handled in paint invalidation phase of this cycle.
}

// TODO(leviw): We don't assert lifecycle information from documents in child PluginViews.
void FrameView::updateLifecyclePhasesInternal(LifeCycleUpdateOption phases)
{
    Optional<TemporaryChange<bool>> isUpdatingAllLifecyclePhasesScope;
    if (phases == AllPhases)
        isUpdatingAllLifecyclePhasesScope.emplace(m_isUpdatingAllLifecyclePhases, true);

    // This must be called from the root frame, since it recurses down, not up.
    // Otherwise the lifecycles of the frames might be out of sync.
    ASSERT(m_frame->isLocalRoot());

    // Updating layout can run script, which can tear down the FrameView.
    RefPtrWillBeRawPtr<FrameView> protector(this);

    if (shouldThrottleRendering()) {
        updateViewportIntersectionsForSubtree(std::min(phases, OnlyUpToCompositingCleanPlusScrolling));
        return;
    }

    updateStyleAndLayoutIfNeededRecursive();
    ASSERT(lifecycle().state() >= DocumentLifecycle::LayoutClean);

    if (phases == OnlyUpToLayoutClean) {
        updateViewportIntersectionsForSubtree(phases);
        return;
    }

    if (LayoutView* view = layoutView()) {
        {
            TRACE_EVENT1("devtools.timeline", "UpdateLayerTree", "data", InspectorUpdateLayerTreeEvent::data(m_frame.get()));

            // This was required for slimming paint v1 but is only temporarily
            // needed for slimming paint v2.
            view->compositor()->updateIfNeededRecursive();
            scrollContentsIfNeededRecursive();

            ASSERT(lifecycle().state() >= DocumentLifecycle::CompositingClean);

            if (phases == AllPhases) {
                invalidateTreeIfNeededRecursive();

                if (view->compositor()->inCompositingMode())
                    scrollingCoordinator()->updateAfterCompositingChangeIfNeeded();

                updateCompositedSelectionIfNeeded();
            }
        }

        if (phases == AllPhases) {
            if (RuntimeEnabledFeatures::slimmingPaintV2Enabled())
                updatePaintProperties();

            if (!m_frame->document()->printing())
                synchronizedPaint();

            if (RuntimeEnabledFeatures::frameTimingSupportEnabled())
                updateFrameTimingRequestsIfNeeded();

            if (RuntimeEnabledFeatures::slimmingPaintV2Enabled())
                pushPaintArtifactToCompositor();

            ASSERT(!view->hasPendingSelection());
            ASSERT((m_frame->document()->printing() && lifecycle().state() == DocumentLifecycle::PaintInvalidationClean)
                || lifecycle().state() == DocumentLifecycle::PaintClean);
        }
    }

    updateViewportIntersectionsForSubtree(phases);
}

void FrameView::updatePaintProperties()
{
    TRACE_EVENT0("blink", "FrameView::updatePaintProperties");

    ASSERT(RuntimeEnabledFeatures::slimmingPaintV2Enabled());

    forAllNonThrottledFrameViews([](FrameView& frameView) { frameView.lifecycle().advanceTo(DocumentLifecycle::InUpdatePaintProperties); });
    PaintPropertyTreeBuilder().buildPropertyTrees(*this);
    forAllNonThrottledFrameViews([](FrameView& frameView) { frameView.lifecycle().advanceTo(DocumentLifecycle::UpdatePaintPropertiesClean); });
}

void FrameView::synchronizedPaint()
{
    TRACE_EVENT0("blink", "FrameView::synchronizedPaint");

    ASSERT(frame() == page()->mainFrame() || (!frame().tree().parent()->isLocalFrame()));

    LayoutView* view = layoutView();
    ASSERT(view);
    forAllNonThrottledFrameViews([](FrameView& frameView) { frameView.lifecycle().advanceTo(DocumentLifecycle::InPaint); });

    // A null graphics layer can occur for painting of SVG images that are not parented into the main frame tree,
    // or when the FrameView is the main frame view of a page overlay. The page overlay is in the layer tree of
    // the host page and will be painted during synchronized painting of the host page.
    if (GraphicsLayer* rootGraphicsLayer = view->compositor()->rootGraphicsLayer()) {
        synchronizedPaintRecursively(rootGraphicsLayer);
    }
    if (GraphicsLayer* layerForHorizontalScrollbar = view->compositor()->layerForHorizontalScrollbar()) {
        synchronizedPaintRecursively(layerForHorizontalScrollbar);
    }
    if (GraphicsLayer* layerForVerticalScrollbar = view->compositor()->layerForVerticalScrollbar()) {
        synchronizedPaintRecursively(layerForVerticalScrollbar);
    }
    if (GraphicsLayer* layerForScrollCorner = view->compositor()->layerForScrollCorner()) {
        synchronizedPaintRecursively(layerForScrollCorner);
    }

    forAllNonThrottledFrameViews([](FrameView& frameView) {
        frameView.lifecycle().advanceTo(DocumentLifecycle::PaintClean);
        if (LayoutView* layoutView = frameView.layoutView())
            layoutView->layer()->clearNeedsRepaintRecursively();
    });
}

void FrameView::synchronizedPaintRecursively(GraphicsLayer* graphicsLayer)
{
    if (graphicsLayer->drawsContent())
        graphicsLayer->paint(nullptr);

    if (!RuntimeEnabledFeatures::slimmingPaintV2Enabled()) {
        if (GraphicsLayer* maskLayer = graphicsLayer->maskLayer())
            synchronizedPaintRecursively(maskLayer);
        if (GraphicsLayer* contentsClippingMaskLayer = graphicsLayer->contentsClippingMaskLayer())
            synchronizedPaintRecursively(contentsClippingMaskLayer);
        if (GraphicsLayer* replicaLayer = graphicsLayer->replicaLayer())
            synchronizedPaintRecursively(replicaLayer);
    }

    for (auto& child : graphicsLayer->children())
        synchronizedPaintRecursively(child);
}

void FrameView::pushPaintArtifactToCompositor()
{
    TRACE_EVENT0("blink", "FrameView::pushPaintArtifactToCompositor");

    ASSERT(RuntimeEnabledFeatures::slimmingPaintV2Enabled());

    LayoutView* view = layoutView();
    ASSERT(view);

    // TODO(jbroman): Simplify the path to PaintController.
    PaintLayer* layer = view->layer();
    ASSERT(layer);
    if (!layer->hasCompositedLayerMapping())
        return;
    GraphicsLayer* rootGraphicsLayer = layer->compositedLayerMapping()->mainGraphicsLayer();
    if (!rootGraphicsLayer->drawsContent())
        return;
    const PaintArtifact& paintArtifact = rootGraphicsLayer->paintController().paintArtifact();

    Page* page = frame().page();
    if (!page)
        return;
    page->chromeClient().didPaint(paintArtifact);
}

void FrameView::updateFrameTimingRequestsIfNeeded()
{
    GraphicsLayerFrameTimingRequests graphicsLayerTimingRequests;
    // TODO(mpb) use a 'dirty' bit to not call this every time.
    collectFrameTimingRequestsRecursive(graphicsLayerTimingRequests);

    for (const auto& iter : graphicsLayerTimingRequests) {
        const GraphicsLayer* graphicsLayer = iter.key;
        graphicsLayer->platformLayer()->setFrameTimingRequests(iter.value);
    }
}

void FrameView::updateStyleAndLayoutIfNeededRecursive()
{
    if (shouldThrottleRendering())
        return;

    // We have to crawl our entire subtree looking for any FrameViews that need
    // layout and make sure they are up to date.
    // Mac actually tests for intersection with the dirty region and tries not to
    // update layout for frames that are outside the dirty region.  Not only does this seem
    // pointless (since those frames will have set a zero timer to layout anyway), but
    // it is also incorrect, since if two frames overlap, the first could be excluded from the dirty
    // region but then become included later by the second frame adding rects to the dirty region
    // when it lays out.

    m_frame->document()->updateLayoutTreeIfNeeded();

    if (needsLayout())
        layout();

    // WebView plugins need to update regardless of whether the LayoutEmbeddedObject
    // that owns them needed layout.
    // TODO(leviw): This currently runs the entire lifecycle on plugin WebViews. We
    // should have a way to only run these other Documents to the same lifecycle stage
    // as this frame.
    const ChildrenWidgetSet* viewChildren = children();
    for (const RefPtrWillBeMember<Widget>& child : *viewChildren) {
        if ((*child).isPluginContainer())
            toPluginView(child.get())->updateAllLifecyclePhases();
    }

    // FIXME: Calling layout() shouldn't trigger script execution or have any
    // observable effects on the frame tree but we're not quite there yet.
    WillBeHeapVector<RefPtrWillBeMember<FrameView>> frameViews;
    for (Frame* child = m_frame->tree().firstChild(); child; child = child->tree().nextSibling()) {
        if (!child->isLocalFrame())
            continue;
        if (FrameView* view = toLocalFrame(child)->view())
            frameViews.append(view);
    }

    for (const auto& frameView : frameViews)
        frameView->updateStyleAndLayoutIfNeededRecursive();

    // When an <iframe> gets composited, it triggers an extra style recalc in its containing FrameView.
    // To avoid pushing an invalid tree for display, we have to check for this case and do another
    // style recalc. The extra style recalc needs to happen after our child <iframes> were updated.
    // FIXME: We shouldn't be triggering an extra style recalc in the first place.
    if (m_frame->document()->hasSVGFilterElementsRequiringLayerUpdate()) {
        m_frame->document()->updateLayoutTreeIfNeeded();

        if (needsLayout())
            layout();
    }

    // These asserts ensure that parent frames are clean, when child frames finished updating layout and style.
    ASSERT(!needsLayout());
    ASSERT(!m_frame->document()->hasSVGFilterElementsRequiringLayerUpdate());
#if ENABLE(ASSERT)
    m_frame->document()->layoutView()->assertLaidOut();
#endif

    updateWidgetGeometriesIfNeeded();

    if (lifecycle().state() < DocumentLifecycle::LayoutClean)
        lifecycle().advanceTo(DocumentLifecycle::LayoutClean);

    // Ensure that we become visually non-empty eventually.
    // TODO(esprehn): This should check isRenderingReady() instead.
    if (frame().document()->hasFinishedParsing() && frame().loader().stateMachine()->committedFirstRealDocumentLoad())
        m_isVisuallyNonEmpty = true;
}

void FrameView::invalidateTreeIfNeededRecursive()
{
    RELEASE_ASSERT(layoutView());

    // We need to stop recursing here since a child frame view might not be throttled
    // even though we are (e.g., it didn't compute its visibility yet).
    if (shouldThrottleRendering())
        return;
    TRACE_EVENT1("blink", "FrameView::invalidateTreeIfNeededRecursive", "root", layoutView()->debugName().ascii());

    Vector<LayoutObject*> pendingDelayedPaintInvalidations;
    PaintInvalidationState rootPaintInvalidationState(*layoutView(), pendingDelayedPaintInvalidations);

    if (lifecycle().state() < DocumentLifecycle::PaintInvalidationClean)
        invalidateTreeIfNeeded(rootPaintInvalidationState);

    // Some frames may be not reached during the above invalidateTreeIfNeeded because
    // - the frame is a detached frame; or
    // - it didn't need paint invalidation.
    // We need to call invalidateTreeIfNeededRecursive() for such frames to finish required
    // paint invalidation and advance their life cycle state.
    for (Frame* child = m_frame->tree().firstChild(); child; child = child->tree().nextSibling()) {
        if (child->isLocalFrame()) {
            FrameView& childFrameView = *toLocalFrame(child)->view();
            // The children frames can be in any state, including stopping.
            // Thus we have to check that it makes sense to do paint
            // invalidation onto them here.
            if (!childFrameView.layoutView())
                continue;
            childFrameView.invalidateTreeIfNeededRecursive();
        }
    }

    // Process objects needing paint invalidation on the next frame. See the definition of PaintInvalidationDelayedFull for more details.
    for (auto& target : pendingDelayedPaintInvalidations)
        target->setShouldDoFullPaintInvalidation(PaintInvalidationDelayedFull);
}

void FrameView::enableAutoSizeMode(const IntSize& minSize, const IntSize& maxSize)
{
    if (!m_autoSizeInfo)
        m_autoSizeInfo = FrameViewAutoSizeInfo::create(this);

    m_autoSizeInfo->configureAutoSizeMode(minSize, maxSize);
    setLayoutSizeFixedToFrameSize(true);
    setNeedsLayout();
    scheduleRelayout();
}

void FrameView::disableAutoSizeMode()
{
    if (!m_autoSizeInfo)
        return;

    setLayoutSizeFixedToFrameSize(false);
    setNeedsLayout();
    scheduleRelayout();

    // Since autosize mode forces the scrollbar mode, change them to being auto.
    setVerticalScrollbarLock(false);
    setHorizontalScrollbarLock(false);
    setScrollbarModes(ScrollbarAuto, ScrollbarAuto);
    m_autoSizeInfo.clear();
}

void FrameView::forceLayoutForPagination(const FloatSize& pageSize, const FloatSize& originalPageSize, float maximumShrinkFactor)
{
    // Dumping externalRepresentation(m_frame->layoutObject()).ascii() is a good trick to see
    // the state of things before and after the layout
    if (LayoutView* layoutView = this->layoutView()) {
        float pageLogicalWidth = layoutView->style()->isHorizontalWritingMode() ? pageSize.width() : pageSize.height();
        float pageLogicalHeight = layoutView->style()->isHorizontalWritingMode() ? pageSize.height() : pageSize.width();

        LayoutUnit flooredPageLogicalWidth = static_cast<LayoutUnit>(pageLogicalWidth);
        LayoutUnit flooredPageLogicalHeight = static_cast<LayoutUnit>(pageLogicalHeight);
        layoutView->setLogicalWidth(flooredPageLogicalWidth);
        layoutView->setPageLogicalHeight(flooredPageLogicalHeight);
        layoutView->setNeedsLayoutAndPrefWidthsRecalcAndFullPaintInvalidation(LayoutInvalidationReason::PrintingChanged);
        layout();

        // If we don't fit in the given page width, we'll lay out again. If we don't fit in the
        // page width when shrunk, we will lay out at maximum shrink and clip extra content.
        // FIXME: We are assuming a shrink-to-fit printing implementation.  A cropping
        // implementation should not do this!
        bool horizontalWritingMode = layoutView->style()->isHorizontalWritingMode();
        const LayoutRect& documentRect = LayoutRect(layoutView->documentRect());
        LayoutUnit docLogicalWidth = horizontalWritingMode ? documentRect.width() : documentRect.height();
        if (docLogicalWidth > pageLogicalWidth) {
            FloatSize expectedPageSize(std::min<float>(documentRect.width().toFloat(), pageSize.width() * maximumShrinkFactor), std::min<float>(documentRect.height().toFloat(), pageSize.height() * maximumShrinkFactor));
            FloatSize maxPageSize = m_frame->resizePageRectsKeepingRatio(FloatSize(originalPageSize.width(), originalPageSize.height()), expectedPageSize);
            pageLogicalWidth = horizontalWritingMode ? maxPageSize.width() : maxPageSize.height();
            pageLogicalHeight = horizontalWritingMode ? maxPageSize.height() : maxPageSize.width();

            flooredPageLogicalWidth = static_cast<LayoutUnit>(pageLogicalWidth);
            flooredPageLogicalHeight = static_cast<LayoutUnit>(pageLogicalHeight);
            layoutView->setLogicalWidth(flooredPageLogicalWidth);
            layoutView->setPageLogicalHeight(flooredPageLogicalHeight);
            layoutView->setNeedsLayoutAndPrefWidthsRecalcAndFullPaintInvalidation(LayoutInvalidationReason::PrintingChanged);
            layout();

            const LayoutRect& updatedDocumentRect = LayoutRect(layoutView->documentRect());
            LayoutUnit docLogicalHeight = horizontalWritingMode ? updatedDocumentRect.height() : updatedDocumentRect.width();
            LayoutUnit docLogicalTop = horizontalWritingMode ? updatedDocumentRect.y() : updatedDocumentRect.x();
            LayoutUnit docLogicalRight = horizontalWritingMode ? updatedDocumentRect.maxX() : updatedDocumentRect.maxY();
            LayoutUnit clippedLogicalLeft = 0;
            if (!layoutView->style()->isLeftToRightDirection())
                clippedLogicalLeft = docLogicalRight - pageLogicalWidth;
            LayoutRect overflow(clippedLogicalLeft, docLogicalTop, pageLogicalWidth, docLogicalHeight);

            if (!horizontalWritingMode)
                overflow = overflow.transposedRect();
            layoutView->clearLayoutOverflow();
            layoutView->addLayoutOverflow(overflow); // This is how we clip in case we overflow again.
        }
    }

    adjustViewSize();
}

IntRect FrameView::convertFromLayoutObject(const LayoutObject& layoutObject, const IntRect& layoutObjectRect) const
{
    IntRect rect = pixelSnappedIntRect(enclosingLayoutRect(layoutObject.localToAbsoluteQuad(FloatRect(layoutObjectRect)).boundingBox()));

    // Convert from page ("absolute") to FrameView coordinates.
    rect.moveBy(-scrollPosition());

    return rect;
}

IntRect FrameView::convertToLayoutObject(const LayoutObject& layoutObject, const IntRect& frameRect) const
{
    IntRect rectInContent = frameToContents(frameRect);

    // Convert from FrameView coords into page ("absolute") coordinates.
    rectInContent.moveBy(scrollPosition());

    // FIXME: we don't have a way to map an absolute rect down to a local quad, so just
    // move the rect for now.
    rectInContent.setLocation(roundedIntPoint(layoutObject.absoluteToLocal(rectInContent.location(), UseTransforms)));
    return rectInContent;
}

IntPoint FrameView::convertFromLayoutObject(const LayoutObject& layoutObject, const IntPoint& layoutObjectPoint) const
{
    IntPoint point = roundedIntPoint(layoutObject.localToAbsolute(layoutObjectPoint, UseTransforms));

    // Convert from page ("absolute") to FrameView coordinates.
    point.moveBy(-scrollPosition());
    return point;
}

IntPoint FrameView::convertToLayoutObject(const LayoutObject& layoutObject, const IntPoint& framePoint) const
{
    IntPoint point = framePoint;

    // Convert from FrameView coords into page ("absolute") coordinates.
    point += IntSize(scrollX(), scrollY());

    return roundedIntPoint(layoutObject.absoluteToLocal(point, UseTransforms));
}

IntRect FrameView::convertToContainingWidget(const IntRect& localRect) const
{
    if (const FrameView* parentView = toFrameView(parent())) {
        // Get our layoutObject in the parent view
        LayoutPart* layoutObject = m_frame->ownerLayoutObject();
        if (!layoutObject)
            return localRect;

        IntRect rect(localRect);
        // Add borders and padding??
        rect.move(layoutObject->borderLeft() + layoutObject->paddingLeft(),
            layoutObject->borderTop() + layoutObject->paddingTop());
        return parentView->convertFromLayoutObject(*layoutObject, rect);
    }

    return localRect;
}

IntRect FrameView::convertFromContainingWidget(const IntRect& parentRect) const
{
    if (const FrameView* parentView = toFrameView(parent())) {
        // Get our layoutObject in the parent view
        LayoutPart* layoutObject = m_frame->ownerLayoutObject();
        if (!layoutObject)
            return parentRect;

        IntRect rect = parentView->convertToLayoutObject(*layoutObject, parentRect);
        // Subtract borders and padding
        rect.move(-layoutObject->borderLeft() - layoutObject->paddingLeft(),
            -layoutObject->borderTop() - layoutObject->paddingTop());
        return rect;
    }

    return parentRect;
}

IntPoint FrameView::convertToContainingWidget(const IntPoint& localPoint) const
{
    if (const FrameView* parentView = toFrameView(parent())) {
        // Get our layoutObject in the parent view
        LayoutPart* layoutObject = m_frame->ownerLayoutObject();
        if (!layoutObject)
            return localPoint;

        IntPoint point(localPoint);

        // Add borders and padding
        point.move(layoutObject->borderLeft() + layoutObject->paddingLeft(),
            layoutObject->borderTop() + layoutObject->paddingTop());
        return parentView->convertFromLayoutObject(*layoutObject, point);
    }

    return localPoint;
}

IntPoint FrameView::convertFromContainingWidget(const IntPoint& parentPoint) const
{
    if (const FrameView* parentView = toFrameView(parent())) {
        // Get our layoutObject in the parent view
        LayoutPart* layoutObject = m_frame->ownerLayoutObject();
        if (!layoutObject)
            return parentPoint;

        IntPoint point = parentView->convertToLayoutObject(*layoutObject, parentPoint);
        // Subtract borders and padding
        point.move(-layoutObject->borderLeft() - layoutObject->paddingLeft(),
            -layoutObject->borderTop() - layoutObject->paddingTop());
        return point;
    }

    return parentPoint;
}

void FrameView::setInitialTracksPaintInvalidationsForTesting(bool trackPaintInvalidations)
{
    s_initialTrackAllPaintInvalidations = trackPaintInvalidations;
}

void FrameView::setTracksPaintInvalidations(bool trackPaintInvalidations)
{
    if (trackPaintInvalidations == m_isTrackingPaintInvalidations)
        return;

    for (Frame* frame = m_frame->tree().top(); frame; frame = frame->tree().traverseNext()) {
        if (!frame->isLocalFrame())
            continue;
        if (LayoutView* layoutView = toLocalFrame(frame)->contentLayoutObject())
            layoutView->compositor()->setTracksPaintInvalidations(trackPaintInvalidations);
    }

    TRACE_EVENT_INSTANT1(TRACE_DISABLED_BY_DEFAULT("blink.invalidation"),
        "FrameView::setTracksPaintInvalidations", TRACE_EVENT_SCOPE_GLOBAL, "enabled", trackPaintInvalidations);

    resetTrackedPaintInvalidations();
    m_isTrackingPaintInvalidations = trackPaintInvalidations;
}

void FrameView::resetTrackedPaintInvalidations()
{
    if (LayoutView* layoutView = this->layoutView())
        layoutView->compositor()->resetTrackedPaintInvalidationRects();
}

void FrameView::addResizerArea(LayoutBox& resizerBox)
{
    if (!m_resizerAreas)
        m_resizerAreas = adoptPtr(new ResizerAreaSet);
    m_resizerAreas->add(&resizerBox);
}

void FrameView::removeResizerArea(LayoutBox& resizerBox)
{
    if (!m_resizerAreas)
        return;

    ResizerAreaSet::iterator it = m_resizerAreas->find(&resizerBox);
    if (it != m_resizerAreas->end())
        m_resizerAreas->remove(it);
}

void FrameView::addScrollableArea(ScrollableArea* scrollableArea)
{
    ASSERT(scrollableArea);
    if (!m_scrollableAreas)
        m_scrollableAreas = adoptPtrWillBeNoop(new ScrollableAreaSet);
    m_scrollableAreas->add(scrollableArea);

    if (ScrollingCoordinator* scrollingCoordinator = this->scrollingCoordinator())
        scrollingCoordinator->scrollableAreasDidChange();
}

void FrameView::removeScrollableArea(ScrollableArea* scrollableArea)
{
    if (!m_scrollableAreas)
        return;
    m_scrollableAreas->remove(scrollableArea);

    if (ScrollingCoordinator* scrollingCoordinator = this->scrollingCoordinator())
        scrollingCoordinator->scrollableAreasDidChange();
}

void FrameView::addAnimatingScrollableArea(ScrollableArea* scrollableArea)
{
    ASSERT(scrollableArea);
    if (!m_animatingScrollableAreas)
        m_animatingScrollableAreas = adoptPtrWillBeNoop(new ScrollableAreaSet);
    m_animatingScrollableAreas->add(scrollableArea);
}

void FrameView::removeAnimatingScrollableArea(ScrollableArea* scrollableArea)
{
    if (!m_animatingScrollableAreas)
        return;
    m_animatingScrollableAreas->remove(scrollableArea);
}

void FrameView::setParent(Widget* parentView)
{
    if (parentView == parent())
        return;

    if (m_scrollbarsAvoidingResizer && parent())
        toFrameView(parent())->adjustScrollbarsAvoidingResizerCount(-m_scrollbarsAvoidingResizer);

    Widget::setParent(parentView);

    if (m_scrollbarsAvoidingResizer && parent())
        toFrameView(parent())->adjustScrollbarsAvoidingResizerCount(m_scrollbarsAvoidingResizer);

    updateScrollableAreaSet();
    setNeedsUpdateViewportIntersection();
}

void FrameView::removeChild(Widget* child)
{
    ASSERT(child->parent() == this);

    if (child->isFrameView())
        removeScrollableArea(toFrameView(child));

    child->setParent(0);
    m_children.remove(child);
}

bool FrameView::isVerticalDocument() const
{
    LayoutView* layoutView = this->layoutView();
    if (!layoutView)
        return true;

    return layoutView->style()->isHorizontalWritingMode();
}

bool FrameView::isFlippedDocument() const
{
    LayoutView* layoutView = this->layoutView();
    if (!layoutView)
        return false;

    return layoutView->hasFlippedBlocksWritingMode();
}

bool FrameView::visualViewportSuppliesScrollbars() const
{
    return m_frame->isMainFrame() && m_frame->settings() && m_frame->settings()->viewportMetaEnabled();
}

AXObjectCache* FrameView::axObjectCache() const
{
    if (frame().document())
        return frame().document()->existingAXObjectCache();
    return nullptr;
}

void FrameView::setCursor(const Cursor& cursor)
{
    Page* page = frame().page();
    if (!page || !page->settings().deviceSupportsMouse())
        return;
    page->chromeClient().setCursor(cursor, m_frame->localFrameRoot());
}

void FrameView::frameRectsChanged()
{
    TRACE_EVENT0("blink", "FrameView::frameRectsChanged");
    if (layoutSizeFixedToFrameSize())
        setLayoutSizeInternal(frameRect().size());

    setNeedsUpdateViewportIntersection();
    for (const auto& child : m_children)
        child->frameRectsChanged();
}

void FrameView::setLayoutSizeInternal(const IntSize& size)
{
    if (m_layoutSize == size)
        return;

    m_layoutSize = size;

    if (Document* document = m_frame->document()) {
        if (document->isActive())
            document->notifyResizeForViewportUnits();
    }

    contentsResized();
}

void FrameView::didAddScrollbar(Scrollbar& scrollbar, ScrollbarOrientation orientation)
{
    ScrollableArea::didAddScrollbar(scrollbar, orientation);
    if (AXObjectCache* cache = axObjectCache())
        cache->handleScrollbarUpdate(this);
}

void FrameView::setTopControlsViewportAdjustment(float adjustment)
{
    m_topControlsViewportAdjustment = adjustment;
}

IntPoint FrameView::maximumScrollPosition() const
{
    // Make the same calculation as in CC's LayerImpl::MaxScrollOffset()
    // FIXME: We probably shouldn't be storing the bounds in a float. crbug.com/422331.
    IntSize visibleSize = visibleContentSize(ExcludeScrollbars) + topControlsSize();
    IntSize contentBounds = contentsSize();
    IntPoint maximumPosition = -scrollOrigin() + (contentBounds - visibleSize);
    return maximumPosition.expandedTo(minimumScrollPosition());
}

void FrameView::addChild(PassRefPtrWillBeRawPtr<Widget> prpChild)
{
    Widget* child = prpChild.get();
    ASSERT(child != this && !child->parent());
    child->setParent(this);
    m_children.add(prpChild);
}

void FrameView::setHasHorizontalScrollbar(bool hasBar)
{
    if (hasBar == !!m_horizontalScrollbar)
        return;

    if (hasBar) {
        m_horizontalScrollbar = createScrollbar(HorizontalScrollbar);
        addChild(m_horizontalScrollbar.get());
        didAddScrollbar(*m_horizontalScrollbar, HorizontalScrollbar);
        m_horizontalScrollbar->styleChanged();
    } else {
        willRemoveScrollbar(*m_horizontalScrollbar, HorizontalScrollbar);
        if (AXObjectCache* cache = axObjectCache())
            cache->remove(m_horizontalScrollbar.get());
        // If the scrollbar has been marked as overlapping the window resizer,
        // then its removal should reduce the count.
        if (m_horizontalScrollbar->overlapsResizer())
            adjustScrollbarsAvoidingResizerCount(-1);
        removeChild(m_horizontalScrollbar.get());
        m_horizontalScrollbar->disconnectFromScrollableArea();
        m_horizontalScrollbar = nullptr;
        if (AXObjectCache* cache = axObjectCache())
            cache->handleScrollbarUpdate(this);
    }

    setScrollCornerNeedsPaintInvalidation();
}

void FrameView::setHasVerticalScrollbar(bool hasBar)
{
    if (hasBar == !!m_verticalScrollbar)
        return;

    if (hasBar) {
        m_verticalScrollbar = createScrollbar(VerticalScrollbar);
        addChild(m_verticalScrollbar.get());
        didAddScrollbar(*m_verticalScrollbar, VerticalScrollbar);
        m_verticalScrollbar->styleChanged();
    } else {
        willRemoveScrollbar(*m_verticalScrollbar, VerticalScrollbar);
        if (AXObjectCache* cache = axObjectCache())
            cache->remove(m_verticalScrollbar.get());
        // If the scrollbar has been marked as overlapping the window resizer,
        // then its removal should reduce the count.
        if (m_verticalScrollbar->overlapsResizer())
            adjustScrollbarsAvoidingResizerCount(-1);
        removeChild(m_verticalScrollbar.get());
        m_verticalScrollbar->disconnectFromScrollableArea();
        m_verticalScrollbar = nullptr;
        if (AXObjectCache* cache = axObjectCache())
            cache->handleScrollbarUpdate(this);
    }

    setScrollCornerNeedsPaintInvalidation();
}

void FrameView::setScrollbarModes(ScrollbarMode horizontalMode, ScrollbarMode verticalMode,
    bool horizontalLock, bool verticalLock)
{
    bool needsUpdate = false;

    // If the page's overflow setting has disabled scrolling, do not allow anything to override that setting.
    // http://crbug.com/426447
    LayoutObject* viewport = viewportLayoutObject();
    if (viewport && !shouldIgnoreOverflowHidden()) {
        if (viewport->style()->overflowX() == OHIDDEN)
            horizontalMode = ScrollbarAlwaysOff;
        if (viewport->style()->overflowY() == OHIDDEN)
            verticalMode = ScrollbarAlwaysOff;
    }

    if (horizontalMode != horizontalScrollbarMode() && !m_horizontalScrollbarLock) {
        m_horizontalScrollbarMode = horizontalMode;
        needsUpdate = true;
    }

    if (verticalMode != verticalScrollbarMode() && !m_verticalScrollbarLock) {
        m_verticalScrollbarMode = verticalMode;
        needsUpdate = true;
    }

    if (horizontalLock)
        setHorizontalScrollbarLock();

    if (verticalLock)
        setVerticalScrollbarLock();

    if (!needsUpdate)
        return;

    updateScrollbars(scrollOffsetDouble());

    if (!layerForScrolling())
        return;
    WebLayer* layer = layerForScrolling()->platformLayer();
    if (!layer)
        return;
    layer->setUserScrollable(userInputScrollable(HorizontalScrollbar), userInputScrollable(VerticalScrollbar));
}

IntSize FrameView::visibleContentSize(IncludeScrollbarsInRect scrollbarInclusion) const
{
    return scrollbarInclusion == ExcludeScrollbars ? excludeScrollbars(frameRect().size()) : frameRect().size();
}

IntRect FrameView::visibleContentRect(IncludeScrollbarsInRect scrollbarInclusion) const
{
    return IntRect(flooredIntPoint(m_scrollPosition), visibleContentSize(scrollbarInclusion));
}

IntSize FrameView::contentsSize() const
{
    return m_contentsSize;
}

IntPoint FrameView::minimumScrollPosition() const
{
    return IntPoint(-scrollOrigin().x(), -scrollOrigin().y());
}

void FrameView::adjustScrollbarOpacity()
{
    if (m_horizontalScrollbar && layerForHorizontalScrollbar()) {
        bool isOpaqueScrollbar = !m_horizontalScrollbar->isOverlayScrollbar();
        layerForHorizontalScrollbar()->setContentsOpaque(isOpaqueScrollbar);
    }
    if (m_verticalScrollbar && layerForVerticalScrollbar()) {
        bool isOpaqueScrollbar = !m_verticalScrollbar->isOverlayScrollbar();
        layerForVerticalScrollbar()->setContentsOpaque(isOpaqueScrollbar);
    }
}

int FrameView::scrollSize(ScrollbarOrientation orientation) const
{
    Scrollbar* scrollbar = ((orientation == HorizontalScrollbar) ? m_horizontalScrollbar : m_verticalScrollbar).get();

    // If no scrollbars are present, the content may still be scrollable.
    if (!scrollbar) {
        IntSize scrollSize = m_contentsSize - visibleContentRect().size();
        scrollSize.clampNegativeToZero();
        return orientation == HorizontalScrollbar ? scrollSize.width() : scrollSize.height();
    }

    return scrollbar->totalSize() - scrollbar->visibleSize();
}

void FrameView::setScrollOffset(const IntPoint& offset, ScrollType)
{
    scrollTo(clampScrollPosition(offset));
}

void FrameView::setScrollOffset(const DoublePoint& offset, ScrollType)
{
    scrollTo(clampScrollPosition(offset));
}

void FrameView::windowResizerRectChanged()
{
    updateScrollbars(scrollOffsetDouble());
}

bool FrameView::hasOverlayScrollbars() const
{

    return (m_horizontalScrollbar && m_horizontalScrollbar->isOverlayScrollbar())
        || (m_verticalScrollbar && m_verticalScrollbar->isOverlayScrollbar());
}

void FrameView::computeScrollbarExistence(bool& newHasHorizontalScrollbar, bool& newHasVerticalScrollbar, const IntSize& docSize, ComputeScrollbarExistenceOption option) const
{
    bool hasHorizontalScrollbar = m_horizontalScrollbar;
    bool hasVerticalScrollbar = m_verticalScrollbar;

    newHasHorizontalScrollbar = hasHorizontalScrollbar;
    newHasVerticalScrollbar = hasVerticalScrollbar;

    if (m_frame->settings() && m_frame->settings()->rootLayerScrolls())
        return;

    ScrollbarMode hScroll = m_horizontalScrollbarMode;
    ScrollbarMode vScroll = m_verticalScrollbarMode;

    if (hScroll != ScrollbarAuto)
        newHasHorizontalScrollbar = (hScroll == ScrollbarAlwaysOn);
    if (vScroll != ScrollbarAuto)
        newHasVerticalScrollbar = (vScroll == ScrollbarAlwaysOn);

    if (m_scrollbarsSuppressed || (hScroll != ScrollbarAuto && vScroll != ScrollbarAuto))
        return;

    if (hScroll == ScrollbarAuto)
        newHasHorizontalScrollbar = docSize.width() > visibleWidth();
    if (vScroll == ScrollbarAuto)
        newHasVerticalScrollbar = docSize.height() > visibleHeight();

    if (hasOverlayScrollbars())
        return;

    IntSize fullVisibleSize = visibleContentRect(IncludeScrollbars).size();

    bool attemptToRemoveScrollbars = (option == FirstPass
        && docSize.width() <= fullVisibleSize.width() && docSize.height() <= fullVisibleSize.height());
    if (attemptToRemoveScrollbars) {
        if (hScroll == ScrollbarAuto)
            newHasHorizontalScrollbar = false;
        if (vScroll == ScrollbarAuto)
            newHasVerticalScrollbar = false;
    }
}

void FrameView::updateScrollbarGeometry()
{
    if (m_horizontalScrollbar) {
        int thickness = m_horizontalScrollbar->scrollbarThickness();
        int clientWidth = visibleWidth();
        IntRect oldRect(m_horizontalScrollbar->frameRect());
        IntRect hBarRect((shouldPlaceVerticalScrollbarOnLeft() && m_verticalScrollbar) ? m_verticalScrollbar->width() : 0,
            height() - thickness,
            width() - (m_verticalScrollbar ? m_verticalScrollbar->width() : 0),
            thickness);
        m_horizontalScrollbar->setFrameRect(adjustScrollbarRectForResizer(hBarRect, *m_horizontalScrollbar));
        if (oldRect != m_horizontalScrollbar->frameRect())
            setScrollbarNeedsPaintInvalidation(HorizontalScrollbar);

        m_horizontalScrollbar->setEnabled(contentsWidth() > clientWidth);
        m_horizontalScrollbar->setProportion(clientWidth, contentsWidth());
        m_horizontalScrollbar->offsetDidChange();
    }

    if (m_verticalScrollbar) {
        int thickness = m_verticalScrollbar->scrollbarThickness();
        int clientHeight = visibleHeight();
        IntRect oldRect(m_verticalScrollbar->frameRect());
        IntRect vBarRect(shouldPlaceVerticalScrollbarOnLeft() ? 0 : (width() - thickness),
            0,
            thickness,
            height() - (m_horizontalScrollbar ? m_horizontalScrollbar->height() : 0));
        m_verticalScrollbar->setFrameRect(adjustScrollbarRectForResizer(vBarRect, *m_verticalScrollbar));
        if (oldRect != m_verticalScrollbar->frameRect())
            setScrollbarNeedsPaintInvalidation(VerticalScrollbar);

        m_verticalScrollbar->setEnabled(contentsHeight() > clientHeight);
        m_verticalScrollbar->setProportion(clientHeight, contentsHeight());
        m_verticalScrollbar->offsetDidChange();
    }
}

IntRect FrameView::adjustScrollbarRectForResizer(const IntRect& rect, Scrollbar& scrollbar)
{
    // Get our window resizer rect and see if we overlap. Adjust to avoid the overlap
    // if necessary.
    IntRect adjustedRect(rect);
    bool overlapsResizer = false;
    if (!rect.isEmpty() && !windowResizerRect().isEmpty()) {
        IntRect resizerRect = convertFromRootFrame(windowResizerRect());
        if (rect.intersects(resizerRect)) {
            if (scrollbar.orientation() == HorizontalScrollbar) {
                int overlap = rect.maxX() - resizerRect.x();
                if (overlap > 0 && resizerRect.maxX() >= rect.maxX()) {
                    adjustedRect.setWidth(rect.width() - overlap);
                    overlapsResizer = true;
                }
            } else {
                int overlap = rect.maxY() - resizerRect.y();
                if (overlap > 0 && resizerRect.maxY() >= rect.maxY()) {
                    adjustedRect.setHeight(rect.height() - overlap);
                    overlapsResizer = true;
                }
            }
        }
    }
    if (overlapsResizer != scrollbar.overlapsResizer()) {
        scrollbar.setOverlapsResizer(overlapsResizer);
        adjustScrollbarsAvoidingResizerCount(overlapsResizer ? 1 : -1);
    }
    return adjustedRect;
}

bool FrameView::adjustScrollbarExistence(ComputeScrollbarExistenceOption option)
{
    ASSERT(m_inUpdateScrollbars);

    // If we came in here with the view already needing a layout, then go ahead and do that
    // first.  (This will be the common case, e.g., when the page changes due to window resizing for example).
    // This layout will not re-enter updateScrollbars and does not count towards our max layout pass total.
    if (!m_scrollbarsSuppressed)
        scrollbarExistenceDidChange();

    bool hasHorizontalScrollbar = m_horizontalScrollbar;
    bool hasVerticalScrollbar = m_verticalScrollbar;

    bool newHasHorizontalScrollbar = false;
    bool newHasVerticalScrollbar = false;
    computeScrollbarExistence(newHasHorizontalScrollbar, newHasVerticalScrollbar, contentsSize(), option);

    bool scrollbarExistenceChanged = hasHorizontalScrollbar != newHasHorizontalScrollbar || hasVerticalScrollbar != newHasVerticalScrollbar;
    if (!scrollbarExistenceChanged)
        return false;

    setHasHorizontalScrollbar(newHasHorizontalScrollbar);
    setHasVerticalScrollbar(newHasVerticalScrollbar);

    if (m_scrollbarsSuppressed)
        return true;

    if (!hasOverlayScrollbars())
        contentsResized();
    scrollbarExistenceDidChange();
    return true;
}

bool FrameView::needsScrollbarReconstruction() const
{
    Element* customScrollbarElement = nullptr;
    LocalFrame* customScrollbarFrame = nullptr;
    bool shouldUseCustom = shouldUseCustomScrollbars(customScrollbarElement, customScrollbarFrame);

    bool hasAnyScrollbar = m_horizontalScrollbar || m_verticalScrollbar;
    bool hasCustom = (m_horizontalScrollbar && m_horizontalScrollbar->isCustomScrollbar())
        || (m_verticalScrollbar && m_verticalScrollbar->isCustomScrollbar());

    return hasAnyScrollbar && (shouldUseCustom != hasCustom);
}

bool FrameView::shouldIgnoreOverflowHidden() const
{
    return m_frame->settings()->ignoreMainFrameOverflowHiddenQuirk() && m_frame->isMainFrame();
}

void FrameView::updateScrollbars(const DoubleSize& desiredOffset)
{
    if (m_frame->settings() && m_frame->settings()->rootLayerScrolls())
        return;

    // Avoid drawing two sets of scrollbars when visual viewport is enabled.
    if (visualViewportSuppliesScrollbars()) {
        setHasHorizontalScrollbar(false);
        setHasVerticalScrollbar(false);
        setScrollOffsetFromUpdateScrollbars(desiredOffset);
        return;
    }

    if (m_inUpdateScrollbars)
        return;
    InUpdateScrollbarsScope inUpdateScrollbarsScope(this);

    bool scrollbarExistenceChanged = false;

    if (needsScrollbarReconstruction()) {
        setHasHorizontalScrollbar(false);
        setHasVerticalScrollbar(false);
        scrollbarExistenceChanged = true;
    }

    int maxUpdateScrollbarsPass = hasOverlayScrollbars() || m_scrollbarsSuppressed ? 1 : 3;
    for (int updateScrollbarsPass = 0; updateScrollbarsPass < maxUpdateScrollbarsPass; updateScrollbarsPass++) {
        if (!adjustScrollbarExistence(updateScrollbarsPass ? Incremental : FirstPass))
            break;
        scrollbarExistenceChanged = true;
    }

    updateScrollbarGeometry();

    if (scrollbarExistenceChanged) {
        // FIXME: Is frameRectsChanged really necessary here? Have any frame rects changed?
        frameRectsChanged();
        positionScrollbarLayers();
        updateScrollCorner();
    }

    setScrollOffsetFromUpdateScrollbars(desiredOffset);
}

void FrameView::setScrollOffsetFromUpdateScrollbars(const DoubleSize& offset)
{
    DoublePoint adjustedScrollPosition = clampScrollPosition(DoublePoint(offset));

    if (adjustedScrollPosition != scrollPositionDouble() || scrollOriginChanged()) {
        ScrollableArea::setScrollPosition(adjustedScrollPosition, ProgrammaticScroll);
        resetScrollOriginChanged();
    }
}

IntRect FrameView::rectToCopyOnScroll() const
{
    IntRect scrollViewRect = convertToRootFrame(IntRect((shouldPlaceVerticalScrollbarOnLeft() && verticalScrollbar()) ? verticalScrollbar()->width() : 0, 0, visibleWidth(), visibleHeight()));
    if (hasOverlayScrollbars()) {
        int verticalScrollbarWidth = (verticalScrollbar() && !hasLayerForVerticalScrollbar()) ? verticalScrollbar()->width() : 0;
        int horizontalScrollbarHeight = (horizontalScrollbar() && !hasLayerForHorizontalScrollbar()) ? horizontalScrollbar()->height() : 0;

        scrollViewRect.setWidth(scrollViewRect.width() - verticalScrollbarWidth);
        scrollViewRect.setHeight(scrollViewRect.height() - horizontalScrollbarHeight);
    }
    return scrollViewRect;
}

void FrameView::scrollContentsIfNeeded()
{
    if (m_pendingScrollDelta.isZero())
        return;
    DoubleSize scrollDelta = m_pendingScrollDelta;
    m_pendingScrollDelta = DoubleSize();
    // FIXME: Change scrollContents() to take DoubleSize. crbug.com/414283.
    scrollContents(flooredIntSize(scrollDelta));
}

void FrameView::scrollContents(const IntSize& scrollDelta)
{
    HostWindow* window = hostWindow();
    if (!window)
        return;

    TRACE_EVENT0("blink", "FrameView::scrollContents");

    IntRect clipRect = windowClipRect();
    IntRect updateRect = clipRect;
    updateRect.intersect(rectToCopyOnScroll());

    if (!scrollContentsFastPath(-scrollDelta))
        scrollContentsSlowPath(updateRect);

    // This call will move children with native widgets (plugins) and invalidate them as well.
    frameRectsChanged();
}

IntPoint FrameView::contentsToFrame(const IntPoint& pointInContentSpace) const
{
    return pointInContentSpace - scrollOffset();
}

IntRect FrameView::contentsToFrame(const IntRect& rectInContentSpace) const
{
    return IntRect(contentsToFrame(rectInContentSpace.location()), rectInContentSpace.size());
}

FloatPoint FrameView::frameToContents(const FloatPoint& pointInFrame) const
{
    return pointInFrame + scrollOffset();
}

IntPoint FrameView::frameToContents(const IntPoint& pointInFrame) const
{
    return pointInFrame + scrollOffset();
}

IntRect FrameView::frameToContents(const IntRect& rectInFrame) const
{
    return IntRect(frameToContents(rectInFrame.location()), rectInFrame.size());
}

IntPoint FrameView::rootFrameToContents(const IntPoint& rootFramePoint) const
{
    IntPoint framePoint = convertFromRootFrame(rootFramePoint);
    return frameToContents(framePoint);
}

IntRect FrameView::rootFrameToContents(const IntRect& rootFrameRect) const
{
    return IntRect(rootFrameToContents(rootFrameRect.location()), rootFrameRect.size());
}

IntPoint FrameView::contentsToRootFrame(const IntPoint& contentsPoint) const
{
    IntPoint framePoint = contentsToFrame(contentsPoint);
    return convertToRootFrame(framePoint);
}

IntRect FrameView::contentsToRootFrame(const IntRect& contentsRect) const
{
    IntRect rectInFrame = contentsToFrame(contentsRect);
    return convertToRootFrame(rectInFrame);
}

FloatPoint FrameView::rootFrameToContents(const FloatPoint& pointInRootFrame) const
{
    FloatPoint framePoint = convertFromRootFrame(pointInRootFrame);
    return frameToContents(framePoint);
}

IntRect FrameView::viewportToContents(const IntRect& rectInViewport) const
{
    IntRect rectInRootFrame = m_frame->host()->visualViewport().viewportToRootFrame(rectInViewport);
    IntRect frameRect = convertFromRootFrame(rectInRootFrame);
    return frameToContents(frameRect);
}

IntPoint FrameView::viewportToContents(const IntPoint& pointInViewport) const
{
    IntPoint pointInRootFrame = m_frame->host()->visualViewport().viewportToRootFrame(pointInViewport);
    IntPoint pointInFrame = convertFromRootFrame(pointInRootFrame);
    return frameToContents(pointInFrame);
}

IntRect FrameView::contentsToViewport(const IntRect& rectInContents) const
{
    IntRect rectInFrame = contentsToFrame(rectInContents);
    IntRect rectInRootFrame = convertToRootFrame(rectInFrame);
    return m_frame->host()->visualViewport().rootFrameToViewport(rectInRootFrame);
}

IntPoint FrameView::contentsToViewport(const IntPoint& pointInContents) const
{
    IntPoint pointInFrame = contentsToFrame(pointInContents);
    IntPoint pointInRootFrame = convertToRootFrame(pointInFrame);
    return m_frame->host()->visualViewport().rootFrameToViewport(pointInRootFrame);
}

IntRect FrameView::contentsToScreen(const IntRect& rect) const
{
    HostWindow* window = hostWindow();
    if (!window)
        return IntRect();
    return window->viewportToScreen(contentsToViewport(rect));
}

IntRect FrameView::soonToBeRemovedContentsToUnscaledViewport(const IntRect& rectInContents) const
{
    IntRect rectInFrame = contentsToFrame(rectInContents);
    IntRect rectInRootFrame = convertToRootFrame(rectInFrame);
    return enclosingIntRect(m_frame->host()->visualViewport().mainViewToViewportCSSPixels(rectInRootFrame));
}

IntPoint FrameView::soonToBeRemovedUnscaledViewportToContents(const IntPoint& pointInViewport) const
{
    IntPoint pointInRootFrame = flooredIntPoint(m_frame->host()->visualViewport().viewportCSSPixelsToRootFrame(pointInViewport));
    IntPoint pointInThisFrame = convertFromRootFrame(pointInRootFrame);
    return frameToContents(pointInThisFrame);
}

bool FrameView::containsScrollbarsAvoidingResizer() const
{
    return !m_scrollbarsAvoidingResizer;
}

void FrameView::adjustScrollbarsAvoidingResizerCount(int overlapDelta)
{
    int oldCount = m_scrollbarsAvoidingResizer;
    m_scrollbarsAvoidingResizer += overlapDelta;
    if (parent()) {
        toFrameView(parent())->adjustScrollbarsAvoidingResizerCount(overlapDelta);
    } else if (!scrollbarsSuppressed()) {
        // If we went from n to 0 or from 0 to n and we're the outermost view,
        // we need to invalidate the windowResizerRect(), since it will now need to paint
        // differently.
        if ((oldCount > 0 && m_scrollbarsAvoidingResizer == 0)
            || (oldCount == 0 && m_scrollbarsAvoidingResizer > 0))
            invalidateRect(windowResizerRect());
    }
}

Scrollbar* FrameView::scrollbarAtFramePoint(const IntPoint& pointInFrame)
{
    if (m_horizontalScrollbar && m_horizontalScrollbar->shouldParticipateInHitTesting() && m_horizontalScrollbar->frameRect().contains(pointInFrame))
        return m_horizontalScrollbar.get();
    if (m_verticalScrollbar && m_verticalScrollbar->shouldParticipateInHitTesting() && m_verticalScrollbar->frameRect().contains(pointInFrame))
        return m_verticalScrollbar.get();
    return nullptr;
}

static void positionScrollbarLayer(GraphicsLayer* graphicsLayer, Scrollbar* scrollbar)
{
    if (!graphicsLayer || !scrollbar)
        return;

    IntRect scrollbarRect = scrollbar->frameRect();
    graphicsLayer->setPosition(scrollbarRect.location());

    if (scrollbarRect.size() == graphicsLayer->size())
        return;

    graphicsLayer->setSize(FloatSize(scrollbarRect.size()));

    if (graphicsLayer->hasContentsLayer()) {
        graphicsLayer->setContentsRect(IntRect(0, 0, scrollbarRect.width(), scrollbarRect.height()));
        return;
    }

    graphicsLayer->setDrawsContent(true);
    graphicsLayer->setNeedsDisplay();
}

static void positionScrollCornerLayer(GraphicsLayer* graphicsLayer, const IntRect& cornerRect)
{
    if (!graphicsLayer)
        return;
    graphicsLayer->setDrawsContent(!cornerRect.isEmpty());
    graphicsLayer->setPosition(cornerRect.location());
    if (cornerRect.size() != graphicsLayer->size())
        graphicsLayer->setNeedsDisplay();
    graphicsLayer->setSize(FloatSize(cornerRect.size()));
}

void FrameView::positionScrollbarLayers()
{
    positionScrollbarLayer(layerForHorizontalScrollbar(), horizontalScrollbar());
    positionScrollbarLayer(layerForVerticalScrollbar(), verticalScrollbar());
    positionScrollCornerLayer(layerForScrollCorner(), scrollCornerRect());
}

bool FrameView::userInputScrollable(ScrollbarOrientation orientation) const
{
    Document* document = frame().document();
    Element* fullscreenElement = Fullscreen::fullscreenElementFrom(*document);
    if (fullscreenElement && fullscreenElement != document->documentElement())
        return false;

    if (frame().settings() && frame().settings()->rootLayerScrolls())
        return false;

    ScrollbarMode mode = (orientation == HorizontalScrollbar) ?
        m_horizontalScrollbarMode : m_verticalScrollbarMode;

    return mode == ScrollbarAuto || mode == ScrollbarAlwaysOn;
}

bool FrameView::shouldPlaceVerticalScrollbarOnLeft() const
{
    return false;
}

Widget* FrameView::widget()
{
    return this;
}

LayoutRect FrameView::scrollIntoView(const LayoutRect& rectInContent, const ScrollAlignment& alignX, const ScrollAlignment& alignY, ScrollType scrollType)
{
    LayoutRect viewRect(visibleContentRect());
    LayoutRect exposeRect = ScrollAlignment::getRectToExpose(viewRect, rectInContent, alignX, alignY);

    double xOffset = exposeRect.x();
    double yOffset = exposeRect.y();

    setScrollPosition(DoublePoint(xOffset, yOffset), scrollType);

    // Scrolling the FrameView cannot change the input rect's location relative to the document.
    return rectInContent;
}

IntRect FrameView::scrollCornerRect() const
{
    IntRect cornerRect;

    if (hasOverlayScrollbars())
        return cornerRect;

    if (m_horizontalScrollbar && width() - m_horizontalScrollbar->width() > 0) {
        cornerRect.unite(IntRect(shouldPlaceVerticalScrollbarOnLeft() ? 0 : m_horizontalScrollbar->width(),
            height() - m_horizontalScrollbar->height(),
            width() - m_horizontalScrollbar->width(),
            m_horizontalScrollbar->height()));
    }

    if (m_verticalScrollbar && height() - m_verticalScrollbar->height() > 0) {
        cornerRect.unite(IntRect(shouldPlaceVerticalScrollbarOnLeft() ? 0 : (width() - m_verticalScrollbar->width()),
            m_verticalScrollbar->height(),
            m_verticalScrollbar->width(),
            height() - m_verticalScrollbar->height()));
    }

    return cornerRect;
}

bool FrameView::isScrollCornerVisible() const
{
    return !scrollCornerRect().isEmpty();
}

ScrollBehavior FrameView::scrollBehaviorStyle() const
{
    Element* scrollElement = m_frame->document()->scrollingElement();
    LayoutObject* layoutObject = scrollElement ? scrollElement->layoutObject() : nullptr;
    if (layoutObject && layoutObject->style()->scrollBehavior() == ScrollBehaviorSmooth)
        return ScrollBehaviorSmooth;

    return ScrollBehaviorInstant;
}

void FrameView::paint(GraphicsContext& context, const CullRect& cullRect) const
{
    paint(context, GlobalPaintNormalPhase, cullRect);
}

void FrameView::paint(GraphicsContext& context, const GlobalPaintFlags globalPaintFlags, const CullRect& cullRect) const
{
    // TODO(skyostil): Remove this early-out in favor of painting cached scrollbars.
    if (shouldThrottleRendering())
        return;
    FramePainter(*this).paint(context, globalPaintFlags, cullRect);
}

void FrameView::paintContents(GraphicsContext& context, const GlobalPaintFlags globalPaintFlags, const IntRect& damageRect) const
{
    if (shouldThrottleRendering())
        return;
    FramePainter(*this).paintContents(context, globalPaintFlags, damageRect);
}

bool FrameView::isPointInScrollbarCorner(const IntPoint& pointInRootFrame)
{
    if (!scrollbarCornerPresent())
        return false;

    IntPoint framePoint = convertFromRootFrame(pointInRootFrame);

    if (m_horizontalScrollbar) {
        int horizontalScrollbarYMin = m_horizontalScrollbar->frameRect().y();
        int horizontalScrollbarYMax = m_horizontalScrollbar->frameRect().y() + m_horizontalScrollbar->frameRect().height();
        int horizontalScrollbarXMin = m_horizontalScrollbar->frameRect().x() + m_horizontalScrollbar->frameRect().width();

        return framePoint.y() > horizontalScrollbarYMin && framePoint.y() < horizontalScrollbarYMax && framePoint.x() > horizontalScrollbarXMin;
    }

    int verticalScrollbarXMin = m_verticalScrollbar->frameRect().x();
    int verticalScrollbarXMax = m_verticalScrollbar->frameRect().x() + m_verticalScrollbar->frameRect().width();
    int verticalScrollbarYMin = m_verticalScrollbar->frameRect().y() + m_verticalScrollbar->frameRect().height();

    return framePoint.x() > verticalScrollbarXMin && framePoint.x() < verticalScrollbarXMax && framePoint.y() > verticalScrollbarYMin;
}

bool FrameView::scrollbarCornerPresent() const
{
    return (m_horizontalScrollbar && width() - m_horizontalScrollbar->width() > 0)
        || (m_verticalScrollbar && height() - m_verticalScrollbar->height() > 0);
}

IntRect FrameView::convertFromScrollbarToContainingWidget(const Scrollbar& scrollbar, const IntRect& localRect) const
{
    // Scrollbars won't be transformed within us
    IntRect newRect = localRect;
    newRect.moveBy(scrollbar.location());
    return newRect;
}

IntRect FrameView::convertFromContainingWidgetToScrollbar(const Scrollbar& scrollbar, const IntRect& parentRect) const
{
    IntRect newRect = parentRect;
    // Scrollbars won't be transformed within us
    newRect.moveBy(-scrollbar.location());
    return newRect;
}

// FIXME: test these on windows
IntPoint FrameView::convertFromScrollbarToContainingWidget(const Scrollbar& scrollbar, const IntPoint& localPoint) const
{
    // Scrollbars won't be transformed within us
    IntPoint newPoint = localPoint;
    newPoint.moveBy(scrollbar.location());
    return newPoint;
}

IntPoint FrameView::convertFromContainingWidgetToScrollbar(const Scrollbar& scrollbar, const IntPoint& parentPoint) const
{
    IntPoint newPoint = parentPoint;
    // Scrollbars won't be transformed within us
    newPoint.moveBy(-scrollbar.location());
    return newPoint;
}

void FrameView::setParentVisible(bool visible)
{
    if (isParentVisible() == visible)
        return;

    Widget::setParentVisible(visible);

    if (!isSelfVisible())
        return;

    for (const auto& child : m_children)
        child->setParentVisible(visible);
}

void FrameView::show()
{
    if (!isSelfVisible()) {
        setSelfVisible(true);
        updateScrollableAreaSet();
        if (isParentVisible()) {
            for (const auto& child : m_children)
                child->setParentVisible(true);
        }
    }

    Widget::show();
}

void FrameView::hide()
{
    if (isSelfVisible()) {
        if (isParentVisible()) {
            for (const auto& child : m_children)
                child->setParentVisible(false);
        }
        setSelfVisible(false);
        updateScrollableAreaSet();
    }

    Widget::hide();
}

void FrameView::setScrollOrigin(const IntPoint& origin, bool updatePositionAtAll, bool updatePositionSynchronously)
{
    if (scrollOrigin() == origin)
        return;

    ScrollableArea::setScrollOrigin(origin);

    // Update if the scroll origin changes, since our position will be different if the content size did not change.
    if (updatePositionAtAll && updatePositionSynchronously)
        updateScrollbars(scrollOffsetDouble());
}

int FrameView::viewportWidth() const
{
    int viewportWidth = layoutSize(IncludeScrollbars).width();
    return adjustForAbsoluteZoom(viewportWidth, layoutView());
}

ScrollableArea* FrameView::scrollableArea()
{
    if (m_viewportScrollableArea)
        return m_viewportScrollableArea.get();

    return layoutViewportScrollableArea();
}

ScrollableArea* FrameView::layoutViewportScrollableArea()
{
    Settings* settings = frame().settings();
    if (!settings || !settings->rootLayerScrolls())
        return this;

    LayoutView* layoutView = this->layoutView();
    return layoutView ? layoutView->scrollableArea() : nullptr;
}

LayoutObject* FrameView::viewportLayoutObject()
{
    if (Document* document = frame().document()) {
        if (Element* element = document->viewportDefiningElement())
            return element->layoutObject();
    }
    return nullptr;
}

void FrameView::collectAnnotatedRegions(LayoutObject& layoutObject, Vector<AnnotatedRegionValue>& regions) const
{
    // LayoutTexts don't have their own style, they just use their parent's style,
    // so we don't want to include them.
    if (layoutObject.isText())
        return;

    layoutObject.addAnnotatedRegions(regions);
    for (LayoutObject* curr = layoutObject.slowFirstChild(); curr; curr = curr->nextSibling())
        collectAnnotatedRegions(*curr, regions);
}

void FrameView::collectFrameTimingRequestsRecursive(GraphicsLayerFrameTimingRequests& graphicsLayerTimingRequests)
{
    if (!m_frameTimingRequestsDirty)
        return;

    collectFrameTimingRequests(graphicsLayerTimingRequests);

    for (Frame* child = m_frame->tree().firstChild(); child; child = child->tree().nextSibling()) {
        if (!child->isLocalFrame())
            continue;

        toLocalFrame(child)->view()->collectFrameTimingRequestsRecursive(graphicsLayerTimingRequests);
    }
    m_frameTimingRequestsDirty = false;
}

void FrameView::collectFrameTimingRequests(GraphicsLayerFrameTimingRequests& graphicsLayerTimingRequests)
{
    if (!m_frame->isLocalFrame())
        return;
    Frame* frame = m_frame.get();
    LocalFrame* localFrame = toLocalFrame(frame);
    LayoutRect viewRect = localFrame->contentLayoutObject()->viewRect();
    const LayoutBoxModelObject& paintInvalidationContainer = localFrame->contentLayoutObject()->containerForPaintInvalidation();
    // If the frame is being throttled, its compositing state may not be up to date.
    if (!paintInvalidationContainer.enclosingLayer()->isAllowedToQueryCompositingState())
        return;
    const GraphicsLayer* graphicsLayer = paintInvalidationContainer.enclosingLayer()->graphicsLayerBacking();

    if (!graphicsLayer)
        return;

    PaintLayer::mapRectToPaintInvalidationBacking(localFrame->contentLayoutObject(), &paintInvalidationContainer, viewRect);

    graphicsLayerTimingRequests.add(graphicsLayer, Vector<std::pair<int64_t, WebRect>>()).storedValue->value.append(std::make_pair(m_frame->frameID(), enclosingIntRect(viewRect)));
}

void FrameView::setNeedsUpdateViewportIntersection()
{
    m_needsUpdateViewportIntersection = true;
    for (FrameView* parent = parentFrameView(); parent; parent = parent->parentFrameView())
        parent->m_needsUpdateViewportIntersectionInSubtree = true;
}

void FrameView::updateViewportIntersectionIfNeeded()
{
    if (!m_needsUpdateViewportIntersection)
        return;
    m_needsUpdateViewportIntersection = false;
    m_viewportIntersectionValid = true;
    FrameView* parent = parentFrameView();
    if (!parent) {
        m_viewportIntersection = frameRect();
        return;
    }
    ASSERT(!parent->m_needsUpdateViewportIntersection);

    // If our parent is hidden, then we are too.
    if (parent->m_viewportIntersection.isEmpty()) {
        m_viewportIntersection = parent->m_viewportIntersection;
        return;
    }

    // Transform our bounds into the root frame's content coordinate space,
    // making sure we have valid layout data in our parent document. If our
    // parent is throttled, we'll use possible stale layout information and
    // rely on the fact that another lifecycle update will be scheduled once
    // our parent becomes unthrottled.
    ASSERT(parent->lifecycle().state() >= DocumentLifecycle::LayoutClean || parent->shouldThrottleRendering());
    m_viewportIntersection = parent->contentsToRootFrame(frameRect());

    // TODO(skyostil): Expand the viewport to make it less likely to see stale content while scrolling.
    IntRect viewport = parent->m_viewportIntersection;
    m_viewportIntersection.intersect(viewport);
}

void FrameView::updateViewportIntersectionsForSubtree(LifeCycleUpdateOption phases)
{
    bool hadValidIntersection = m_viewportIntersectionValid;
    bool hadEmptyIntersection = m_viewportIntersection.isEmpty();
    updateViewportIntersectionIfNeeded();

    // Notify javascript IntersectionObservers
    if (phases == AllPhases && frame().document()->intersectionObserverController())
        frame().document()->intersectionObserverController()->computeTrackedIntersectionObservations();

    // Adjust render throttling for iframes based on visibility
    bool shouldNotify = !hadValidIntersection || hadEmptyIntersection != m_viewportIntersection.isEmpty();
    if (shouldNotify && !m_renderThrottlingObserverNotificationFactory->isPending())
        m_frame->frameScheduler()->timerTaskRunner()->postTask(BLINK_FROM_HERE, m_renderThrottlingObserverNotificationFactory->cancelAndCreate());

    if (!m_needsUpdateViewportIntersectionInSubtree)
        return;
    m_needsUpdateViewportIntersectionInSubtree = false;

    for (Frame* child = m_frame->tree().firstChild(); child; child = child->tree().nextSibling()) {
        if (!child->isLocalFrame())
            continue;
        if (FrameView* view = toLocalFrame(child)->view())
            view->updateViewportIntersectionsForSubtree(phases);
    }
}

void FrameView::notifyRenderThrottlingObservers()
{
    TRACE_EVENT0("blink", "FrameView::notifyRenderThrottlingObservers");
    ASSERT(!isInPerformLayout());
    ASSERT(!m_frame->document()->inStyleRecalc());
    bool wasThrottled = canThrottleRendering();

    // Only offscreen frames can be throttled.
    m_hiddenForThrottling = m_viewportIntersectionValid && m_viewportIntersection.isEmpty();

    // We only throttle the rendering pipeline in cross-origin frames. This is
    // to avoid a situation where an ancestor frame directly depends on the
    // pipeline timing of a descendant and breaks as a result of throttling.
    // The rationale is that cross-origin frames must already communicate with
    // asynchronous messages, so they should be able to tolerate some delay in
    // receiving replies from a throttled peer.
    //
    // Check if we can access our parent's security origin.
    m_crossOriginForThrottling = false;
    const SecurityOrigin* origin = frame().securityContext()->securityOrigin();
    for (Frame* parentFrame = m_frame->tree().parent(); parentFrame; parentFrame = parentFrame->tree().parent()) {
        const SecurityOrigin* parentOrigin = parentFrame->securityContext()->securityOrigin();
        if (!origin->canAccess(parentOrigin)) {
            m_crossOriginForThrottling = true;
            break;
        }
    }

    bool becameUnthrottled = wasThrottled && !canThrottleRendering();
    if (becameUnthrottled)
        page()->animator().scheduleVisualUpdate(m_frame.get());
}

bool FrameView::shouldThrottleRendering() const
{
    return canThrottleRendering() && lifecycle().throttlingAllowed();
}

bool FrameView::canThrottleRendering() const
{
    if (!RuntimeEnabledFeatures::renderingPipelineThrottlingEnabled())
        return false;
    return m_hiddenForThrottling && m_crossOriginForThrottling;
}

LayoutBox& FrameView::boxForScrollControlPaintInvalidation() const
{
    ASSERT(layoutView());
    return *layoutView();
}

} // namespace blink
