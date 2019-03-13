// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: browser_impl.h
// Description: BrowserImpl Class
//      Author: Ziming Li
//     Created: 2019-03-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_BROWSER_IMPL_H
#define BLINKIT_BLINKIT_BROWSER_IMPL_H

#pragma once

#include "public/web/WebFrameClient.h"
#include "public/web/WebView.h"
#include "public/web/WebViewClient.h"

namespace blink {
class KURL;
}

namespace BlinKit {

class BrowserImpl : public blink::WebViewClient, public blink::WebFrameClient
{
protected:
    BrowserImpl(void);
    ~BrowserImpl(void);

    blink::WebView* GetWebView(void) { return m_webView; }
    const blink::WebView* GetWebView(void) const { return m_webView; }
protected:
    int Load(const blink::KURL &URL);
private:
    // blink::WebWidgetClient
    bool allowsBrokenNullLayerTreeView(void) const final { return true; }

    blink::WebView *m_webView;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_BROWSER_IMPL_H
