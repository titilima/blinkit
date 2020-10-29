// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: web_view_impl.cpp
// Description: WebViewImpl Class
//      Author: Ziming Li
//     Created: 2020-10-16
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "web_view_impl.h"

#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/page_scale_constraints_set.h"
#include "third_party/blink/renderer/core/loader/frame_load_request.h"
#include "third_party/blink/renderer/core/page/chrome_client_impl.h"
#include "third_party/blink/renderer/core/page/page.h"
#include "url/gurl.h"

using namespace blink;

WebViewImpl::WebViewImpl(PageVisibilityState visibilityState)
    : m_chromeClient(ChromeClientImpl::Create(this)), m_baseBackgroundColor(Color::kWhite)
{
    memset(&m_client, 0, sizeof(m_client));

    Page::PageClients pageClients;
    pageClients.chromeClient = m_chromeClient.get();
    pageClients.frameClient = this;
    m_page = Page::Create(pageClients);
    SetVisibilityState(visibilityState, true);

#if 0 // BKTODO:
    // TODO(dcheng): All WebViewImpls should have an associated LayerTreeView,
    // but for various reasons, that's not the case... WebView plugin, printing,
    // workers, and tests don't use a compositor in their WebViews. Sometimes
    // they avoid the compositor by using a null client, and sometimes by having
    // the client return a null compositor. We should make things more consistent
    // and clear.
    // For some reason this was not set when WidgetClient() is not provided,
    // even though there will be no LayerTreeView in that case either.
    if (WidgetClient() && WidgetClient()->AllowsBrokenNullLayerTreeView())
        page_->GetSettings().SetAcceleratedCompositingEnabled(false);

    AllInstances().insert(this);

    page_importance_signals_.SetObserver(client);
    resize_viewport_anchor_ = new ResizeViewportAnchor(*page_);
#endif

    m_page->GetFrame()->Init();
}

WebViewImpl::~WebViewImpl(void) = default;

Color WebViewImpl::BaseBackgroundColor(void) const
{
    return m_baseBackgroundColor;
}

void WebViewImpl::DispatchDidFailProvisionalLoad(const ResourceError &error)
{
    ASSERT(false); // BKTODO:
}

void WebViewImpl::DispatchDidFinishLoad(void)
{
    ASSERT(false); // BKTODO:
}

IntSize WebViewImpl::FrameSize(void)
{
    // The frame size should match the viewport size at minimum scale, since the
    // viewport must always be contained by the frame.
    FloatSize frameSize(m_size);
    frameSize.Scale(1 / MinimumPageScaleFactor());
    return ExpandedIntSize(frameSize);
}

PageScaleConstraintsSet& WebViewImpl::GetPageScaleConstraintsSet(void) const
{
    return m_page->GetPageScaleConstraintsSet();
}

int WebViewImpl::LoadUI(const char *URI)
{
    GURL u(URI);
    if (u.SchemeIsHTTPOrHTTPS())
    {
        BKLOG("URLs are not supported: %s", URI);
        return BK_ERR_URI;
    }

    FrameLoadRequest request(nullptr, ResourceRequest(u));
    m_page->GetFrame()->Loader().StartNavigation(request);
    return BK_ERR_SUCCESS;
}

float WebViewImpl::MinimumPageScaleFactor(void) const
{
    return GetPageScaleConstraintsSet().FinalConstraints().minimum_scale;
}

void WebViewImpl::ScheduleAnimation(void)
{
#if 0 // BKTODO:
    if (layer_tree_view_)
    {
        layer_tree_view_->SetNeedsBeginFrame();
        return;
    }
    if (client_)
        client_->WidgetClient()->ScheduleAnimation();
#endif
}

void WebViewImpl::SetClient(const BkWebViewClient &client)
{
    memset(&m_client, 0, sizeof(m_client));

    m_client.UserData = client.UserData;
    m_client.DocumentReady = client.DocumentReady;

    // Use `offsetof` macro for different client versions.
}

void WebViewImpl::SetVisibilityState(PageVisibilityState visibilityState, bool isInitialState)
{
    ASSERT(m_page);
    GetPage()->SetVisibilityState(visibilityState, isInitialState);

#if 0 // BKTODO:
    bool visible = visibilityState == PageVisibilityState::kVisible;
    if (layer_tree_view_ && !override_compositor_visibility_)
        layer_tree_view_->SetVisible(visible);
    GetPage()->GetPageScheduler()->SetPageVisible(visible);
#endif
}

void WebViewImpl::TransitionToCommittedForNewPage(void)
{
    // Check if we're shutting down.
    if (!m_page)
        return;

    IntSize initialSize = FrameSize();
    Color baseBackgroundColor = BaseBackgroundColor();

    LocalFrame *frame = m_page->GetFrame();
    frame->CreateView(initialSize, baseBackgroundColor);

    LocalFrameView *view = frame->View();
    view->SetInitialViewportSize(GetPageScaleConstraintsSet().InitialViewportSize());
    if (ShouldAutoResize())
    {
        ASSERT(false); // BKTODO:
#if 0
        view->EnableAutoSizeMode(MinAutoSize(), MaxAutoSize());
#endif
    }

#if 0 // BKTODO: Check if necessary
    view->SetInputEventsScaleForEmulation(input_events_scale_factor_for_emulation_);
    view->SetDisplayMode(DisplayMode());

    if (frame_widget_)
        frame_widget_->DidCreateLocalRootView();
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT int BKAPI BkLoadUI(BkWebView view, const char *URI)
{
    return view->LoadUI(URI);
}

BKEXPORT void BKAPI BkWebViewSetClient(BkWebView view, BkWebViewClient *client)
{
    view->SetClient(*client);
}

} // extern "C"
