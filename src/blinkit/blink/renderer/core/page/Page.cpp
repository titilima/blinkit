// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: Page.cpp
// Description: Page Class
//      Author: Ziming Li
//     Created: 2021-07-27
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013 Apple Inc. All Rights Reserved.
 * Copyright (C) 2008 Torch Mobile Inc. All rights reserved. (http://www.torchmobile.com/)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
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

#include "core/page/Page.h"

#include "core/css/resolver/ViewportStyleResolver.h"
#include "core/dom/ClientRectList.h"
// BKTODO: #include "core/dom/VisitedLinkState.h"
#include "core/editing/DragCaretController.h"
#include "core/editing/commands/UndoStack.h"
#include "core/editing/markers/DocumentMarkerController.h"
#include "core/events/Event.h"
// BKTODO: #include "core/fetch/MemoryCache.h"
#include "core/fetch/ResourceFetcher.h"
#include "core/frame/DOMTimer.h"
#include "core/frame/FrameConsole.h"
#include "core/frame/FrameHost.h"
#include "core/frame/FrameView.h"
// BKTODO: #include "core/frame/RemoteFrame.h"
// BKTODO: #include "core/frame/RemoteFrameView.h"
#include "core/frame/Settings.h"
// BKTODO: #include "core/html/HTMLMediaElement.h"
#include "core/inspector/InspectorInstrumentation.h"
#include "core/layout/LayoutView.h"
#include "core/layout/TextAutosizer.h"
#include "core/page/AutoscrollController.h"
#include "core/page/ChromeClient.h"
#include "core/page/ContextMenuController.h"
#include "core/page/DragController.h"
#include "core/page/FocusController.h"
#include "core/page/PointerLockController.h"
#include "core/page/ValidationMessageClient.h"
#include "core/page/scrolling/ScrollingCoordinator.h"
#include "core/paint/PaintLayer.h"
#include "platform/graphics/GraphicsLayer.h"
// BKTODO: #include "platform/plugins/PluginData.h"
#include "public/platform/Platform.h"

namespace blink {

#if 0 // BKTODO:
// Set of all live pages; includes internal Page objects that are
// not observable from scripts.
static Page::PageSet& allPages()
{
    DEFINE_STATIC_LOCAL(Page::PageSet, allPages, ());
    return allPages;
}

Page::PageSet& Page::ordinaryPages()
{
    DEFINE_STATIC_LOCAL(Page::PageSet, ordinaryPages, ());
    return ordinaryPages;
}
#endif

void Page::networkStateChanged(bool online)
{
    ASSERT(false); // BKTODO:
#if 0
    WillBeHeapVector<RefPtrWillBeMember<LocalFrame>> frames;

    // Get all the frames of all the pages in all the page groups
    for (Page* page : allPages()) {
        for (Frame* frame = page->mainFrame(); frame; frame = frame->tree().traverseNext()) {
            // FIXME: There is currently no way to dispatch events to out-of-process frames.
            if (frame->isLocalFrame())
                frames.append(toLocalFrame(frame));
        }
    }

    AtomicString eventName = online ? EventTypeNames::online : EventTypeNames::offline;
    for (const auto& frame : frames) {
        frame->domWindow()->dispatchEvent(Event::create(eventName));
        InspectorInstrumentation::networkStateChanged(frame.get(), online);
    }
#endif
}

void Page::onMemoryPressure()
{
    ASSERT(false); // BKTODO:
#if 0
    for (Page* page : ordinaryPages())
        page->memoryPurgeController().purgeMemory();
#endif
}

float deviceScaleFactor(LocalFrame* frame)
{
    if (!frame)
        return 1;
    Page* page = frame->page();
    if (!page)
        return 1;
    return page->deviceScaleFactor();
}

#if 0 // BKTODO:
PassOwnPtrWillBeRawPtr<Page> Page::createOrdinary(PageClients& pageClients)
{
    OwnPtrWillBeRawPtr<Page> page = create(pageClients);
    ASSERT(false); // BKTODO:
#if 0
    ordinaryPages().add(page.get());
    page->memoryPurgeController().registerClient(page.get());
#endif
    return page.release();
}
#endif

Page::Page(PageClients& pageClients)
    : m_animator(PageAnimator::create(*this))
    , m_autoscrollController(AutoscrollController::create(*this))
    , m_chromeClient(pageClients.chromeClient)
    , m_dragCaretController(DragCaretController::create())
    , m_dragController(DragController::create(this, pageClients.dragClient))
    , m_focusController(FocusController::create(this))
    , m_contextMenuController(ContextMenuController::create(this, pageClients.contextMenuClient))
    , m_pointerLockController(PointerLockController::create(this))
    , m_undoStack(UndoStack::create())
    , m_mainFrame(nullptr)
    , m_editorClient(pageClients.editorClient)
    // BKTODO: , m_spellCheckerClient(pageClients.spellCheckerClient)
    , m_openedByDOM(false)
    , m_tabKeyCyclesThroughElements(true)
    , m_defersLoading(false)
    , m_deviceScaleFactor(1)
    , m_visibilityState(PageVisibilityStateVisible)
    , m_isCursorVisible(true)
#if ENABLE(ASSERT)
    , m_isPainting(false)
#endif
    , m_frameHost(FrameHost::create(*this))
{
    ASSERT(m_editorClient);

#if 0 // BKTODO:
    ASSERT(!allPages().contains(this));
    allPages().add(this);
#endif
}

Page::~Page()
{
#if !ENABLE(OILPAN)
    ASSERT(!ordinaryPages().contains(this));
#endif
    // willBeDestroyed() must be called before Page destruction.
    ASSERT(!m_mainFrame);
}

ViewportDescription Page::viewportDescription() const
{
    return mainFrame() && mainFrame()->isLocalFrame() && deprecatedLocalMainFrame()->document() ? deprecatedLocalMainFrame()->document()->viewportDescription() : ViewportDescription();
}

ScrollingCoordinator* Page::scrollingCoordinator()
{
    if (!m_scrollingCoordinator && Settings::acceleratedCompositingEnabled())
        m_scrollingCoordinator = ScrollingCoordinator::create(this);

    return m_scrollingCoordinator.get();
}

#if 0 // BKTODO:
MemoryPurgeController& Page::memoryPurgeController()
{
    if (!m_memoryPurgeController)
        m_memoryPurgeController = MemoryPurgeController::create();

    return *m_memoryPurgeController;
}
#endif

String Page::mainThreadScrollingReasonsAsText()
{
    if (ScrollingCoordinator* scrollingCoordinator = this->scrollingCoordinator())
        return scrollingCoordinator->mainThreadScrollingReasonsAsText();

    return String();
}

ClientRectList* Page::nonFastScrollableRects(const LocalFrame* frame)
{
    if (ScrollingCoordinator* scrollingCoordinator = this->scrollingCoordinator()) {
        // Hits in compositing/iframes/iframe-composited-scrolling.html
        DisableCompositingQueryAsserts disabler;
        scrollingCoordinator->updateAfterCompositingChangeIfNeeded();
    }

    if (!frame->view()->layerForScrolling())
        return ClientRectList::create();

    // Now retain non-fast scrollable regions
    ASSERT(false); // BKTODO: return ClientRectList::create(frame->view()->layerForScrolling()->platformLayer()->nonFastScrollableRegion());
    return nullptr;
}

void Page::setMainFrame(Frame* mainFrame)
{
    // Should only be called during initialization or swaps between local and
    // remote frames.
    // FIXME: Unfortunately we can't assert on this at the moment, because this
    // is called in the base constructor for both LocalFrame and RemoteFrame,
    // when the vtables for the derived classes have not yet been setup.
    m_mainFrame = mainFrame;
}

void Page::documentDetached(Document* document)
{
    m_multisamplingChangedObservers.clear();
    m_pointerLockController->documentDetached(document);
    m_contextMenuController->documentDetached(document);
    if (m_validationMessageClient)
        m_validationMessageClient->documentDetached(*document);
    // BKTODO: m_originsUsingFeatures.documentDetached(*document);
}

bool Page::openedByDOM() const
{
    return m_openedByDOM;
}

void Page::setOpenedByDOM()
{
    m_openedByDOM = true;
}

void Page::platformColorsChanged()
{
    ASSERT(false); // BKTODO:
#if 0
    for (const Page* page : allPages())
        for (Frame* frame = page->mainFrame(); frame; frame = frame->tree().traverseNext()) {
            if (frame->isLocalFrame())
                toLocalFrame(frame)->document()->platformColorsChanged();
        }
#endif
}

void Page::setNeedsRecalcStyleInAllFrames()
{
#if 0 // BKTODO:
    for (Frame* frame = mainFrame(); frame; frame = frame->tree().traverseNext()) {
        if (frame->isLocalFrame())
            toLocalFrame(frame)->document()->styleResolverChanged();
    }
#else
    if (Frame *frame = mainFrame())
        ASSERT(false); // BKTODO:
#endif
}

void Page::setNeedsLayoutInAllFrames()
{
    ASSERT(false); // BKTODO:
#if 0
    for (Frame* frame = mainFrame(); frame; frame = frame->tree().traverseNext()) {
        if (!frame->isLocalFrame())
            continue;
        if (FrameView* view = toLocalFrame(frame)->view()) {
            view->setNeedsLayout();
            view->scheduleRelayout();
        }
    }
#endif
}

#if 0 // BKTODO:
void Page::refreshPlugins()
{
    if (allPages().isEmpty())
        return;

    PluginData::refresh();

    for (const Page* page : allPages()) {
        // Clear out the page's plugin data.
        if (page->m_pluginData)
            page->m_pluginData = nullptr;
    }
}

PluginData* Page::pluginData() const
{
    if (!mainFrame()->isLocalFrame()
        || !deprecatedLocalMainFrame()->loader().allowPlugins(NotAboutToInstantiatePlugin))
        return nullptr;
    if (!m_pluginData)
        m_pluginData = PluginData::create(this);
    return m_pluginData.get();
}
#endif

void Page::unmarkAllTextMatches()
{
    if (!mainFrame())
        return;

    ASSERT(false); // BKTODO:
#if 0
    Frame* frame = mainFrame();
    do {
        if (frame->isLocalFrame())
            toLocalFrame(frame)->document()->markers().removeMarkers(DocumentMarker::TextMatch);
        frame = frame->tree().traverseNextWithWrap(false);
    } while (frame);
#endif
}

void Page::setValidationMessageClient(PassOwnPtrWillBeRawPtr<ValidationMessageClient> client)
{
    m_validationMessageClient = client;
}

void Page::setDefersLoading(bool defers)
{
    if (defers == m_defersLoading)
        return;

    m_defersLoading = defers;
    ASSERT(false); // BKTODO:
#if 0
    for (Frame* frame = mainFrame(); frame; frame = frame->tree().traverseNext()) {
        if (frame->isLocalFrame())
            toLocalFrame(frame)->loader().setDefersLoading(defers);
    }
#endif
}

void Page::setPageScaleFactor(float scale)
{
    frameHost().visualViewport().setScale(scale);
}

float Page::pageScaleFactor() const
{
    return frameHost().visualViewport().scale();
}

void Page::setDeviceScaleFactor(float scaleFactor)
{
    if (m_deviceScaleFactor == scaleFactor)
        return;

    m_deviceScaleFactor = scaleFactor;
    setNeedsRecalcStyleInAllFrames();

    if (mainFrame() && mainFrame()->isLocalFrame())
        deprecatedLocalMainFrame()->deviceScaleFactorChanged();
}

void Page::setDeviceColorProfile(const Vector<char>& profile)
{
    // FIXME: implement.
}

#if 0 // BKTODO:
void Page::resetDeviceColorProfileForTesting()
{
    RuntimeEnabledFeatures::setImageColorProfilesEnabled(false);
}

void Page::allVisitedStateChanged(bool invalidateVisitedLinkHashes)
{
    for (const Page* page : ordinaryPages()) {
        for (Frame* frame = page->m_mainFrame; frame; frame = frame->tree().traverseNext()) {
            if (frame->isLocalFrame())
                toLocalFrame(frame)->document()->visitedLinkState().invalidateStyleForAllLinks(invalidateVisitedLinkHashes);
        }
    }
}

void Page::visitedStateChanged(LinkHash linkHash)
{
    for (const Page* page : ordinaryPages()) {
        for (Frame* frame = page->m_mainFrame; frame; frame = frame->tree().traverseNext()) {
            if (frame->isLocalFrame())
                toLocalFrame(frame)->document()->visitedLinkState().invalidateStyleForLink(linkHash);
        }
    }
}
#endif

void Page::setVisibilityState(PageVisibilityState visibilityState, bool isInitialState)
{
    if (m_visibilityState == visibilityState)
        return;
    m_visibilityState = visibilityState;

#if 0 // BKTODO:
    if (!isInitialState)
        notifyPageVisibilityChanged();
#endif

    if (!isInitialState && m_mainFrame && m_mainFrame->isLocalFrame())
        deprecatedLocalMainFrame()->didChangeVisibilityState();
}

PageVisibilityState Page::visibilityState() const
{
    return m_visibilityState;
}

bool Page::isCursorVisible() const
{
    return m_isCursorVisible && Settings::deviceSupportsMouse();
}

void Page::addMultisamplingChangedObserver(MultisamplingChangedObserver* observer)
{
    m_multisamplingChangedObservers.add(observer);
}

// For Oilpan, unregistration is handled by the GC and weak references.
#if !ENABLE(OILPAN)
void Page::removeMultisamplingChangedObserver(MultisamplingChangedObserver* observer)
{
    m_multisamplingChangedObservers.remove(observer);
}
#endif

#if 0 // BKTODO:
void Page::settingsChanged(SettingsDelegate::ChangeType changeType)
{
    switch (changeType) {
    case SettingsDelegate::StyleChange:
        setNeedsRecalcStyleInAllFrames();
        break;
    case SettingsDelegate::ViewportDescriptionChange:
        if (mainFrame() && mainFrame()->isLocalFrame())
            deprecatedLocalMainFrame()->document()->updateViewportDescription();
        break;
    case SettingsDelegate::DNSPrefetchingChange:
        for (Frame* frame = mainFrame(); frame; frame = frame->tree().traverseNext()) {
            if (frame->isLocalFrame())
                toLocalFrame(frame)->document()->initDNSPrefetch();
        }
        break;
    case SettingsDelegate::MultisamplingChange: {
        for (MultisamplingChangedObserver* observer : m_multisamplingChangedObservers)
            observer->multisamplingChanged(m_settings->openGLMultisamplingEnabled());
        break;
    }
    case SettingsDelegate::ImageLoadingChange:
        for (Frame* frame = mainFrame(); frame; frame = frame->tree().traverseNext()) {
            if (frame->isLocalFrame()) {
                toLocalFrame(frame)->document()->fetcher()->setImagesEnabled(settings().imagesEnabled());
                toLocalFrame(frame)->document()->fetcher()->setAutoLoadImages(settings().loadsImagesAutomatically());
            }
        }
        break;
    case SettingsDelegate::TextAutosizingChange:
        if (!mainFrame() || !mainFrame()->isLocalFrame())
            break;
        if (TextAutosizer* textAutosizer = deprecatedLocalMainFrame()->document()->textAutosizer())
            textAutosizer->updatePageInfoInAllFrames();
        break;
    case SettingsDelegate::FontFamilyChange:
        for (Frame* frame = mainFrame(); frame; frame = frame->tree().traverseNext()) {
            if (frame->isLocalFrame())
                toLocalFrame(frame)->document()->styleEngine().updateGenericFontFamilySettings();
        }
        setNeedsRecalcStyleInAllFrames();
        break;
    case SettingsDelegate::AcceleratedCompositingChange:
        updateAcceleratedCompositingSettings();
        break;
    case SettingsDelegate::MediaQueryChange:
        for (Frame* frame = mainFrame(); frame; frame = frame->tree().traverseNext()) {
            if (frame->isLocalFrame())
                toLocalFrame(frame)->document()->mediaQueryAffectingValueChanged();
        }
        break;
    case SettingsDelegate::AccessibilityStateChange:
        if (!mainFrame() || !mainFrame()->isLocalFrame())
            break;
        deprecatedLocalMainFrame()->document()->axObjectCacheOwner().clearAXObjectCache();
        break;
    case SettingsDelegate::ViewportRuleChange:
        {
            if (!mainFrame() || !mainFrame()->isLocalFrame())
                break;
            Document* doc = toLocalFrame(mainFrame())->document();
            if (!doc || !doc->styleResolver())
                break;
            doc->styleResolver()->viewportStyleResolver()->collectViewportRules();
        }
        break;
    case SettingsDelegate::TextTrackKindUserPreferenceChange:
        for (Frame* frame = mainFrame(); frame; frame = frame->tree().traverseNext()) {
            if (frame->isLocalFrame()) {
                Document* doc = toLocalFrame(frame)->document();
                if (doc)
                    HTMLMediaElement::setTextTrackKindUserPreferenceForAllMediaElements(doc);
            }
        }
        break;
    }
}
#endif

void Page::updateAcceleratedCompositingSettings()
{
    ASSERT(false); // BKTODO:
#if 0
    for (Frame* frame = mainFrame(); frame; frame = frame->tree().traverseNext()) {
        if (!frame->isLocalFrame())
            continue;
        if (FrameView* view = toLocalFrame(frame)->view())
            view->updateAcceleratedCompositingSettings();
    }
#endif
}

void Page::didCommitLoad(LocalFrame* frame)
{
    // BKTODO: notifyDidCommitLoad(frame);
#if 0 // BKTODO: May be useless.
    if (m_mainFrame == frame) {
        frame->console().clearMessages();
        useCounter().didCommitLoad();
        frameHost().visualViewport().sendUMAMetrics();
        m_originsUsingFeatures.updateMeasurementsAndClear();
        UserGestureIndicator::clearProcessedUserGestureSinceLoad();
    }
#endif
}

void Page::acceptLanguagesChanged()
{
    WillBeHeapVector<RefPtrWillBeMember<LocalFrame>> frames;

    ASSERT(false); // BKTODO:
#if 0
    // Even though we don't fire an event from here, the LocalDOMWindow's will fire
    // an event so we keep the frames alive until we are done.
    for (Frame* frame = mainFrame(); frame; frame = frame->tree().traverseNext()) {
        if (frame->isLocalFrame())
            frames.append(toLocalFrame(frame));
    }
#endif

    for (unsigned i = 0; i < frames.size(); ++i)
        frames[i]->localDOMWindow()->acceptLanguagesChanged();
}

#if 0 // BKTODO:
void Page::purgeMemory(DeviceKind deviceKind)
{
    if (deviceKind == DeviceKind::LowEnd)
        memoryCache()->pruneAll();
}

DEFINE_TRACE(Page)
{
#if ENABLE(OILPAN)
    visitor->trace(m_animator);
    visitor->trace(m_autoscrollController);
    visitor->trace(m_chromeClient);
    visitor->trace(m_dragCaretController);
    visitor->trace(m_dragController);
    visitor->trace(m_focusController);
    visitor->trace(m_contextMenuController);
    visitor->trace(m_pointerLockController);
    visitor->trace(m_scrollingCoordinator);
    visitor->trace(m_undoStack);
    visitor->trace(m_mainFrame);
    visitor->trace(m_validationMessageClient);
    visitor->trace(m_multisamplingChangedObservers);
    visitor->trace(m_frameHost);
    visitor->trace(m_memoryPurgeController);
    HeapSupplementable<Page>::trace(visitor);
#endif
    PageLifecycleNotifier::trace(visitor);
    MemoryPurgeClient::trace(visitor);
}
#endif

void Page::layerTreeViewInitialized(WebLayerTreeView& layerTreeView)
{
    if (scrollingCoordinator())
        scrollingCoordinator()->layerTreeViewInitialized(layerTreeView);
}

void Page::willCloseLayerTreeView(WebLayerTreeView& layerTreeView)
{
    if (m_scrollingCoordinator)
        m_scrollingCoordinator->willCloseLayerTreeView(layerTreeView);
}

void Page::willBeClosed()
{
    ASSERT(false); // BKTODO: ordinaryPages().remove(this);
}

void Page::willBeDestroyed()
{
    RefPtrWillBeRawPtr<Frame> mainFrame = m_mainFrame;

    mainFrame->detach();

#if 0 // BKTODO:
    ASSERT(allPages().contains(this));
    allPages().remove(this);
    ordinaryPages().remove(this);
#endif
#if !ENABLE(OILPAN)
    if (m_memoryPurgeController)
        m_memoryPurgeController->unregisterClient(this);
#endif

    if (m_scrollingCoordinator)
        m_scrollingCoordinator->willBeDestroyed();

    chromeClient().chromeDestroyed();
    if (m_validationMessageClient)
        m_validationMessageClient->willBeDestroyed();
    m_mainFrame = nullptr;

    // BKTODO: PageLifecycleNotifier::notifyContextDestroyed();
}

Page::PageClients::PageClients()
    : chromeClient(nullptr)
    , contextMenuClient(nullptr)
    , editorClient(nullptr)
    , dragClient(nullptr)
    // BKTODO: , spellCheckerClient(nullptr)
{
}

Page::PageClients::~PageClients()
{
}

template class CORE_TEMPLATE_EXPORT WillBeHeapSupplement<Page>;

} // namespace blink
