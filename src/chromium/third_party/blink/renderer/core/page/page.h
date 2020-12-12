// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: page.h
// Description: Page Class
//      Author: Ziming Li
//     Created: 2020-09-23
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_PAGE_H
#define BLINKIT_BLINK_PAGE_H

#pragma once

#include "third_party/blink/renderer/core/frame/use_counter.h"
#include "third_party/blink/renderer/core/page/page_animator.h"
#include "third_party/blink/renderer/core/page/page_visibility_state.h"

namespace blink {

class BrowserControls;
class ChromeClient;
class DragController;
class Frame;
class LocalFrame;
class LocalFrameClient;
class OverscrollController;
class PageScaleConstraintsSet;
class ScrollbarTheme;
class ScrollingCoordinator;
class TopDocumentRootScrollerController;
class VisualViewport;

class Page
{
public:
    ~Page(void);

    // It is up to the platform to ensure that non-null clients are provided where
    // required.
    struct PageClients final {
        STACK_ALLOCATED();
    public:
        PageClients(void) = default;

        ChromeClient *chromeClient = nullptr;
        LocalFrameClient *frameClient = nullptr;
        DISALLOW_COPY_AND_ASSIGN(PageClients);
    };

    static std::unique_ptr<Page> Create(PageClients &pageClients);
    void DocumentDetached(Document *document);

    ChromeClient& GetChromeClient(void) const
    {
        ASSERT(nullptr != m_chromeClient);
        return *m_chromeClient;
    }
    DragController& GetDragController(void) const { return *m_dragController; }
    LocalFrame* GetFrame(void) const { return m_mainFrame; }
    LocalFrame* MainFrame(void) const { return m_mainFrame; }
    void SetMainFrame(Frame *mainFrame);
    ScrollbarTheme& GetScrollbarTheme(void) const;
    PageAnimator& Animator(void) { return *m_animator; }

    // Corresponds to pixel density of the device where this Page is
    // being displayed. In multi-monitor setups this can vary between pages.
    // This value does not account for Page zoom, use LocalFrame::devicePixelRatio
    // instead.  This is to be deprecated. Use this with caution.
    // 1) If you need to scale the content per device scale factor, this is still
    //    valid.  In use-zoom-for-dsf mode, this is always 1, and will be remove
    //    when transition is complete.
    // 2) If you want to compute the device related measure (such as device pixel
    //    height, or the scale factor for drag image), use
    //    ChromeClient::screenInfo() instead.
    float DeviceScaleFactorDeprecated(void) const { return m_deviceScaleFactor; }
    void SetDeviceScaleFactorDeprecated(float scaleFactor);

    PageScaleConstraintsSet& GetPageScaleConstraintsSet(void);
    const PageScaleConstraintsSet& GetPageScaleConstraintsSet(void) const;

    BrowserControls& GetBrowserControls(void);
    const BrowserControls& GetBrowserControls(void) const;

    VisualViewport& GetVisualViewport(void);
    const VisualViewport& GetVisualViewport(void) const;

    OverscrollController& GetOverscrollController(void);
    const OverscrollController& GetOverscrollController(void) const;

    TopDocumentRootScrollerController& GlobalRootScrollerController(void) const;

    PageVisibilityState VisibilityState(void) const { return m_visibilityState; }
    void SetVisibilityState(PageVisibilityState visibilityState, bool isInitialState);

    ScrollingCoordinator* GetScrollingCoordinator(void);

    void DidCommitLoad(LocalFrame *frame);

#if DCHECK_IS_ON()
    bool IsPainting(void) const { return m_isPainting; }
    void SetIsPainting(bool painting) { m_isPainting = painting; }
#endif
private:
    explicit Page(PageClients &pageClients);

    LocalFrame *m_mainFrame = nullptr;
    ChromeClient *m_chromeClient;
    std::unique_ptr<PageAnimator> m_animator;
    const std::unique_ptr<DragController> m_dragController;
    float m_deviceScaleFactor = 1.0;
    std::unique_ptr<PageScaleConstraintsSet> m_pageScaleConstraintsSet;
    const std::unique_ptr<BrowserControls> m_browserControls;
    const std::unique_ptr<TopDocumentRootScrollerController> m_globalRootScrollerController;
    const std::unique_ptr<VisualViewport> m_visualViewport;
    const std::unique_ptr<OverscrollController> m_overscrollController;
    PageVisibilityState m_visibilityState = PageVisibilityState::kVisible;

#if DCHECK_IS_ON()
    bool m_isPainting = false;
#endif
};

} // namespace blink

#endif // BLINKIT_BLINK_PAGE_H
