// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_web_view.cpp
// Description: WinWebView Class
//      Author: Ziming Li
//     Created: 2020-10-16
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "win_web_view.h"

#include <windowsx.h>
#include "base/strings/sys_string_conversions.h"
#include "blinkit/win/dib_section.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"

using namespace blink;

namespace BlinKit {

std::unordered_map<HWND, WinWebView *> WinWebView::s_viewMap;

static SkColor WindowColor(void)
{
    COLORREF color = GetSysColor(COLOR_WINDOW);
    return SkColorSetARGB(0xff, GetRValue(color), GetGValue(color), GetBValue(color));
}

WinWebView::WinWebView(HWND hWnd, bool isWindowVisible)
    : WebViewImpl(isWindowVisible ? PageVisibilityState::kVisible : PageVisibilityState::kHidden, WindowColor())
    , m_hWnd(hWnd)
{
    ASSERT(IsMainThread());
    ASSERT(Lookup(m_hWnd) == nullptr);
    s_viewMap[m_hWnd] = this;

    HDC dc = GetDC(m_hWnd);
    m_dpi = GetDeviceCaps(dc, LOGPIXELSY);
    ASSERT(GetDeviceCaps(dc, LOGPIXELSX) == m_dpi);
    m_memoryDC = CreateCompatibleDC(dc);
    ReleaseDC(m_hWnd, dc);
}

WinWebView::~WinWebView(void)
{
    ASSERT(IsMainThread());
    ASSERT(Lookup(m_hWnd) == this);

    if (nullptr != m_memoryDC)
    {
        ASSERT(nullptr != m_oldBitmap);
        SelectBitmap(m_memoryDC, m_oldBitmap);
        DeleteDC(m_memoryDC);
    }

    s_viewMap.erase(m_hWnd);
}

std::unique_ptr<cc::SkiaPaintCanvas> WinWebView::CreateCanvas(const WebSize &size)
{
    if (nullptr != m_oldBitmap)
        SelectBitmap(m_memoryDC, m_oldBitmap);

    DIBSection bitmap(size.width, size.height, m_memoryDC);
    m_oldBitmap = SelectObject(m_memoryDC, bitmap.GetHBITMAP());
    return std::make_unique<cc::SkiaPaintCanvas>(bitmap); // BKTODO: Is ImageProvider needed?
}

void WinWebView::DispatchDidReceiveTitle(const String &title)
{
    std::string newTitle = title.StdUtf8();
    if (ProcessTitleChange(newTitle))
        return;

    std::wstring ws = base::SysUTF8ToWide(newTitle);
    SetWindowTextW(m_hWnd, ws.c_str());
}

WinWebView* WinWebView::Lookup(HWND hWnd)
{
    auto it = s_viewMap.find(hWnd);
    return std::end(s_viewMap) != it ? it->second : nullptr;
}

BOOL WinWebView::OnNCCreate(HWND hwnd, LPCREATESTRUCT cs)
{
    if (nullptr == Lookup(hwnd))
    {
        bool isWindowVisible = 0 != (cs->style & WS_VISIBLE);

        auto v = new WinWebView(hwnd, isWindowVisible);
        v->Initialize();
    }
    else
    {
        NOTREACHED();
    }
    return TRUE;
}

void WinWebView::OnPaint(HWND hwnd)
{
    ASSERT(m_hWnd == hwnd);

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    int x = ps.rcPaint.left;
    int y = ps.rcPaint.top;
    int w = ps.rcPaint.right - ps.rcPaint.left;
    int h = ps.rcPaint.bottom - ps.rcPaint.top;
    BitBlt(hdc, x, y, w, h, m_memoryDC, x, y, SRCCOPY);

    EndPaint(hwnd, &ps);
}

void WinWebView::OnSize(HWND, UINT state, int cx, int cy)
{
    if (SIZE_MINIMIZED == state)
        return;

    Resize(WebSize(cx, cy));
    ASSERT(false); // BKTODO: Update
}

bool WinWebView::ProcessWindowMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT *result)
{
    if (WM_NCCREATE == Msg)
    {
        *result = HANDLE_WM_NCCREATE(hWnd, wParam, lParam, OnNCCreate);
        return true;
    }

    WinWebView *v = Lookup(hWnd);
    if (nullptr == v)
        return false;

    return v->ProcessWindowMessageImpl(hWnd, Msg, wParam, lParam, result);
}

bool WinWebView::ProcessWindowMessageImpl(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT *result)
{
    switch (Msg)
    {
        case WM_PAINT:
            HANDLE_WM_PAINT(hWnd, wParam, lParam, OnPaint);
            break;
        case WM_SIZE:
            HANDLE_WM_SIZE(hWnd, wParam, lParam, OnSize);
            break;
        case WM_NCDESTROY:
            delete this;
            break;
        default:
            return false;
    }
    return true;
}

} // namespace BlinKit

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT LRESULT CALLBACK BkDefWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT r = 0;
    bool handled = BlinKit::WinWebView::ProcessWindowMessage(hWnd, Msg, wParam, lParam, &r);
    if (handled)
    {
        switch (Msg)
        {
            case WM_NCCREATE:
            case WM_NCDESTROY:
                break;
            default:
                return r;
        }
    }

    WNDPROC defProc = IsWindowUnicode(hWnd) ? DefWindowProcW : DefWindowProcA;
    return defProc(hWnd, Msg, wParam, lParam);
}

BKEXPORT BkWebView BKAPI BkGetWebView(HWND hWnd)
{
    return BlinKit::WinWebView::Lookup(hWnd);
}

BKEXPORT bool_t BKAPI BkProcessWindowMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT *result)
{
    return BlinKit::WinWebView::ProcessWindowMessage(hWnd, Msg, wParam, lParam, result);
}

} // extern "C"
