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
#include "public/web/WebSettings.h"

#include "win/dib_section.h"

using namespace blink;

namespace BlinKit {

WinView::WinView(BkViewClient &client) : ViewImpl(client)
{
    WebSettings *settings = GetWebView()->settings();
    settings->setStandardFontFamily("Segoe UI");
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

std::unique_ptr<SkCanvas> WinView::CreateMemoryCanvas(int width, int height)
{
    if (nullptr != m_oldBitmap)
        SelectBitmap(m_memoryDC, m_oldBitmap);

    DIBSection bitmap(width, height, m_memoryDC);
    m_oldBitmap = SelectObject(m_memoryDC, bitmap.GetHBITMAP());
    return std::make_unique<SkCanvas>(bitmap);
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

        case WM_SETFOCUS:
            SetFocus(true);
            break;
        case WM_KILLFOCUS:
            SetFocus(false);
            break;

        case WM_SIZE:
            HANDLE_WM_SIZE(h, w, l, OnSize);
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
