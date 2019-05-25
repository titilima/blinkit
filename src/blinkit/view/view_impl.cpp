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
#include "platform/weborigin/KURL.h"
#include "public/platform/Platform.h"
#include "public/platform/WebTaskRunner.h"
#include "public/platform/WebTraceLocation.h"
#include "public/web/WebCache.h"
#include "public/web/WebLocalFrame.h"
#include "public/web/WebSettings.h"
#include "web/WebLocalFrameImpl.h"

#include "view/context_menu.h"

#include "js/public/script_controller.h"

using namespace blink;

namespace BlinKit {

ViewImpl::ViewImpl(BkViewClient &client) : m_client(client), m_updateRequired(std::make_shared<bool>(false))
{
    // WebView::create SHOULD BE put here, but not into the initialize list.
    // Because the vtable need to be fully constructed.
    m_webView = WebView::create(this);

    WebLocalFrame *mainFrame = WebLocalFrame::create(WebTreeScopeType::Document, this);
    m_webView->setMainFrame(mainFrame);

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
    m_webView->close();
    WebCache::clear();

    *m_updateRequired = false; // Ignore pending update requests.
}

int BKAPI ViewImpl::CallFunction(const char *name, BkCallback *callback)
{
    return GetFrame()->script().CallFunction(name, callback);
}

void ViewImpl::didFinishLoad(WebLocalFrame *)
{
    m_client.DocumentReady(this);
}

void ViewImpl::didReceiveTitle(WebLocalFrame *, const WebString &title, WebTextDirection)
{
    std::string newTitle = title.utf8();
    if (m_client.AllowTitleChange(this, newTitle.c_str()))
        ChangeTitle(GetNativeView(), newTitle);
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

int BKAPI ViewImpl::Eval(const char *code, size_t length, BkCallback *callback)
{
    return GetFrame()->script().Eval(code, length, callback);
}

void ViewImpl::FillCoordinates(WebMouseEvent &dst, int x, int y)
{
    dst.x = dst.windowX = dst.globalX = x;
    dst.y = dst.windowY = dst.globalY = y;
    dst.movementX = dst.x - m_lastX;
    dst.movementY = dst.y - m_lastY;
}

bool BKAPI ViewImpl::GetCaretRect(BkRect *dst)
{
    WebRect anchor, focus;
    if (!GetWebView()->selectionBounds(anchor, focus))
        return false;

    dst->x = focus.x;
    dst->y = focus.y;
    dst->width = 1;
    dst->height = focus.height;
    return true;
}

LocalFrame* ViewImpl::GetFrame(void)
{
    WebLocalFrameImpl *f = toWebLocalFrameImpl(GetWebView()->mainFrame());
    return f->frame();
}

int BKAPI ViewImpl::Load(const char *URI)
{
    KURL u(ParsedURLString, URI);
    if (!u.isValid() || u.protocolIsInHTTPFamily())
        return BkError::URIError;

    m_webView->mainFrame()->loadRequest(WebURLRequest(u));
    return BkError::Success;
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
    GetWebView()->handleInputEvent(we);
    PostHandleInput(we);
}

int BKAPI ViewImpl::RegisterExternalFunction(const char *name, BkCallback &functionImpl)
{
    return GetFrame()->script().RegisterFunction(name, functionImpl);
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

void ViewImpl::showContextMenu(const WebContextMenuData &data)
{
    if (!ContextMenu::ShouldShow(data))
        return;

    if (!m_contextMenu)
        m_contextMenu = ContextMenu::CreateInstance(GetWebView());
    m_contextMenu->Show(GetNativeView(), data);
}

void ViewImpl::startDragging(
    WebLocalFrame *frame,
    const WebDragData &data, WebDragOperationsMask mask,
    const WebImage &image, const WebPoint &dragImageOffset)
{
    GetWebView()->dragSourceSystemDragEnded();
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

WebKeyboardEvent ViewImpl::Translate(const KeyboardEvent &e)
{
    WebKeyboardEvent we;

    we.timeStampSeconds = base::Time::Now().ToDoubleT();
    we.type = Translate(e.type);

    if (e.shiftPressed)
        we.modifiers |= WebInputEvent::ShiftKey;
    if (e.ctrlPressed)
        we.modifiers |= WebInputEvent::ControlKey;
    if (e.altPressed)
        we.modifiers |= WebInputEvent::AltKey;
    if (e.fromKeyPad)
        we.modifiers |= WebInputEvent::IsKeyPad;

    we.windowsKeyCode = we.nativeKeyCode = e.code;
    we.domCode = we.domKey = 0;

    memset(we.text, 0, sizeof(we.text));
    we.text[0] = e.code;
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

WebInputEvent::Type ViewImpl::Translate(KeyboardEvent::Type t)
{
    switch (t)
    {
        case KeyboardEvent::KeyDown:
            return WebInputEvent::KeyDown;
        case KeyboardEvent::KeyUp:
            return WebInputEvent::KeyUp;
        case KeyboardEvent::Char:
            return WebInputEvent::Char;
    }

    assert(false); // Not reached!
    return WebInputEvent::Undefined;
}

} // namespace BlinKit
