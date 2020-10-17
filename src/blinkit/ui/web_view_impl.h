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
#include "third_party/blink/renderer/core/page/chrome_client.h"

class WebViewImpl
{
public:
    virtual ~WebViewImpl(void);
protected:
    WebViewImpl(void);
private:
    std::unique_ptr<blink::ChromeClient> m_chromeClient;
    std::unique_ptr<blink::Page> m_page;
};

#endif // BLINKIT_BLINKIT_WEB_VIEW_IMPL_H
