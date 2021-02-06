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
#include "blinkit/app/win_app.h"
#include "blinkit/win/bk_bitmap.h"
#include "blinkit/win/message_task.h"
#include "blinkit/win/view_store.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"

using namespace blink;

namespace BlinKit {

static ViewStore g_viewStore;

static SkColor WindowColor(void)
{
    COLORREF color = GetSysColor(COLOR_WINDOW);
    return SkColorSetARGB(0xff, GetRValue(color), GetGValue(color), GetBValue(color));
}

WinWebView::WinWebView(HWND hWnd, ClientCaller &clientCaller, bool isWindowVisible)
    : WebViewImpl(clientCaller, isWindowVisible ? PageVisibilityState::kVisible : PageVisibilityState::kHidden, WindowColor())
    , m_hWnd(hWnd)
{
    g_viewStore.OnNewView(m_hWnd, this);

    HDC dc = GetDC(m_hWnd);
    m_dpi = GetDeviceCaps(dc, LOGPIXELSY);
    ASSERT(GetDeviceCaps(dc, LOGPIXELSX) == m_dpi);
    m_memoryDC = CreateCompatibleDC(dc);
    ReleaseDC(m_hWnd, dc);
    UpdateScaleFactor();
}

WinWebView::~WinWebView(void)
{
    if (nullptr != m_oldBitmap)
        SelectBitmap(m_memoryDC, m_oldBitmap);
    DeleteDC(m_memoryDC);

    g_viewStore.OnViewDestroyed(m_hWnd);
}

void WinWebView::DispatchDidReceiveTitle(const String &title)
{
    auto task = [this, newTitle = title.StdUtf8()]
    {
        if (ProcessTitleChange(newTitle))
            return;

        std::wstring ws = base::SysUTF8ToWide(newTitle);
        SetWindowTextW(m_hWnd, ws.c_str());
    };
    MessageTask::Post(m_hWnd, std::move(task));
}

void WinWebView::InvalidateNativeView(const IntRect &rect)
{
    RECT rc = { rect.X(), rect.Y(), rect.MaxX(), rect.MaxY() };
    ::InvalidateRect(m_hWnd, &rc, FALSE);
}

void WinWebView::OnDPIChanged(HWND hwnd, UINT newDPI, const RECT *rc)
{
    m_dpi = newDPI;
    UpdateScaleFactor();
    SetWindowPos(hwnd, nullptr, rc->left, rc->top, rc->right - rc->left, rc->bottom - rc->top,
        SWP_NOZORDER | SWP_NOACTIVATE);
}

BOOL WinWebView::OnNCCreate(HWND hwnd, LPCREATESTRUCT cs)
{
    AppImpl &app = AppImpl::Get();

    ClientCaller &clientCaller = app.AcquireCallerForClient();

    WinWebView *webView = nullptr;
    auto task = [&webView, &clientCaller, hwnd, style = cs->style]
    {
        webView = new WinWebView(hwnd, clientCaller, 0 != (style & WS_VISIBLE));
        webView->Initialize();
    };
    app.GetAppCaller().SyncCall(FROM_HERE, task);
    return TRUE;
}

void WinWebView::OnNCDestroy(HWND hwnd)
{
    m_appCaller.Call(
        FROM_HERE,
        std::bind(std::default_delete<WinWebView>(), this)
    );
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
    {
        std::unique_lock<BkMutex> lock(m_canvasLock);
        BitBlt(hdc, x, y, w, h, m_memoryDC, x, y, SRCCOPY);
    }

    EndPaint(hwnd, &ps);
}

void WinWebView::OnShowWindow(HWND, BOOL fShow, UINT)
{
    PageVisibilityState state = fShow ? PageVisibilityState::kVisible : PageVisibilityState::kHidden;
    WebViewImpl::SetVisibilityState(state);
}

void WinWebView::OnSize(HWND, UINT state, int cx, int cy)
{
    if (SIZE_MINIMIZED == state)
        return;

    auto task = [this, cx, cy]
    {
        Resize(WebSize(cx, cy));
        UpdateAndPaint();
    };
    m_appCaller.Call(FROM_HERE, std::move(task));
}

SkBitmap WinWebView::PrepareBitmapForCanvas(const WebSize &size)
{
    BkBitmap ret;
    HBITMAP hBitmap = ret.InstallDIBSection(size.width, size.height, m_memoryDC);
    HBITMAP oldBitmap = SelectBitmap(m_memoryDC, hBitmap);
    if (nullptr == m_oldBitmap)
        m_oldBitmap = oldBitmap;
    return ret;
}

bool WinWebView::ProcessWindowMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT *result)
{
    if (WM_NCCREATE == Msg)
    {
        *result = HANDLE_WM_NCCREATE(hWnd, wParam, lParam, OnNCCreate);
        return true;
    }

    WinWebView *v = g_viewStore.Lookup(hWnd);
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
        case WM_ERASEBKGND:
            *result = TRUE;
            break;
        case WM_SIZE:
            HANDLE_WM_SIZE(hWnd, wParam, lParam, OnSize);
            break;
        case WM_SETFOCUS:
            WebViewImpl::SetFocus(true);
            break;
        case WM_KILLFOCUS:
            WebViewImpl::SetFocus(false);
            break;
        case WM_SHOWWINDOW:
            HANDLE_WM_SHOWWINDOW(hWnd, wParam, lParam, OnShowWindow);
            break;
        case WM_DPICHANGED:
            ASSERT(HIWORD(wParam) == LOWORD(lParam));
            OnDPIChanged(hWnd, HIWORD(wParam), reinterpret_cast<LPRECT>(lParam));
            break;
        case WM_NCDESTROY:
            HANDLE_WM_NCDESTROY(hWnd, wParam, lParam, OnNCDestroy);
            break;
        default:
            return MessageTask::Process(Msg, wParam, lParam);
    }
    return true;
}

void WinWebView::UpdateScaleFactor(void)
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
            NOTREACHED();
            return;
    }
    SetScaleFactor(scaleFactor);
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
    return BlinKit::g_viewStore.Lookup(hWnd);
}

BKEXPORT bool_t BKAPI BkProcessWindowMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT *result)
{
    return BlinKit::WinWebView::ProcessWindowMessage(hWnd, Msg, wParam, lParam, result);
}

} // extern "C"
