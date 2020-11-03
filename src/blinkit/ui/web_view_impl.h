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
#include "third_party/blink/public/platform/web_size.h"
#include "third_party/blink/renderer/core/page/chrome_client.h"
#include "third_party/blink/renderer/core/page/page_visibility_state.h"
#include "third_party/blink/renderer/platform/geometry/int_rect.h"
#include "third_party/blink/renderer/platform/graphics/graphics_layer.h"

namespace blink {
class PageScaleConstraintsSet;
struct ViewportDescription;
}

class WebViewImpl : public BlinKit::LocalFrameClientImpl
{
public:
    virtual ~WebViewImpl(void);

    void Initialize(void);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Exports
    int LoadUI(const char *URI);
    void SetClient(const BkWebViewClient &client);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Returns the page object associated with this view. This may be null when
    // the page is shutting down, but will be valid at all other times.
    blink::Page* GetPage(void) const { return m_page.get(); }

    void ScheduleAnimation(void);
    void UpdateMainFrameLayoutSize(void);
    void UpdatePageDefinedViewportConstraints(const blink::ViewportDescription &description);
protected:
    WebViewImpl(blink::PageVisibilityState visibilityState);

    bool ProcessTitleChange(const std::string &newTitle) const;
private:
    blink::IntSize FrameSize(void);
    blink::Color BaseBackgroundColor(void) const;
    blink::PageScaleConstraintsSet& GetPageScaleConstraintsSet(void) const;
    float MinimumPageScaleFactor(void) const;
    void SetVisibilityState(blink::PageVisibilityState visibilityState, bool isInitialState);
    bool ShouldAutoResize(void) const { return m_shouldAutoResize; }

    // LocalFrameClient
    bool IsCrawler(void) const final { return false; }
    bool HasWebView(void) const final { return true; }
    void TransitionToCommittedForNewPage(void) final;
    void DispatchDidFailProvisionalLoad(const blink::ResourceError &error) final;
    void DispatchDidFinishLoad(void) final;

    BkWebViewClient m_client;
    std::unique_ptr<blink::ChromeClient> m_chromeClient;
    blink::WebSize m_size;
    // If true, automatically resize the layout view around its content.
    bool m_shouldAutoResize = false;
    std::unique_ptr<blink::Page> m_page;
    SkColor m_baseBackgroundColor;
};

#endif // BLINKIT_BLINKIT_WEB_VIEW_IMPL_H
