// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: page.cpp
// Description: Page Class
//      Author: Ziming Li
//     Created: 2020-10-17
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "page.h"

#include "third_party/blink/renderer/core/editing/drag_caret.h"
#include "third_party/blink/renderer/core/frame/browser_controls.h"
#include "third_party/blink/renderer/core/frame/link_highlights.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/page_scale_constraints_set.h"
#include "third_party/blink/renderer/core/frame/visual_viewport.h"
#include "third_party/blink/renderer/core/page/autoscroll_controller.h"
#include "third_party/blink/renderer/core/page/chrome_client.h"
#include "third_party/blink/renderer/core/page/drag_controller.h"
#include "third_party/blink/renderer/core/page/focus_controller.h"
#include "third_party/blink/renderer/core/page/scrolling/overscroll_controller.h"
#include "third_party/blink/renderer/core/page/scrolling/top_document_root_scroller_controller.h"
#include "third_party/blink/renderer/core/scroll/scrollbar_theme.h"

namespace blink {

Page::Page(PageClients &pageClients)
    : m_chromeClient(pageClients.chromeClient)
    , m_dragCaret(DragCaret::Create())
    , m_animator(PageAnimator::Create(*this))
    , m_autoscrollController(AutoscrollController::Create(*this))
    , m_dragController(DragController::Create(this))
    , m_focusController(FocusController::Create(this))
    , m_pageScaleConstraintsSet(PageScaleConstraintsSet::Create(this))
    , m_browserControls(BrowserControls::Create(*this))
    , m_globalRootScrollerController(TopDocumentRootScrollerController::Create(*this))
    , m_visualViewport(VisualViewport::Create(*this))
    , m_linkHighlights(LinkHighlights::Create(*this))
    , m_overscrollController(OverscrollController::Create(GetVisualViewport(), GetChromeClient()))
{
}

Page::~Page(void)
{
    // WillBeDestroyed() must be called before Page destruction.
    ASSERT(nullptr == m_mainFrame);
}

std::unique_ptr<Page> Page::Create(PageClients &pageClients)
{
    return base::WrapUnique(new (ObjectType::Root) Page(pageClients));
}

void Page::DidCommitLoad(LocalFrame *frame)
{
#if 0 // BKTODO: Check if necessary
    GetConsoleMessageStorage().Clear();
    // TODO(loonybear): Most of this doesn't appear to take into account that
    // each SVGImage gets it's own Page instance.
    GetDeprecation().ClearSuppression();
    GetVisualViewport().SendUMAMetrics();
#endif
    // Need to reset visual viewport position here since before commit load we
    // would update the previous history item, Page::didCommitLoad is called
    // after a new history item is created in FrameLoader.
    // See crbug.com/642279
    GetVisualViewport().SetScrollOffset(ScrollOffset(), kProgrammaticScroll);
#if 0 // BKTODO: Check if necessary
    hosts_using_features_.UpdateMeasurementsAndClear();
    GetLinkHighlights().ResetForPageNavigation();
#endif
}

void Page::DocumentDetached(Document *document)
{
#if 0 // BKTODO: Check this later
    pointer_lock_controller_->DocumentDetached(document);
    context_menu_controller_->DocumentDetached(document);
    if (validation_message_client_)
        validation_message_client_->DocumentDetached(*document);
    hosts_using_features_.DocumentDetached(*document);
#endif
}

BrowserControls& Page::GetBrowserControls(void)
{
    return *m_browserControls;
}

const BrowserControls& Page::GetBrowserControls(void) const
{
    return *m_browserControls;
}

OverscrollController& Page::GetOverscrollController(void)
{
    return *m_overscrollController;
}

const OverscrollController& Page::GetOverscrollController(void) const
{
    return *m_overscrollController;
}

PageScaleConstraintsSet& Page::GetPageScaleConstraintsSet(void)
{
    return *m_pageScaleConstraintsSet;
}

const PageScaleConstraintsSet& Page::GetPageScaleConstraintsSet(void) const
{
    return *m_pageScaleConstraintsSet;
}

ScrollbarTheme& Page::GetScrollbarTheme(void) const
{
#if 0 // BKTODO: Check the logic later.
    if (settings_->GetForceAndroidOverlayScrollbar())
        return ScrollbarThemeOverlay::MobileTheme();
#endif
    return ScrollbarTheme::DeprecatedStaticGetTheme();
}

ScrollingCoordinator* Page::GetScrollingCoordinator(void)
{
#if 0 // BKTODO: Check the logic later
    if (!scrolling_coordinator_ && settings_->GetAcceleratedCompositingEnabled())
        scrolling_coordinator_ = ScrollingCoordinator::Create(this);
#endif
    return nullptr;
}

VisualViewport& Page::GetVisualViewport(void)
{
    return *m_visualViewport;
}

const VisualViewport& Page::GetVisualViewport(void) const
{
    return *m_visualViewport;
}

TopDocumentRootScrollerController& Page::GlobalRootScrollerController(void) const
{
    return *m_globalRootScrollerController;
}

void Page::SetMainFrame(Frame *mainFrame)
{
    ASSERT(mainFrame->IsLocalFrame());
    m_mainFrame = static_cast<LocalFrame *>(mainFrame);
}

void Page::SetDeviceScaleFactorDeprecated(float scaleFactor)
{
    if (m_deviceScaleFactor == scaleFactor)
        return;

    m_deviceScaleFactor = scaleFactor;
    if (nullptr != m_mainFrame)
        m_mainFrame->DeviceScaleFactorChanged();
}

void Page::SetVisibilityState(PageVisibilityState visibilityState, bool isInitialState)
{
    if (m_visibilityState == visibilityState)
        return;

    m_visibilityState = visibilityState;
    if (isInitialState)
        return;

    NotifyPageVisibilityChanged();
    if (nullptr != m_mainFrame)
        m_mainFrame->DidChangeVisibilityState();
}

void Page::Trace(Visitor *visitor)
{
    m_dragCaret->Trace(visitor);
    visitor->Trace(m_globalRootScrollerController);
}

void Page::WillBeDestroyed(void)
{
    m_mainFrame->Detach(FrameDetachType::kRemove);

#if 0 // BKTODO:
    if (m_scrollingCoordinator)
        scrolling_coordinator_->WillBeDestroyed();
#endif

    GetChromeClient().ChromeDestroyed();
#if 0 // BKTODO:
    if (m_validationMessageClient)
        validation_message_client_->WillBeDestroyed();
#endif
    m_mainFrame = nullptr;

#if 0 // BKTODO:
    PageVisibilityNotifier::NotifyContextDestroyed();

    m_pageScheduler.reset();
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float DeviceScaleFactorDeprecated(LocalFrame *frame)
{
    if (nullptr == frame)
        return 1;
    Page *page = frame->GetPage();
    if (nullptr == page)
        return 1;
    return page->DeviceScaleFactorDeprecated();
}

} // namespace blink
