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

#include "third_party/blink/renderer/core/page/chrome_client_impl.h"
#include "third_party/blink/renderer/core/page/page.h"

using namespace blink;

WebViewImpl::WebViewImpl(PageVisibilityState visibilityState) : m_chromeClient(ChromeClientImpl::Create(this))
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
}

WebViewImpl::~WebViewImpl(void) = default;

void WebViewImpl::DispatchDidFailProvisionalLoad(const ResourceError &error)
{
    ASSERT(false); // BKTODO:
}

void WebViewImpl::DispatchDidFinishLoad(void)
{
    ASSERT(false); // BKTODO:
}

int WebViewImpl::LoadUI(const char *URI)
{
    ASSERT(false); // BKTODO:
    return BK_ERR_FORBIDDEN;
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
    ASSERT(false); // BKTODO:
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
