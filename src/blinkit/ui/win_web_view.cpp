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

#include "./win_web_view.h"

#include <windowsx.h>
#include "blinkit/app/win_app.h"
#include "blinkit/win/bk_bitmap.h"
#include "blinkit/win/message_task.h"
#include "blinkit/win/view_store.h"
#include "third_party/zed/include/zed/log.hpp"

using namespace blink;

namespace BlinKit {

static ViewStore g_viewStore;

#ifndef NDEBUG
class MessageLogger
{
public:
    MessageLogger(const UINT msg) : m_msg(msg)
    {
        ZLOG("ProcessWindowMessage begin: {}", m_msg);
    }
    ~MessageLogger(void)
    {
        ZLOG("ProcessWindowMessage end: {}", m_msg);
    }
private:
    const UINT m_msg;
};
#endif

static PageVisibilityState GetPageVisibilityState(bool isWindowVisible)
{
    return isWindowVisible
        ? PageVisibilityState::PageVisibilityStateVisible
        : PageVisibilityState::PageVisibilityStateHidden;
}

static SkColor WindowColor(void)
{
    COLORREF color = GetSysColor(COLOR_WINDOW);
    return SkColorSetARGB(0xff, GetRValue(color), GetGValue(color), GetBValue(color));
}

WinWebView::WinWebView(HWND hWnd, ClientCaller &clientCaller, bool isWindowVisible)
    : WebViewImpl(clientCaller, GetPageVisibilityState(isWindowVisible), WindowColor())
    , m_hWnd(hWnd)
{
    g_viewStore.OnNewView(m_hWnd, this);

    HDC dc = GetDC(m_hWnd);
    m_dpi = GetDeviceCaps(dc, LOGPIXELSY);
    ASSERT(GetDeviceCaps(dc, LOGPIXELSX) == m_dpi);
    m_memoryDC = CreateCompatibleDC(dc);
    ReleaseDC(m_hWnd, dc);

    m_cursorInfo.externalHandle = LoadCursor(nullptr, IDC_ARROW);
}

WinWebView::~WinWebView(void)
{
    if (nullptr != m_oldBitmap)
        SelectBitmap(m_memoryDC, m_oldBitmap);
    DeleteDC(m_memoryDC);

    g_viewStore.OnViewDestroyed(m_hWnd);
}

void WinWebView::didChangeCursor(const WebCursorInfo &cursorInfo)
{
    if (m_changingSizeOrPosition)
        return;

    PCTSTR cursorName = nullptr;
    switch (cursorInfo.type)
    {
        case WebCursorInfo::TypePointer:
            cursorName = IDC_ARROW;
            break;
        case WebCursorInfo::TypeCross:
            cursorName = IDC_CROSS;
            break;
        case WebCursorInfo::TypeHand:
            cursorName = IDC_HAND;
            break;
        case WebCursorInfo::TypeIBeam:
            cursorName = IDC_IBEAM;
            break;
        case WebCursorInfo::TypeWait:
            cursorName = IDC_WAIT;
            break;
        case WebCursorInfo::TypeHelp:
            cursorName = IDC_HELP;
            break;
        case WebCursorInfo::TypeEastResize:
        case WebCursorInfo::TypeWestResize:
        case WebCursorInfo::TypeEastWestResize:
        case WebCursorInfo::TypeColumnResize:
            cursorName = IDC_SIZEWE;
            break;
        case WebCursorInfo::TypeNorthResize:
        case WebCursorInfo::TypeSouthResize:
        case WebCursorInfo::TypeNorthSouthResize:
        case WebCursorInfo::TypeRowResize:
            cursorName = IDC_SIZENS;
            break;
        case WebCursorInfo::TypeNorthEastResize:
        case WebCursorInfo::TypeSouthWestResize:
            cursorName = IDC_SIZENESW;
            break;
        case WebCursorInfo::TypeNorthWestResize:
        case WebCursorInfo::TypeSouthEastResize:
            cursorName = IDC_SIZENWSE;
            break;
        case WebCursorInfo::TypeNorthEastSouthWestResize:
        case WebCursorInfo::TypeNorthWestSouthEastResize:
        case WebCursorInfo::TypeMove:
            cursorName = IDC_SIZEALL;
            break;
        case WebCursorInfo::TypeProgress:
            cursorName = IDC_APPSTARTING;
            break;
        case WebCursorInfo::TypeNoDrop:
        case WebCursorInfo::TypeNotAllowed:
            cursorName = IDC_NO;
            break;
        case WebCursorInfo::TypeCustom:
            break;
        default:
            ASSERT_NOT_REACHED();
            cursorName = IDC_ARROW;
    }

    auto _ = m_lock.guard();
    if (WebCursorInfo::TypeCustom == m_cursorInfo.type)
        DestroyCursor(m_cursorInfo.externalHandle);
    m_cursorInfo = cursorInfo;
    if (nullptr != cursorName)
        m_cursorInfo.externalHandle = LoadCursor(nullptr, cursorName);
    SetCursor(m_cursorInfo.externalHandle);
}

void WinWebView::dispatchDidReceiveTitle(const String &title)
{
    auto task = [this, newTitle = title.stdUtf8()]
    {
        if (ProcessTitleChange(newTitle))
            return;

        std::wstring ws = zed::multi_byte_to_wide_string(newTitle, CP_UTF8);
        SetWindowTextW(m_hWnd, ws.c_str());
    };
    MessageTask::Post(m_hWnd, std::move(task));
}

void WinWebView::InvalidateNativeView(const IntRect &rect)
{
    RECT rc = { rect.x(), rect.y(), rect.maxX(), rect.maxY() };
    ::InvalidateRect(m_hWnd, &rc, FALSE);
}

void WinWebView::OnChar(HWND hwnd, TCHAR ch, int)
{
    ProcessKeyEvent(WebInputEvent::Char, ch, 0);
}

void WinWebView::OnDPIChanged(HWND hwnd, UINT newDPI, const RECT *rc)
{
    m_dpi = newDPI;
    UpdateScaleFactor();
    SetWindowPos(hwnd, nullptr, rc->left, rc->top, rc->right - rc->left, rc->bottom - rc->top,
        SWP_NOZORDER | SWP_NOACTIVATE);
}

void WinWebView::OnIMEStartComposition(HWND hwnd)
{
    std::optional<POINT> caretPos;
    const auto task = [this, &caretPos] {
        IntRect anchor, focus;
        if (WebViewImpl::SelectionBounds(anchor, focus))
            caretPos = { focus.x(), focus.y() };
    };
    AppImpl::Get().GetAppCaller().SyncCall(BLINK_FROM_HERE, task);

    if (caretPos.has_value())
    {
        COMPOSITIONFORM cf = { 0 };
        cf.dwStyle = CFS_POINT | CFS_FORCE_POSITION;
        cf.ptCurrentPos = caretPos.value();

        HIMC hIMC = ImmGetContext(hwnd);
        ImmSetCompositionWindow(hIMC, &cf);
        ImmReleaseContext(hwnd, hIMC);
    }
}

void WinWebView::OnInitialized(void)
{
    WebViewImpl::OnInitialized();
    UpdateScaleFactor();
}

void WinWebView::OnKey(HWND, UINT vk, BOOL fDown, int, UINT)
{
    WebInputEvent::Type type = fDown ? WebInputEvent::RawKeyDown : WebInputEvent::KeyUp;
    int modifiers = 0;
    if (GetKeyState(VK_SHIFT) < 0)
        modifiers |= WebInputEvent::ShiftKey;
    if (GetKeyState(VK_CONTROL) < 0)
        modifiers |= WebInputEvent::ControlKey;
    if (GetKeyState(VK_MENU) < 0)
        modifiers |= WebInputEvent::AltKey;
    ProcessKeyEvent(type, vk, modifiers);
}

void WinWebView::OnMouse(UINT message, UINT keyFlags, int x, int y)
{
    if (m_changingSizeOrPosition)
        return;

    bool trackLeave = !m_mouseEventSession.MouseEntered();

    WebInputEvent::Type type;
    WebPointerProperties::Button button = WebMouseEvent::ButtonNone;
    switch (message)
    {
        case WM_LBUTTONDOWN:
            type = WebInputEvent::MouseDown;
            button = WebPointerProperties::ButtonLeft;
            SetCapture(m_hWnd);
            break;
        case WM_LBUTTONUP:
            type = WebInputEvent::MouseUp;
            button = WebPointerProperties::ButtonLeft;
            break;
        case WM_MOUSEMOVE:
            type = WebInputEvent::MouseMove;
            if (MK_LBUTTON & keyFlags)
                button = WebPointerProperties::ButtonLeft;
            else if (MK_RBUTTON & keyFlags)
                button = WebPointerProperties::ButtonRight;
            else if (MK_MBUTTON & keyFlags)
                button = WebPointerProperties::ButtonMiddle;
            break;
        case WM_RBUTTONDOWN:
            type = WebInputEvent::MouseDown;
            button = WebPointerProperties::ButtonRight;
            break;
        case WM_RBUTTONUP:
            type = WebInputEvent::MouseUp;
            button = WebPointerProperties::ButtonRight;
            break;
        case WM_MOUSELEAVE:
            type = WebInputEvent::MouseLeave;
            break;
        case WM_MBUTTONDOWN:
            type = WebInputEvent::MouseDown;
            button = WebPointerProperties::ButtonMiddle;
            break;
        case WM_MBUTTONUP:
            type = WebInputEvent::MouseUp;
            button = WebPointerProperties::ButtonMiddle;
            break;
        default:
            NOTREACHED();
            return;
    }

    ProcessMouseEvent(type, button, x, y);

    if (WM_LBUTTONUP == message)
        ReleaseCapture();

    if (trackLeave)
    {
        TRACKMOUSEEVENT tme = { 0 };
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = m_hWnd;
        TrackMouseEvent(&tme);
    }
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
    app.GetAppCaller().SyncCall(BLINK_FROM_HERE, task);

    if (GetClassLong(hwnd, GCL_STYLE) & CS_DBLCLKS)
        webView->m_mouseEventSession.SetHasDoubleClickEvent();
    return TRUE;
}

void WinWebView::OnNCDestroy(HWND hwnd)
{
    m_appCaller.Call(
        BLINK_FROM_HERE,
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
        auto _ = m_canvasLock.guard();
        BitBlt(hdc, x, y, w, h, m_memoryDC, x, y, SRCCOPY);
    }

    EndPaint(hwnd, &ps);
}

void WinWebView::OnShowWindow(HWND, BOOL fShow, UINT)
{
    PageVisibilityState state = GetPageVisibilityState(fShow);
    WebViewImpl::SetVisibilityState(state);
}

void WinWebView::OnSize(HWND, UINT state, int cx, int cy)
{
    if (SIZE_MINIMIZED == state)
        return;

    auto task = [this, cx, cy]
    {
        Resize(IntSize(cx, cy));
        UpdateAndPaint();
    };
    m_appCaller.Call(BLINK_FROM_HERE, std::move(task));
}

SkBitmap WinWebView::PrepareBitmapForCanvas(const IntSize &size)
{
    BkBitmap ret;
    HBITMAP hBitmap = ret.InstallDIBSection(size.width(), size.height(), m_memoryDC);
    HBITMAP oldBitmap = SelectBitmap(m_memoryDC, hBitmap);
    if (nullptr == m_oldBitmap)
        m_oldBitmap = oldBitmap;
    return ret;
}

bool WinWebView::ProcessWindowMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT *result)
{
    // MessageLogger _(Msg);
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

        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MOUSEMOVE:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
            OnMouse(Msg, wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            break;

        case WM_SETCURSOR:
        {
            const UINT hitTestCode = LOWORD(lParam);
            if (HTCLIENT != hitTestCode)
                return false;

            auto _ = m_lock.guard_shared();
            SetCursor(m_cursorInfo.externalHandle);
            *result = TRUE;
            break;
        }

        case WM_KEYDOWN:
            HANDLE_WM_KEYDOWN(hWnd, wParam, lParam, OnKey);
            break;
        case WM_KEYUP:
            HANDLE_WM_KEYUP(hWnd, wParam, lParam, OnKey);
            break;
        case WM_CHAR:
            HANDLE_WM_CHAR(hWnd, wParam, lParam, OnChar);
            break;
        case WM_IME_STARTCOMPOSITION:
            OnIMEStartComposition(hWnd);
            break;

        case WM_SIZE:
            HANDLE_WM_SIZE(hWnd, wParam, lParam, OnSize);
            break;
        case WM_ENTERSIZEMOVE:
            m_changingSizeOrPosition = true;
            break;
        case WM_EXITSIZEMOVE:
            m_changingSizeOrPosition = false;
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
