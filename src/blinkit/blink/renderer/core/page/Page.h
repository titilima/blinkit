// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: Page.h
// Description: Page Class
//      Author: Ziming Li
//     Created: 2021-07-13
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006, 2007, 2008, 2009, 2010, 2013 Apple Inc. All rights reserved.
 * Copyright (C) 2008 Torch Mobile Inc. All rights reserved. (http://www.torchmobile.com/)
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

#ifndef Page_h
#define Page_h

#include "core/CoreExport.h"
#include "core/dom/ViewportDescription.h"
#include "core/frame/LocalFrame.h"
#if 0 // BKTODO:
#include "core/frame/OriginsUsingFeatures.h"
#include "core/frame/SettingsDelegate.h"
#endif
#include "core/frame/UseCounter.h"
#include "core/page/Page.h"
#include "core/page/PageAnimator.h"
#if 0 // BKTODO:
#include "core/page/PageLifecycleNotifier.h"
#include "core/page/PageLifecycleObserver.h"
#endif
#include "core/page/PageVisibilityState.h"
// BKTODO: #include "platform/MemoryPurgeController.h"
#include "platform/Supplementable.h"
#include "platform/geometry/LayoutRect.h"
#include "platform/geometry/Region.h"
#include "platform/heap/Handle.h"
#include "wtf/Forward.h"
#include "wtf/HashSet.h"
#include "wtf/Noncopyable.h"
#include "wtf/text/WTFString.h"

namespace blink {

class AutoscrollController;
class ChromeClient;
class ClientRectList;
class ContextMenuClient;
class ContextMenuController;
class Document;
class DragCaretController;
class DragClient;
class DragController;
class EditorClient;
class FocusController;
class Frame;
class FrameHost;
// BKTODO: class PluginData;
class PointerLockController;
class ScrollingCoordinator;
class Settings;
class SpellCheckerClient;
class UndoStack;
class ValidationMessageClient;
class WebLayerTreeView;

typedef uint64_t LinkHash;

float deviceScaleFactor(LocalFrame*);

class CORE_EXPORT Page final : public NoBaseWillBeGarbageCollectedFinalized<Page>, public WillBeHeapSupplementable<Page> { // BKTODO: , public PageLifecycleNotifier {
    WILL_BE_USING_GARBAGE_COLLECTED_MIXIN(Page);
    USING_FAST_MALLOC_WILL_BE_REMOVED(Page);
    WTF_MAKE_NONCOPYABLE(Page);
    friend class Settings;
public:
    // It is up to the platform to ensure that non-null clients are provided where required.
    struct CORE_EXPORT PageClients final {
        STACK_ALLOCATED();
        WTF_MAKE_NONCOPYABLE(PageClients);
    public:
        PageClients();
        ~PageClients();

        ChromeClient* chromeClient;
        ContextMenuClient* contextMenuClient;
        EditorClient* editorClient;
        DragClient* dragClient;
        // BKTODO: SpellCheckerClient* spellCheckerClient;
    };

    static std::unique_ptr<Page> create(PageClients& pageClients)
    {
        return zed::wrap_unique(new Page(pageClients));
    }

#if 0
    // An "ordinary" page is a fully-featured page owned by a web view.
    static PassOwnPtrWillBeRawPtr<Page> createOrdinary(PageClients&);
#endif

    ~Page();

    void willBeClosed();

#if 0 // BKTODO:
    using PageSet = WillBePersistentHeapHashSet<RawPtrWillBeWeakMember<Page>>;

    // Return the current set of full-fledged, ordinary pages.
    // Each created and owned by a WebView.
    //
    // This set does not include Pages created for other, internal purposes
    // (SVGImages, inspector overlays, page popups etc.)
    static PageSet& ordinaryPages();
#endif

    static void platformColorsChanged();
    static void onMemoryPressure();

    FrameHost& frameHost() const { return *m_frameHost; }

    void setNeedsRecalcStyleInAllFrames();
    void updateAcceleratedCompositingSettings();

    ViewportDescription viewportDescription() const;

#if 0 // BKTODO:
    static void refreshPlugins();
    PluginData* pluginData() const;
#endif

    EditorClient& editorClient() const { return *m_editorClient; }
    // BKTODO: SpellCheckerClient& spellCheckerClient() const { return *m_spellCheckerClient; }
    UndoStack& undoStack() const { return *m_undoStack; }

    void setMainFrame(Frame*);
    Frame* mainFrame() const { return m_mainFrame; }
    // Escape hatch for existing code that assumes that the root frame is
    // always a LocalFrame. With OOPI, this is not always the case. Code that
    // depends on this will generally have to be rewritten to propagate any
    // necessary state through all renderer processes for that page and/or
    // coordinate/rely on the browser process to help dispatch/coordinate work.
    LocalFrame* deprecatedLocalMainFrame() const { return toLocalFrame(m_mainFrame); }

    void documentDetached(Document*);

    bool openedByDOM() const;
    void setOpenedByDOM();

    PageAnimator& animator() { return *m_animator; }
    ChromeClient& chromeClient() const { return *m_chromeClient; }
    AutoscrollController& autoscrollController() const { return *m_autoscrollController; }
    DragCaretController& dragCaretController() const { return *m_dragCaretController; }
    DragController& dragController() const { return *m_dragController; }
    FocusController& focusController() const { return *m_focusController; }
    ContextMenuController& contextMenuController() const { return *m_contextMenuController; }
    PointerLockController& pointerLockController() const { return *m_pointerLockController; }
    ValidationMessageClient& validationMessageClient() const { return *m_validationMessageClient; }
    void setValidationMessageClient(PassOwnPtrWillBeRawPtr<ValidationMessageClient>);

    ScrollingCoordinator* scrollingCoordinator();

    String mainThreadScrollingReasonsAsText();
    ClientRectList* nonFastScrollableRects(const LocalFrame*);

#if 0 // BKTODO:
    Settings& settings() const { return *m_settings; }

    UseCounter& useCounter() { return m_useCounter; }
    OriginsUsingFeatures& originsUsingFeatures() { return m_originsUsingFeatures; }
#endif

    void setTabKeyCyclesThroughElements(bool b) { m_tabKeyCyclesThroughElements = b; }
    bool tabKeyCyclesThroughElements() const { return m_tabKeyCyclesThroughElements; }

    void unmarkAllTextMatches();

    // DefersLoading is used to delay loads during modal dialogs.
    // Modal dialogs are supposed to freeze all background processes
    // in the page, including prevent additional loads from staring/continuing.
    void setDefersLoading(bool);
    bool defersLoading() const { return m_defersLoading; }

    void setPageScaleFactor(float);
    float pageScaleFactor() const;

    float deviceScaleFactor() const { return m_deviceScaleFactor; }
    void setDeviceScaleFactor(float);
    void setDeviceColorProfile(const Vector<char>&);
#if 0 // BKTODO:
    void resetDeviceColorProfileForTesting();

    static void allVisitedStateChanged(bool invalidateVisitedLinkHashes);
    static void visitedStateChanged(LinkHash visitedHash);
#endif

    PageVisibilityState visibilityState() const;
    void setVisibilityState(PageVisibilityState, bool);

    bool isCursorVisible() const;
    void setIsCursorVisible(bool isVisible) { m_isCursorVisible = isVisible; }

#if ENABLE(ASSERT)
    void setIsPainting(bool painting) { m_isPainting = painting; }
    bool isPainting() const { return m_isPainting; }
#endif

    class CORE_EXPORT MultisamplingChangedObserver : public WillBeGarbageCollectedMixin {
    public:
        virtual void multisamplingChanged(bool) = 0;
    };

    void addMultisamplingChangedObserver(MultisamplingChangedObserver*);
#if !ENABLE(OILPAN)
    void removeMultisamplingChangedObserver(MultisamplingChangedObserver*);
#endif

    void didCommitLoad(LocalFrame*);

    void acceptLanguagesChanged();

    static void networkStateChanged(bool online);

#if 0 // BKTODO:
    MemoryPurgeController& memoryPurgeController();

    void purgeMemory(DeviceKind) override;

    DECLARE_TRACE();
#endif

    void layerTreeViewInitialized(WebLayerTreeView&);
    void willCloseLayerTreeView(WebLayerTreeView&);

    void willBeDestroyed();

private:
    explicit Page(PageClients&);

    void initGroup();

    void setNeedsLayoutInAllFrames();

#if 0 // BKTODO:
    // SettingsDelegate overrides.
    void settingsChanged(SettingsDelegate::ChangeType) override;
#endif

    std::unique_ptr<PageAnimator> m_animator;
    const std::unique_ptr<AutoscrollController> m_autoscrollController;
    ChromeClient *m_chromeClient;
    const std::unique_ptr<DragCaretController> m_dragCaretController;
    const std::unique_ptr<DragController> m_dragController;
    const std::unique_ptr<FocusController> m_focusController;
    const std::unique_ptr<ContextMenuController> m_contextMenuController;
    const std::unique_ptr<PointerLockController> m_pointerLockController;
    std::unique_ptr<ScrollingCoordinator> m_scrollingCoordinator;
    const std::unique_ptr<UndoStack> m_undoStack;

    // Typically, the main frame and Page should both be owned by the embedder,
    // which must call Page::willBeDestroyed() prior to destroying Page. This
    // call detaches the main frame and clears this pointer, thus ensuring that
    // this field only references a live main frame.
    //
    // However, there are several locations (InspectorOverlay, SVGImage, and
    // WebPagePopupImpl) which don't hold a reference to the main frame at all
    // after creating it. These are still safe because they always create a
    // Frame with a FrameView. FrameView and Frame hold references to each
    // other, thus keeping each other alive. The call to willBeDestroyed()
    // breaks this cycle, so the frame is still properly destroyed once no
    // longer needed.
    RawPtrWillBeMember<Frame> m_mainFrame;

    // BKTODO: mutable RefPtr<PluginData> m_pluginData;

    EditorClient* const m_editorClient;
    // BKTODO: SpellCheckerClient* const m_spellCheckerClient;
    OwnPtrWillBeMember<ValidationMessageClient> m_validationMessageClient;

#if 0 // BKTODO:
    UseCounter m_useCounter;
    OriginsUsingFeatures m_originsUsingFeatures;
#endif

    bool m_openedByDOM;

    bool m_tabKeyCyclesThroughElements;
    bool m_defersLoading;

    float m_deviceScaleFactor;

    PageVisibilityState m_visibilityState;

    bool m_isCursorVisible;

#if ENABLE(ASSERT)
    bool m_isPainting;
#endif

    WillBeHeapHashSet<RawPtrWillBeWeakMember<MultisamplingChangedObserver>> m_multisamplingChangedObservers;

    // A pointer to all the interfaces provided to in-process Frames for this Page.
    // FIXME: Most of the members of Page should move onto FrameHost.
    std::unique_ptr<FrameHost> m_frameHost;

    // BKTODO: OwnPtrWillBeMember<MemoryPurgeController> m_memoryPurgeController;
};

extern template class CORE_EXTERN_TEMPLATE_EXPORT WillBeHeapSupplement<Page>;

} // namespace blink

#endif // Page_h
