// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_view.cpp
// Description: WinView Class
//      Author: Ziming Li
//     Created: 2019-03-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "win_view.h"

#include <windowsx.h>
#include "base/time/time.h"
#include "public/web/WebSettings.h"

#include "win/dib_section.h"

using namespace blink;

namespace BlinKit {

WinView::WinView(BkViewClient &client) : ViewImpl(client)
{
    WebSettings *settings = GetWebView()->settings();
    settings->setStandardFontFamily("Segoe UI");

    m_cursorInfo.externalHandle = LoadCursor(nullptr, IDC_ARROW);
}

WinView::~WinView(void)
{
    if (nullptr != m_memoryDC)
    {
        assert(nullptr != m_oldBitmap);
        SelectBitmap(m_memoryDC, m_oldBitmap);
        DeleteDC(m_memoryDC);
    }
}

void WinView::AdjustGlobalCoordinates(WebMouseEvent &we)
{
    POINT pt = { we.globalX, we.globalY };
    ClientToScreen(m_hWnd, &pt);
    we.globalX = pt.x;
    we.globalY = pt.y;
}

void BKAPI WinView::Attach(NativeView nativeView)
{
    m_hWnd = nativeView;

    HDC dc = GetDC(m_hWnd);
    m_dpi = GetDeviceCaps(dc, LOGPIXELSY);
    assert(GetDeviceCaps(dc, LOGPIXELSX) == m_dpi);
    m_memoryDC = CreateCompatibleDC(dc);
    ReleaseDC(m_hWnd, dc);

    UpdateScaleFactor();

    LONG style = GetClassLong(m_hWnd, GCL_STYLE);
    if (CS_DBLCLKS & style)
        m_hasDoubleClickEvent = true;
}

bool WinView::BuildMouseEvent(MouseEvent &dst, UINT message, UINT keyFlags, int x, int y)
{
    switch (message)
    {
        case WM_LBUTTONDOWN:
            dst.type = MouseEvent::MouseDown;
            dst.button = MouseEvent::LeftButton;
            break;
        case WM_LBUTTONUP:
            dst.type = MouseEvent::MouseUp;
            dst.button = MouseEvent::LeftButton;
            break;
        case WM_MOUSEMOVE:
            dst.type = MouseEvent::MouseMove;
            if (MK_LBUTTON & keyFlags)
                dst.button = MouseEvent::LeftButton;
            else if (MK_RBUTTON & keyFlags)
                dst.button = MouseEvent::RightButton;
            else if (MK_MBUTTON & keyFlags)
                dst.button = MouseEvent::MiddleButton;
            else
                dst.button = MouseEvent::NoButton;
            break;
        case WM_RBUTTONDOWN:
            dst.type = MouseEvent::MouseDown;
            dst.button = MouseEvent::RightButton;
            break;
        case WM_RBUTTONUP:
            dst.type = MouseEvent::MouseUp;
            dst.button = MouseEvent::RightButton;
            break;
        case WM_MOUSELEAVE:
            dst.type = MouseEvent::MouseLeave;
            dst.button = MouseEvent::NoButton;
            break;
        case WM_MBUTTONDOWN:
            dst.type = MouseEvent::MouseDown;
            dst.button = MouseEvent::MiddleButton;
            break;
        case WM_MBUTTONUP:
            dst.type = MouseEvent::MouseUp;
            dst.button = MouseEvent::MiddleButton;
            break;
        default:
            assert(false); // Not reached!
            return false;
    }

    dst.x = x;
    dst.y = y;
    return true;
}

std::unique_ptr<SkCanvas> WinView::CreateMemoryCanvas(int width, int height)
{
    if (nullptr != m_oldBitmap)
        SelectBitmap(m_memoryDC, m_oldBitmap);

    DIBSection bitmap(width, height, m_memoryDC);
    m_oldBitmap = SelectObject(m_memoryDC, bitmap.GetHBITMAP());
    return std::make_unique<SkCanvas>(bitmap);
}

void WinView::didChangeCursor(const WebCursorInfo &cursorInfo)
{
    if (m_changingSizeOrPosition)
        return;

    if (WebCursorInfo::TypeCustom == m_cursorInfo.type)
        DestroyCursor(m_cursorInfo.externalHandle);
    m_cursorInfo = cursorInfo;

    switch (m_cursorInfo.type)
    {
        case WebCursorInfo::TypePointer:
            m_cursorInfo.externalHandle = LoadCursor(nullptr, IDC_ARROW);
            break;
        case WebCursorInfo::TypeCross:
            m_cursorInfo.externalHandle = LoadCursor(nullptr, IDC_CROSS);
            break;
        case WebCursorInfo::TypeHand:
            m_cursorInfo.externalHandle = LoadCursor(nullptr, IDC_HAND);
            break;
        case WebCursorInfo::TypeIBeam:
            m_cursorInfo.externalHandle = LoadCursor(nullptr, IDC_IBEAM);
            break;
        case WebCursorInfo::TypeWait:
            m_cursorInfo.externalHandle = LoadCursor(nullptr, IDC_WAIT);
            break;
        case WebCursorInfo::TypeHelp:
            m_cursorInfo.externalHandle = LoadCursor(nullptr, IDC_HELP);
            break;
        case WebCursorInfo::TypeEastResize:
        case WebCursorInfo::TypeWestResize:
        case WebCursorInfo::TypeEastWestResize:
        case WebCursorInfo::TypeColumnResize:
            m_cursorInfo.externalHandle = LoadCursor(nullptr, IDC_SIZEWE);
            break;
        case WebCursorInfo::TypeNorthResize:
        case WebCursorInfo::TypeSouthResize:
        case WebCursorInfo::TypeNorthSouthResize:
        case WebCursorInfo::TypeRowResize:
            m_cursorInfo.externalHandle = LoadCursor(nullptr, IDC_SIZENS);
            break;
        case WebCursorInfo::TypeNorthEastResize:
        case WebCursorInfo::TypeSouthWestResize:
            m_cursorInfo.externalHandle = LoadCursor(nullptr, IDC_SIZENESW);
            break;
        case WebCursorInfo::TypeNorthWestResize:
        case WebCursorInfo::TypeSouthEastResize:
            m_cursorInfo.externalHandle = LoadCursor(nullptr, IDC_SIZENWSE);
            break;
        case WebCursorInfo::TypeNorthEastSouthWestResize:
        case WebCursorInfo::TypeNorthWestSouthEastResize:
        case WebCursorInfo::TypeMove:
            m_cursorInfo.externalHandle = LoadCursor(nullptr, IDC_SIZEALL);
            break;
        case WebCursorInfo::TypeProgress:
            m_cursorInfo.externalHandle = LoadCursor(nullptr, IDC_APPSTARTING);
            break;
        case WebCursorInfo::TypeNoDrop:
        case WebCursorInfo::TypeNotAllowed:
            m_cursorInfo.externalHandle = LoadCursor(nullptr, IDC_NO);
            break;
        case WebCursorInfo::TypeCustom:
            break;
        default:
            assert(false); // Not reached!
            m_cursorInfo.externalHandle = LoadCursor(nullptr, IDC_ARROW);
    }

    SetCursor(m_cursorInfo.externalHandle);
}

void WinView::DoUpdate(void)
{
    ViewImpl::DoUpdate();
    InvalidateRect(m_hWnd, nullptr, FALSE);
}

void WinView::OnDpiChanged(HWND hwnd, UINT dpi, const RECT &rc)
{
    m_dpi = dpi;
    UpdateScaleFactor();
    SetWindowPos(hwnd, nullptr, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
        SWP_NOZORDER | SWP_NOACTIVATE);
}

void WinView::OnMouse(UINT message, UINT keyFlags, int x, int y)
{
    if (!m_changingSizeOrPosition)
    {
        MouseEvent e;
        if (BuildMouseEvent(e, message, keyFlags, x, y))
            ProcessInput(e);
    }
}

bool WinView::OnNCCreate(HWND hwnd, LPCREATESTRUCT)
{
    Attach(hwnd);
    return true;
}

void WinView::OnNCDestroy(HWND)
{
    Destroy();
}

void WinView::OnPaint(HWND hwnd)
{
    assert(m_hWnd == hwnd);

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    BkRect rc;
    rc.x = ps.rcPaint.left;
    rc.y = ps.rcPaint.top;
    rc.width = ps.rcPaint.right - ps.rcPaint.left;
    rc.height = ps.rcPaint.bottom - ps.rcPaint.top;
    Paint(hdc, &rc);

    EndPaint(hwnd, &ps);
}

void WinView::OnSize(HWND, UINT state, int cx, int cy)
{
    if (SIZE_MINIMIZED == state)
        return;

    Resize(cx, cy);
    ViewImpl::DoUpdate();
}

void BKAPI WinView::Paint(NativeCanvas nativeCanvas, const BkRect *rc)
{
    int x, y, w, h;
    if (nullptr != rc)
    {
        x = rc->x;
        y = rc->y;
        w = rc->width;
        h = rc->height;
    }
    else
    {
        RECT clientRect;
        GetClientRect(m_hWnd, &clientRect);
        x = y = 0;
        w = clientRect.right;
        h = clientRect.bottom;
    }

    BitBlt(nativeCanvas, x, y, w, h, m_memoryDC, x, y, SRCCOPY);
}

void WinView::PostHandleInput(const WebMouseEvent &we)
{
    ViewImpl::PostHandleInput(we);

    if (WebInputEvent::MouseEnter == we.type)
    {
        TRACKMOUSEEVENT tme = { 0 };
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = m_hWnd;
        TrackMouseEvent(&tme);
    }

    if (WebInputEvent::MouseUp == we.type && WebMouseEvent::ButtonLeft == we.button)
        ReleaseCapture();
}

void WinView::PreHandleInput(const WebMouseEvent &we)
{
    if (WebInputEvent::MouseDown == we.type && WebMouseEvent::ButtonLeft == we.button)
        SetCapture(m_hWnd);

    ViewImpl::PreHandleInput(we);
}

void WinView::ProcessDoubleClick(UINT message, UINT keyFlags, int x, int y)
{
    assert(m_hasDoubleClickEvent);

    WebMouseEvent we;
    we.timeStampSeconds = base::Time::Now().ToDoubleT();
    we.type = WebInputEvent::MouseDown;
    switch (message)
    {
        case WM_LBUTTONDBLCLK:
            we.button = WebMouseEvent::ButtonLeft;
            we.modifiers |= WebMouseEvent::LeftButtonDown;
            break;
        case WM_RBUTTONDBLCLK:
            we.button = WebMouseEvent::ButtonRight;
            we.modifiers |= WebMouseEvent::RightButtonDown;
            break;
        case WM_MBUTTONDBLCLK:
            we.button = WebMouseEvent::ButtonMiddle;
            we.modifiers |= WebMouseEvent::MiddleButtonDown;
            break;
    }
    FillCoordinates(we, x, y);
    AdjustGlobalCoordinates(we);
    we.clickCount = 2;

    PreHandleInput(we);
    GetWebView()->handleInputEvent(we);
    PostHandleInput(we);
}

bool BKAPI WinView::ProcessMessage(HWND h, UINT m, WPARAM w, LPARAM l, LRESULT &r)
{
    r = 0;
    switch (m)
    {
        case WM_ERASEBKGND:
            r = TRUE;
            break;
        case WM_PAINT:
            HANDLE_WM_PAINT(h, w, l, OnPaint);
            break;

        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MOUSEMOVE:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
            OnMouse(m, w, GET_X_LPARAM(l), GET_Y_LPARAM(l));
            break;
        case WM_MOUSELEAVE:
        {
            POINT pt = { 0 };
            GetCursorPos(&pt);
            ScreenToClient(h, &pt);
            OnMouse(WM_MOUSELEAVE, 0, pt.x, pt.y);
            break;
        }
        case WM_SETCURSOR:
        {
            const UINT hitTestCode = LOWORD(l);
            if (HTCLIENT != hitTestCode)
                return false;

            SetCursor(m_cursorInfo.externalHandle);
            r = TRUE;
            break;
        }
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDBLCLK:
            ProcessDoubleClick(m, w, GET_X_LPARAM(l), GET_Y_LPARAM(l));
            break;

        case WM_SETFOCUS:
            SetFocus(true);
            break;
        case WM_KILLFOCUS:
            SetFocus(false);
            break;

        case WM_SIZE:
            HANDLE_WM_SIZE(h, w, l, OnSize);
            break;

        case WM_ENTERSIZEMOVE:
            m_changingSizeOrPosition = true;
            break;
        case WM_EXITSIZEMOVE:
            m_changingSizeOrPosition = false;
            break;

        case WM_DPICHANGED:
        {
            const UINT dpi = HIWORD(w);
            assert(LOWORD(w) == dpi);
            LPRECT rc = reinterpret_cast<LPRECT>(l);
            OnDpiChanged(h, dpi, *rc);
            break;
        }

        case WM_NCCREATE:
            r = HANDLE_WM_NCCREATE(h, w, l, OnNCCreate);
            if (r)
                return false; // Let it go on.
            break;
        case WM_NCDESTROY:
            HANDLE_WM_NCDESTROY(h, w, l, OnNCDestroy);
            [[fallthrough]];
        default:
            return false;
    }
    return true;
}

WebMouseEvent WinView::Translate(const MouseEvent &e)
{
    WebMouseEvent we = ViewImpl::Translate(e);
    AdjustGlobalCoordinates(we);
    return we;
}

void WinView::UpdateScaleFactor(void)
{
    float scaleFactor = 1.0;
    switch (m_dpi)
    {
        case 96:
            break;
        case 120:
            scaleFactor = 1.25;
            break;
        case 144:
            scaleFactor = 1.5;
            break;
        case 192:
            scaleFactor = 2.0;
            break;
        default:
            assert(false); // Not reached!
            return;
    }
    SetScaleFactor(scaleFactor);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const double ViewImpl::DoubleClickInterval = static_cast<double>(GetDoubleClickTime()) / 1000.0;

ViewImpl* ViewImpl::CreateInstance(BkViewClient &client)
{
    return new WinView(client);
}

SkColor ViewImpl::BackgroundColor(void)
{
    COLORREF clr = GetSysColor(COLOR_WINDOW);
    return SkColorSetARGB(0xff, GetRValue(clr), GetGValue(clr), GetBValue(clr));
}

} // namespace BlinKit
