// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: LocalFrame.h
// Description: LocalFrame Class
//      Author: Ziming Li
//     Created: 2021-07-13
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1998, 1999 Torben Weis <weis@kde.org>
 *                     1999-2001 Lars Knoll <knoll@kde.org>
 *                     1999-2001 Antti Koivisto <koivisto@kde.org>
 *                     2000-2001 Simon Hausmann <hausmann@kde.org>
 *                     2000-2001 Dirk Mueller <mueller@kde.org>
 *                     2000 Stefan Schimanski <1Stein@gmx.de>
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010 Apple Inc. All rights reserved.
 * Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2008 Eric Seidel <eric@webkit.org>
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

#ifndef LocalFrame_h
#define LocalFrame_h

#include "core/CoreExport.h"
// BKTODO: #include "core/dom/WeakIdentifierMap.h"
#include "core/frame/Frame.h"
#include "core/frame/LocalFrameLifecycleNotifier.h"
#include "core/frame/LocalFrameLifecycleObserver.h"
#include "core/loader/FrameLoader.h"
// BKTODO: #include "core/page/FrameTree.h"
#include "core/paint/PaintPhase.h"
#include "platform/Supplementable.h"
#include "platform/graphics/ImageOrientation.h"
#include "platform/graphics/paint/DisplayItem.h"
#include "platform/heap/Handle.h"
#include "platform/scroll/ScrollTypes.h"
#include "wtf/HashSet.h"

