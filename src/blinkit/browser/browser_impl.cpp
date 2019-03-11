// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: browser_impl.cpp
// Description: BrowserImpl Class
//      Author: Ziming Li
//     Created: 2019-03-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "browser_impl.h"

#include "public/web/WebCache.h" 
#include "public/web/WebLocalFrame.h"

using namespace blink;

namespace BlinKit {

BrowserImpl::BrowserImpl(void)
{
    // WebView::create SHOULD BE put here, but not into the initialize list.
    // Because the vtable need to be fully constructed.
    m_webView = WebView::create(this);

    WebLocalFrame *mainFrame = WebLocalFrame::create(WebTreeScopeType::Document, this);
    m_webView->setMainFrame(mainFrame);
}

BrowserImpl::~BrowserImpl(void)
{
    m_webView->close();
    WebCache::clear();
}

} // namespace BlinKit
