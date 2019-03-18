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

#include "base/time/time.h"
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

void ViewImpl::FillCoordinates(WebMouseEvent &dst, int x, int y)
{
    dst.x = dst.windowX = dst.globalX = x;
    dst.y = dst.windowY = dst.globalY = y;
    dst.movementX = dst.x - m_lastX;
    dst.movementY = dst.y - m_lastY;
}

int BKAPI ViewImpl::Load(const char *URI)
{
    KURL u(ParsedURLString, URI);
    if (u.protocolIsInHTTPFamily())
        return BkError::URIError;
    return BrowserImpl::Load(u);
}

void ViewImpl::PostHandleInput(const WebInputEvent &e)
{
    if (!WebInputEvent::isMouseEventType(e.type))
        return;

    const WebMouseEvent &me = static_cast<const WebMouseEvent &>(e);

    m_lastX = me.x;
    m_lastY = me.y;
    switch (e.type)
    {
        case WebInputEvent::MouseDown:
            m_lastDownTime = e.timeStampSeconds;
            m_lastDownX = me.x;
            m_lastDownY = me.y;
            break;
        case WebInputEvent::MouseLeave:
            m_mouseEntered = false;
            break;
    }
}

void ViewImpl::PreHandleInput(const WebInputEvent &e)
{
    if (WebInputEvent::MouseEnter == e.type)
        m_mouseEntered = true;
}

void BKAPI ViewImpl::ProcessInput(const MouseEvent &e)
{
    WebMouseEvent we = Translate(e);

    PreHandleInput(we);
    GetWebView()->handleInputEvent(we);
    PostHandleInput(we);
}

void BKAPI ViewImpl::ProcessInput(const KeyboardEvent &e)
{
    WebKeyboardEvent we = Translate(e);

    PreHandleInput(we);
    m_webView->handleInputEvent(we);
    PostHandleInput(we);
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

WebMouseEvent ViewImpl::Translate(const MouseEvent &e)
{
    WebMouseEvent we;
    we.timeStampSeconds = base::Time::Now().ToDoubleT();
    we.type = Translate(e.type);
    we.button = Translate(e.button);
    FillCoordinates(we, e.x, e.y);

    if (MouseEvent::MouseDown == e.type || MouseEvent::MouseMove == e.type)
    {
        switch (e.button)
        {
            case MouseEvent::LeftButton:
                we.modifiers |= WebMouseEvent::LeftButtonDown;
                break;
            case MouseEvent::RightButton:
                we.modifiers |= WebMouseEvent::RightButtonDown;
                break;
            case MouseEvent::MiddleButton:
                we.modifiers |= WebMouseEvent::MiddleButtonDown;
                break;
        }
    }

    if (WebMouseEvent::MouseDown == we.type)
    {
        we.clickCount = 1;
        if (!m_hasDoubleClickEvent)
        {
            const int delta = 15;
            if (we.timeStampSeconds - m_lastDownTime < DoubleClickInterval
                && std::abs(we.movementX) < delta && std::abs(we.movementY) < delta)
            {
                we.clickCount = 2;
            }
        }
    }
    return we;
}

WebInputEvent::Type ViewImpl::Translate(MouseEvent::Type t) const
{
    switch (t)
    {
        case MouseEvent::MouseMove:
            return m_mouseEntered ? WebInputEvent::MouseMove : WebInputEvent::MouseEnter;
        case MouseEvent::MouseDown:
            return WebInputEvent::MouseDown;
        case MouseEvent::MouseUp:
            return WebInputEvent::MouseUp;
        case MouseEvent::MouseLeave:
            return WebInputEvent::MouseLeave;
        case MouseEvent::ContextMenu:
            return WebInputEvent::ContextMenu;
    }

    assert(false); // Not reached!
    return WebInputEvent::Undefined;
}

WebMouseEvent::Button ViewImpl::Translate(MouseEvent::Button b)
{
    switch (b)
    {
        case MouseEvent::LeftButton:
            return WebMouseEvent::ButtonLeft;
        case MouseEvent::RightButton:
            return WebMouseEvent::ButtonRight;
        case MouseEvent::MiddleButton:
            return WebMouseEvent::ButtonMiddle;
    }

    assert(MouseEvent::NoButton == b);
    return WebMouseEvent::ButtonNone;
}

} // namespace BlinKit