namespace blink {

class Color;
class Document;
class DragImage;
class Editor;
template <typename Traversal> class EditingAlgorithm;
class Element;
template <typename Strategy> class EphemeralRangeTemplate;
class EventHandler;
class FloatSize;
class FrameConsole;
class FrameSelection;
class FrameView;
class HTMLPlugInElement;
class InputMethodController;
class IntPoint;
class IntSize;
class InstrumentingAgents;
class LocalDOMWindow;
class NavigationScheduler;
class Node;
class NodeTraversal;
class Range;
class LayoutView;
class TreeScope;
class ScriptController;
// BKTODO: class SpellChecker;
class TreeScope;
class WebFrameHostScheduler;
class WebFrameScheduler;
template <typename Strategy> class PositionWithAffinityTemplate;

class LocalFrame : public Frame
                 , public LocalFrameLifecycleNotifier
                 , public WillBeHeapSupplementable<LocalFrame>
                 , public DisplayItemClient
{
    WILL_BE_USING_GARBAGE_COLLECTED_MIXIN(LocalFrame);
public:
    template <class... Args>
    static GCUniquePtr<LocalFrame> create(Args&&... args) {
        return GCWrapUnique(new LocalFrame(std::forward<Args>(args)...));
    }

    void init();
    void setView(PassRefPtrWillBeRawPtr<FrameView>);
    void createView(const IntSize&, const Color&, bool,
        ScrollbarMode = ScrollbarAuto, bool horizontalLock = false,
        ScrollbarMode = ScrollbarAuto, bool verticalLock = false);

    // Frame overrides:
    ~LocalFrame() override;
    DECLARE_VIRTUAL_TRACE();
    bool isLocalFrame() const override { return true; }
    DOMWindow* domWindow() const override;
    WindowProxy* windowProxy(DOMWrapperWorld&) override;
    void navigate(Document& originDocument, const KURL&, bool replaceCurrentItem, UserGestureStatus) override;
    void navigate(const FrameLoadRequest&) override;
    // BKTODO: void reload(FrameLoadType, ClientRedirectPolicy) override;
    void detach(void) override;
    // BKTODO: void disconnectOwnerElement() override;
    bool shouldClose() override;
    // BKTODO: SecurityContext* securityContext() const override;
    void printNavigationErrorMessage(const Frame&, const char* reason) override;
    bool prepareForCommit() override;

    void willDetachFrameHost();

    LocalDOMWindow* localDOMWindow() const;
    void setDOMWindow(GCUniquePtr<LocalDOMWindow> &&);
    FrameView* view() const;
    Document* document() const;
    void setPagePopupOwner(Element&);
    Element* pagePopupOwner() const { return m_pagePopupOwner.get(); }

    LayoutView* contentLayoutObject() const; // Root of the layout tree for the document contained in this frame.

    Editor& editor() const;
    EventHandler& eventHandler() const;
    FrameLoader& loader() const;
    // BKTODO: NavigationScheduler& navigationScheduler() const;
    FrameSelection& selection() const;
    InputMethodController& inputMethodController() const;
    ScriptController& script() const;
    // BKTODO: SpellChecker& spellChecker() const;
    FrameConsole& console() const;

    void didChangeVisibilityState();

    // This method is used to get the highest level LocalFrame in this
    // frame's in-process subtree.
    // FIXME: This is a temporary hack to support RemoteFrames, and callers
    // should be updated to avoid storing things on the main frame.
    LocalFrame* localFrameRoot();

    // BKTODO: InstrumentingAgents* instrumentingAgents() const { return m_instrumentingAgents.get(); }

    // ======== All public functions below this point are candidates to move out of LocalFrame into another class. ========

    // See GraphicsLayerClient.h for accepted flags.
    String layerTreeAsText(unsigned flags = 0) const;

    FloatSize resizePageRectsKeepingRatio(const FloatSize& originalSize, const FloatSize& expectedSize);

    constexpr bool inViewSourceMode(void) const { return false; }

#ifdef BLINKIT_UI_ENABLED
    float pageZoomFactor(void) const { return m_scaleFactor; }
    constexpr float textZoomFactor(void) const { return 1.0f; }
    void SetScaleFactor(float scaleFactor);
#endif

    void deviceScaleFactorChanged();
    double devicePixelRatio() const;

    PassOwnPtr<DragImage> nodeImage(Node&);
    PassOwnPtr<DragImage> dragImageForSelection(float opacity);

    String selectedText() const;
    String selectedTextForClipboard() const;

    PositionWithAffinityTemplate<EditingAlgorithm<NodeTraversal>> positionForPoint(const IntPoint& framePoint);
    Document* documentAtPoint(const IntPoint&);
    EphemeralRangeTemplate<EditingAlgorithm<NodeTraversal>> rangeForPoint(const IntPoint& framePoint);

    bool isURLAllowed(const KURL&) const;
    bool shouldReuseDefaultView(const KURL&) const;
    void removeSpellingMarkersUnderWords(const Vector<String>& words);

    // FIXME: once scroll customization is enabled everywhere
    // (crbug.com/416862), this should take a ScrollState object.
    ScrollResult applyScrollDelta(const FloatSize& delta, bool isScrollBegin);
    bool shouldScrollTopControls(const FloatSize& delta) const;

    // DisplayItemClient methods
    String debugName() const final { return "LocalFrame"; }
    // TODO(chrishtr): fix this.
    IntRect visualRect() const override { return IntRect(); }

    bool shouldThrottleRendering() const;

    // Returns the frame scheduler, creating one if needed.
    WebFrameScheduler* frameScheduler();
    void scheduleVisualUpdateUnlessThrottled();

    // BKTODO: void updateSecurityOrigin(SecurityOrigin*);

    bool isNavigationAllowed() const { return m_navigationDisableCount == 0; }

private:
    friend class FrameNavigationDisabler;

#ifdef BLINKIT_UI_ENABLED
    LocalFrame(FrameLoaderClient*, FrameHost* = nullptr, float = 1.0);
#else
    LocalFrame(FrameLoaderClient*);
#endif

    // Internal Frame helper overrides:
    WindowProxyManager* windowProxyManager() const override;

    String localLayerTreeAsText(unsigned flags) const;

    // Paints the area for the given rect into a DragImage, with the given displayItemClient id attached.
    // The rect is in the coordinate space of the frame.
    PassOwnPtr<DragImage> paintIntoDragImage(const DisplayItemClient&,
        RespectImageOrientationEnum shouldRespectImageOrientation, const GlobalPaintFlags,
        IntRect paintingRect, float opacity = 1);

    void enableNavigation() { --m_navigationDisableCount; }
    void disableNavigation() { ++m_navigationDisableCount; }

    mutable FrameLoader m_loader;
    // BKTODO: OwnPtrWillBeMember<NavigationScheduler> m_navigationScheduler;

    GCUniquePtr<FrameView> m_view;
    GCUniquePtr<LocalDOMWindow> m_domWindow;
    // Usually 0. Non-null if this is the top frame of PagePopup.
    RefPtrWillBeMember<Element> m_pagePopupOwner;

    const std::unique_ptr<ScriptController> m_script;
    const std::unique_ptr<Editor> m_editor;
    // BKTODO: const OwnPtrWillBeMember<SpellChecker> m_spellChecker;
    const std::unique_ptr<FrameSelection> m_selection;
    const std::unique_ptr<EventHandler> m_eventHandler;
    const std::unique_ptr<FrameConsole> m_console;
    const std::unique_ptr<InputMethodController> m_inputMethodController;
    OwnPtr<WebFrameScheduler> m_frameScheduler;

    int m_navigationDisableCount;

#ifdef BLINKIT_UI_ENABLED
    float m_scaleFactor = 1.0;
#endif

    // TODO(dcheng): Temporary to try to debug https://crbug.com/531291
    enum class SupplementStatus { Uncleared, Clearing, Cleared };
    SupplementStatus m_supplementStatus = SupplementStatus::Uncleared;
};

inline void LocalFrame::init()
{
    m_loader.init();
}

inline LocalDOMWindow* LocalFrame::localDOMWindow() const
{
    return m_domWindow.get();
}

inline FrameLoader& LocalFrame::loader() const
{
    return m_loader;
}

#if 0 // BKTODO:
inline NavigationScheduler& LocalFrame::navigationScheduler() const
{
    ASSERT(m_navigationScheduler);
    return *m_navigationScheduler.get();
}
#endif

inline FrameView* LocalFrame::view() const
{
    return m_view.get();
}

inline ScriptController& LocalFrame::script() const
{
    return *m_script;
}

inline FrameSelection& LocalFrame::selection() const
{
    return *m_selection;
}

inline Editor& LocalFrame::editor() const
{
    return *m_editor;
}

#if 0 // BKTODO:
inline SpellChecker& LocalFrame::spellChecker() const
{
    return *m_spellChecker;
}
#endif

inline FrameConsole& LocalFrame::console() const
{
    return *m_console;
}

inline InputMethodController& LocalFrame::inputMethodController() const
{
    return *m_inputMethodController;
}

inline EventHandler& LocalFrame::eventHandler() const
{
    ASSERT(m_eventHandler);
    return *m_eventHandler;
}

DEFINE_TYPE_CASTS(LocalFrame, Frame, localFrame, localFrame->isLocalFrame(), localFrame.isLocalFrame());

class FrameNavigationDisabler {
    WTF_MAKE_NONCOPYABLE(FrameNavigationDisabler);
    STACK_ALLOCATED();
public:
    explicit FrameNavigationDisabler(LocalFrame&);
    ~FrameNavigationDisabler();

private:
    RawPtrWillBeMember<LocalFrame> m_frame;
};

} // namespace blink

#endif // LocalFrame_h
