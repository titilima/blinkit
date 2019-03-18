// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: view_impl.cpp
// Description: ViewImpl Class
//      Author: Ziming Li
//     Created: 2019-03-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "view_impl.h"

#include "public/platform/Platform.h"
#include "public/platform/WebTaskRunner.h"
#include "public/platform/WebTraceLocation.h"
#include "public/web/WebSettings.h"
#include "platform/weborigin/KURL.h"

using namespace blink;

namespace BlinKit {

ViewImpl::ViewImpl(BkViewClient &client) : m_client(client), m_updateRequired(std::make_shared<bool>(false))
{
    WebSettings *settings = GetWebView()->settings();
    settings->setAcceleratedCompositingEnabled(false);
    settings->setCookieEnabled(false);
    settings->setEnableScrollAnimator(false);
    settings->setMinimumFontSize(10);
    settings->setMinimumLogicalFontSize(10);
    settings->setDefaultFontSize(16);
    settings->setDefaultFixedFontSize(16);
    settings->setLoadsImagesAutomatically(true);
}

ViewImpl::~ViewImpl(void)
{
    *m_updateRequired = false; // Ignore pending update requests.
}

void ViewImpl::didFinishLoad(WebLocalFrame *)
{
    m_client.DocumentReady(this);
}

void ViewImpl::DoUpdate(void)
{
    WebView *webView = GetWebView();

    webView->updateAllLifecyclePhases();

    WebSize size = webView->size();

    WebRect rc;
    rc.x = rc.y = 0;
    rc.width = size.width;
    rc.height = size.height;
    webView->paint(m_memoryCanvas.get(), rc);
}

int BKAPI ViewImpl::Load(const char *URI)
{
    KURL u(ParsedURLString, URI);
    if (u.protocolIsInHTTPFamily())
        return BkError::URIError;
    return BrowserImpl::Load(u);
}

void BKAPI ViewImpl::Resize(int width, int height)
{
    if (0 == width || 0 == height)
        return;

    WebView *webView = GetWebView();

    const WebSize newSize(width, height);
    const WebSize oldSize = webView->size();
    if (oldSize == newSize)
        return;

    m_memoryCanvas = CreateMemoryCanvas(width, height);
    m_memoryCanvas->clear(BackgroundColor());
    webView->resize(newSize);
}

void ViewImpl::scheduleAnimation(void)
{
    if (UpdateRequired())
        return;

    std::shared_ptr<bool> updateRequired(m_updateRequired);
    auto task = [this, updateRequired] {
        if (*updateRequired)
        {
            DoUpdate();
            *updateRequired = false;
        }
    };
    Platform::current()->currentThread()->taskRunner()->postTask(BLINK_FROM_HERE, task);
    *m_updateRequired = true;
}

void BKAPI ViewImpl::SetFocus(bool focused)
{
    WebView *webView = GetWebView();
    webView->setFocus(focused);
    if (focused)
        webView->setIsActive(true);
}

void BKAPI ViewImpl::SetScaleFactor(float scaleFactor)
{
    GetWebView()->setZoomFactorForDeviceScaleFactor(scaleFactor);
}

} // namespace BlinKit
