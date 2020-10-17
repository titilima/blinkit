// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: web_view_impl.h
// Description: WebViewImpl Class
//      Author: Ziming Li
//     Created: 2020-10-16
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_WEB_VIEW_IMPL_H
#define BLINKIT_BLINKIT_WEB_VIEW_IMPL_H

#pragma once

#include "bk_ui.h"
#include "blinkit/blink_impl/local_frame_client_impl.h"
#include "third_party/blink/renderer/core/page/chrome_client.h"
#include "third_party/blink/renderer/core/page/page_visibility_state.h"

class WebViewImpl : public BlinKit::LocalFrameClientImpl
{
public:
    virtual ~WebViewImpl(void);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Exports
    int LoadUI(const char *URI);
    void SetClient(const BkWebViewClient &client);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Returns the page object associated with this view. This may be null when
    // the page is shutting down, but will be valid at all other times.
    blink::Page* GetPage(void) const { return m_page.get(); }

protected:
    WebViewImpl(blink::PageVisibilityState visibilityState);
private:
    void SetVisibilityState(blink::PageVisibilityState visibilityState, bool isInitialState);

    // LocalFrameClient
    bool IsCrawler(void) const final { return false; }
    void TransitionToCommittedForNewPage(void) final;
    void DispatchDidFailProvisionalLoad(const blink::ResourceError &error) final;
    void DispatchDidFinishLoad(void) final;

    BkWebViewClient m_client;
    std::unique_ptr<blink::ChromeClient> m_chromeClient;
    std::unique_ptr<blink::Page> m_page;
};

#endif // BLINKIT_BLINKIT_WEB_VIEW_IMPL_H
