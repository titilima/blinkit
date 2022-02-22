// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: web_view_host_window.cpp
// Description: WebViewHostWindow Class
//      Author: Ziming Li
//     Created: 2020-10-16
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "./web_view_host_window.h"

#include <windowsx.h>
#include "blinkit/app/win_app.h"
#include "blinkit/blink/renderer/core/editing/Editor.h"
#include "blinkit/ui/animation/animation_frame.h"
#include "blinkit/ui/compositor/compositor.h"
#include "blinkit/ui/web_view_impl.h"
#include "blinkit/win/context_menu_controller.h"

using namespace blink;

namespace BlinKit {

static std::unordered_map<HWND, WebViewHostWindow *> g_hosts;

#ifndef NDEBUG
class MessageLogger
{
public:
    MessageLogger(const UINT msg) : m_msg(msg)
    {
        BKLOG("ProcessWindowMessage begin: 0x%04X at %d", m_msg, GetTickCount());
    }
    ~MessageLogger(void)
    {
        BKLOG("ProcessWindowMessage end: 0x%04X at %d", m_msg, GetTickCount());
    }
private:
    const UINT m_msg;
};
#endif

static PageVisibilityState GetPageVisibilityState(LONG style)
{
    return 0 != (style & WS_VISIBLE) ? PageVisibilityStateVisible : PageVisibilityStateHidden;
}

static constexpr UINT RESIZING_TIMER_ELAPSE = 100;
static constexpr DWORD RESIZING_COMMIT_ELAPSE = 500;
std::unordered_map<UINT_PTR, WebViewHostWindow *> WebViewHostWindow::m_resizingHosts;

WebViewHostWindow::WebViewHostWindow(const BkWebViewClient &client, HWND hWnd, LPCREATESTRUCT cs)
    : WebViewHost(client, GetPageVisibilityState(cs->style))
    , m_hWnd(hWnd)
    , m_hostAliveFlag(std::make_shared<bool>(true))
    , m_animationTimer(this, &WebViewHostWindow::OnAnimationTimer)
{
    g_hosts.emplace(m_hWnd, this);

    m_animationTimer.SetHostAliveFlag(m_hostAliveFlag);

    m_cursorInfo.externalHandle = LoadCursor(nullptr, IDC_ARROW);

    if (GetClassLong(m_hWnd, GCL_STYLE) & CS_DBLCLKS)
        m_mouseSession.EnableDoubleClick();
}

WebViewHostWindow::~WebViewHostWindow(void)
{
    AppImpl::Get().GetCompositor().Detach(this);

    if (nullptr != m_oldBitmap)
        SelectBitmap(m_memoryDC, m_oldBitmap);
    DeleteDC(m_memoryDC);

    g_hosts.erase(m_hWnd);
    m_hWnd = nullptr;
}

void WebViewHostWindow::AdjustUpdateWhileResizing(DWORD tick)
{
    if (tick - m_resizingTick < RESIZING_COMMIT_ELAPSE)
        return;

    m_resizingTick = tick;
    CommitAnimationImmediately();
}

WebViewHostWindow* WebViewHostWindow::CreateInstance(HWND hWnd, LPCREATESTRUCT cs)
{
    BkWebViewClient *client = reinterpret_cast<BkWebViewClient *>(cs->lpCreateParams);
    if (nullptr != client)
        return new WebViewHostWindow(*client, hWnd, cs);

    ASSERT(nullptr != client);
    return nullptr;
}

void WebViewHostWindow::DidChangeCursor(const WebCursorInfo &cursorInfo)
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

    if (WebCursorInfo::TypeCustom == m_cursorInfo.type)
        DestroyCursor(m_cursorInfo.externalHandle);
    m_cursorInfo = cursorInfo;
    if (nullptr != cursorName)
        m_cursorInfo.externalHandle = LoadCursor(nullptr, cursorName);
    ::SetCursor(m_cursorInfo.externalHandle);
}

void WebViewHostWindow::FlushFrame(const SkBitmap &bitmap, const IntPoint &position, const IntSize &size, const SkPaint &paint)
{
    IntRect dirtyRect(position, size);

    auto _ = m_paintLock.guard();

    SkCanvas *canvas = m_currentFrame->GetCanvas();
    SkIRect src = SkIRect::MakeWH(size.width(), size.height());
    canvas->drawBitmapRect(bitmap, src, dirtyRect, &paint);

    MakeBitBlt(dirtyRect);
}

bool WebViewHostWindow::ForwardMessageToClient(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT &result)
{
    if (nullptr == m_client.ProcessMessage)
        return false;
    return m_client.ProcessMessage(hWnd, Msg, wParam, lParam, &result, m_client.UserData);
}

void WebViewHostWindow::InitializeCanvas(HDC hdc, int cx, int cy)
{
    m_memoryDC = CreateCompatibleDC(hdc);

    cx = std::max(cx, 1);
    cy = std::max(cy, 1);
    m_currentFrame = std::make_unique<AnimationFrame>(IntSize(cx, cy));
    m_currentFrame->GetCanvas()->clear(DefaultBackgroundColor());

    m_oldBitmap = SelectBitmap(m_memoryDC, *m_currentFrame);
}

void WebViewHostWindow::KillResizingTimer(void)
{
    ASSERT(0 != m_resizingTimerId);
    KillTimer(nullptr, m_resizingTimerId);
    m_resizingHosts.erase(m_resizingTimerId);
    m_resizingTimerId = 0;
}

void WebViewHostWindow::MakeBitBlt(const IntRect &rect)
{
    HDC hdc = GetDC(m_hWnd);
    BitBlt(hdc, rect.x(), rect.y(), rect.width(), rect.height(), m_memoryDC, rect.x(), rect.y(), SRCCOPY);
    ReleaseDC(m_hWnd, hdc);
}

void WebViewHostWindow::OnAnimationTimer(Timer<WebViewHostWindow> *)
{
#if 0
    m_paintSession.AttachToScheduler(*this);
    m_paintSession.Begin(*this);
    m_paintSession.Flush(*this);
    m_paintSession.DetachFromScheduler();
#endif
}

void WebViewHostWindow::OnChar(HWND hwnd, TCHAR ch, int)
{
    GetView()->ProcessKeyEvent(WebInputEvent::Char, ch, 0);
}

BOOL WebViewHostWindow::OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    RECT rc;
    GetClientRect(hwnd, &rc);

    HDC dc = GetDC(hwnd);
    UINT dpi = GetDeviceCaps(dc, LOGPIXELSY);
    ASSERT(0 != dpi && GetDeviceCaps(dc, LOGPIXELSX) == dpi);
    InitializeCanvas(dc, rc.right, rc.bottom);
    ReleaseDC(hwnd, dc);

    InitializeView(ScaleFactorFromDPI(dpi));
    return TRUE;
}

void WebViewHostWindow::OnDPIChanged(HWND hwnd, UINT newDPI, const RECT *rc)
{
    SetScaleFactor(ScaleFactorFromDPI(newDPI));
    SetWindowPos(hwnd, nullptr, rc->left, rc->top, rc->right - rc->left, rc->bottom - rc->top,
        SWP_NOZORDER | SWP_NOACTIVATE);
}

void WebViewHostWindow::OnIMEStartComposition(HWND hwnd)
{
    IntRect anchor, focus;
    if (GetView()->SelectionBounds(anchor, focus))
    {
        COMPOSITIONFORM cf = { 0 };
        cf.dwStyle = CFS_POINT | CFS_FORCE_POSITION;
        cf.ptCurrentPos = { focus.x(), focus.y() };

        HIMC hIMC = ImmGetContext(hwnd);
        ImmSetCompositionWindow(hIMC, &cf);
        ImmReleaseContext(hwnd, hIMC);
    }
}

void WebViewHostWindow::OnKey(HWND, UINT vk, BOOL fDown, int, UINT)
{
    WebInputEvent::Type type = fDown ? WebInputEvent::RawKeyDown : WebInputEvent::KeyUp;
    int modifiers = 0;
    if (GetKeyState(VK_SHIFT) < 0)
        modifiers |= WebInputEvent::ShiftKey;
    if (GetKeyState(VK_CONTROL) < 0)
        modifiers |= WebInputEvent::ControlKey;
    if (GetKeyState(VK_MENU) < 0)
        modifiers |= WebInputEvent::AltKey;
    GetView()->ProcessKeyEvent(type, vk, modifiers);
}

void WebViewHostWindow::OnKillFocus(HWND hwnd, HWND hwndNewFocus)
{
    WebViewImpl *v = GetView();
    v->SetIsActive(false);
    v->SetFocus(false);
}

void WebViewHostWindow::OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
    if (GetFocus() != hwnd)
        SetFocus(hwnd);
}

void WebViewHostWindow::OnMouse(UINT message, WPARAM wParam, LPARAM lParam)
{
    if (m_changingSizeOrPosition)
        return;

    const auto callback = [this](const MouseEvent &e) {
        GetView()->ProcessMouseEvent(e);
    };
    m_mouseSession.Process(m_hWnd, message, wParam, lParam, callback);
}

int WebViewHostWindow::OnMouseActivate(HWND hwnd, HWND hwndTopLevel, UINT codeHitTest, UINT msg)
{
    SetFocus(hwnd);
    return MA_ACTIVATE;
}

bool WebViewHostWindow::OnNCCreate(WPARAM wParam, LPARAM lParam, LRESULT &result)
{
    result = TRUE;
    return ForwardMessageToClient(m_hWnd, WM_NCCREATE, wParam, lParam, result);
}

bool WebViewHostWindow::OnNCDestroy(HWND hwnd, WPARAM wParam, LPARAM lParam, LRESULT &result)
{
    auto pfn = m_client.ProcessMessage;
    void *ud = m_client.UserData;

    delete this;
    if (nullptr == pfn)
        return false;

    result = 0;
    return pfn(hwnd, WM_NCDESTROY, wParam, lParam, &result, ud);
}

void WebViewHostWindow::OnPaint(HWND hwnd)
{
    ASSERT(m_hWnd == hwnd);

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    int x = ps.rcPaint.left;
    int y = ps.rcPaint.top;
    int cx = ps.rcPaint.right - ps.rcPaint.top;
    int cy = ps.rcPaint.bottom - ps.rcPaint.top;
    if (auto _ = m_paintLock.guard())
        BitBlt(hdc, x, y, cx, cy, m_memoryDC, x, y, SRCCOPY);

    EndPaint(hwnd, &ps);
}

void WebViewHostWindow::OnSetFocus(HWND hwnd, HWND hwndOldFocus)
{
    WebViewImpl *v = GetView();
    v->SetFocus(true);
    v->SetIsActive(true);
}

void WebViewHostWindow::OnShowWindow(HWND, BOOL fShow, UINT)
{
    GetView()->SetVisibilityState(fShow ? PageVisibilityStateVisible : PageVisibilityStateHidden);
}

void WebViewHostWindow::OnSize(HWND, UINT state, int cx, int cy)
{
    if (SIZE_MINIMIZED == state)
        return;

    m_resizingTick = GetTickCount();
    Resize(IntSize(cx, cy));
}

bool WebViewHostWindow::ProcessMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT &result)
{
    bool handled = true;
    switch (Msg)
    {
        case WM_ERASEBKGND:
            result = FALSE;
            break;
        case WM_PAINT:
            HANDLE_WM_PAINT(hWnd, wParam, lParam, OnPaint);
            break;

        case WM_SETCURSOR:
        {
            const UINT hitTestCode = LOWORD(lParam);
            if (HTCLIENT != hitTestCode)
                return false;

            ::SetCursor(m_cursorInfo.externalHandle);
            result = TRUE;
            break;
        }

        case WM_LBUTTONDOWN:
            HANDLE_WM_LBUTTONDOWN(hWnd, wParam, lParam, OnLButtonDown);
            [[fallthrough]];
        case WM_LBUTTONUP:
        case WM_MOUSEMOVE:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
            OnMouse(Msg, wParam, lParam);
            break;

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

        case WM_MOUSEACTIVATE:
            result = HANDLE_WM_MOUSEACTIVATE(hWnd, wParam, lParam, OnMouseActivate);
            break;
        case WM_SETFOCUS:
            HANDLE_WM_SETFOCUS(hWnd, wParam, lParam, OnSetFocus);
            break;
        case WM_KILLFOCUS:
            HANDLE_WM_KILLFOCUS(hWnd, wParam, lParam, OnKillFocus);
            break;

        case WM_ENTERSIZEMOVE:
            m_changingSizeOrPosition = true;
            SetResizingTimer();
            break;
        case WM_EXITSIZEMOVE:
            KillResizingTimer();
            m_changingSizeOrPosition = false;
            break;

        case WM_SHOWWINDOW:
            HANDLE_WM_SHOWWINDOW(hWnd, wParam, lParam, OnShowWindow);
            break;

        case WM_DPICHANGED:
            ASSERT(HIWORD(wParam) == LOWORD(lParam));
            OnDPIChanged(hWnd, HIWORD(wParam), reinterpret_cast<LPRECT>(lParam));
            break;
        case WM_CREATE:
            result = HANDLE_WM_CREATE(hWnd, wParam, lParam, OnCreate);
            break;
        case WM_NCCREATE:
            return OnNCCreate(wParam, lParam, result);
        case WM_NCDESTROY:
            return OnNCDestroy(hWnd, wParam, lParam, result);
        default:
            handled = false;
    }

    if (!ForwardMessageToClient(hWnd, Msg, wParam, lParam, result))
        return handled;
    return true;
}

void WebViewHostWindow::ProcessTitleChange(const String &title)
{
    if (nullptr == m_client.TitleChange)
        return;

    std::string newTitle = title.stdUtf8();
    if (m_client.TitleChange(GetView(), newTitle.c_str(), m_client.UserData))
        return;

    std::wstring ws = zed::multi_byte_to_wide_string(newTitle, CP_UTF8);
    SetWindowTextW(m_hWnd, ws.c_str());
}

void CALLBACK WebViewHostWindow::ResizingTimerProc(HWND, UINT, UINT_PTR timerId, DWORD tick)
{
    auto it = m_resizingHosts.find(timerId);
    ASSERT(m_resizingHosts.end() != it);
    it->second->AdjustUpdateWhileResizing(tick);
}

float WebViewHostWindow::ScaleFactorFromDPI(UINT dpi)
{
    switch (dpi)
    {
        case 120:
            return 1.25;
        case 144:
            return 1.5;
        case 192:
            return 2.0;
    }
    ASSERT(USER_DEFAULT_SCREEN_DPI == dpi);
    return 1.0;
}

void WebViewHostWindow::SetResizingTimer(void)
{
    ASSERT(0 == m_resizingTimerId);
    m_resizingTimerId = SetTimer(nullptr, 0, RESIZING_TIMER_ELAPSE, ResizingTimerProc);
    m_resizingHosts[m_resizingTimerId] = this;
}

void WebViewHostWindow::ShowContextMenu(const WebContextMenuData &data)
{
    ContextMenuController &menuController = WinApp::Get().GetContextMenuController();

    POINT pt;
    GetCursorPos(&pt);

    HMENU hPopupMenu = menuController.RequireMenu(data.editFlags);

    SetForegroundWindow(m_hWnd);
    UINT returnCmd = TrackPopupMenu(hPopupMenu, TPM_RETURNCMD, pt.x, pt.y, 0, m_hWnd, nullptr);
    if (0 == returnCmd)
        return;

    if (CommandID::SelectAll == returnCmd)
    {
        GetView()->GetFrame().selection().selectAll();
        return;
    }

    ContextMenuController::EditorFunction pfn = nullptr;
    switch (returnCmd)
    {
        case CommandID::Undo:
            pfn = &Editor::undo;
            break;
        case CommandID::Cut:
            pfn = &Editor::cut;
            break;
        case CommandID::Copy:
            pfn = &Editor::copy;
            break;
        case CommandID::Paste:
            pfn = &Editor::pasteAsPlainText;
            break;
        case CommandID::Delete:
            pfn = &Editor::performDelete;
            break;
        default:
            ASSERT_NOT_REACHED();
            return;
    }
    menuController.RunEditorFunction(GetView()->GetFrame().editor(), pfn);
}

void WebViewHostWindow::SwapFrame(std::unique_ptr<AnimationFrame> &frame, const IntSize &size)
{
    IntRect dirtyRect(IntPoint(), size);

    auto _ = m_paintLock.guard();

    m_currentFrame.swap(frame);
    SelectBitmap(m_memoryDC, *m_currentFrame);

    MakeBitBlt(dirtyRect);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SkColor WebViewHost::DefaultBackgroundColor(void)
{
    COLORREF color = GetSysColor(COLOR_WINDOW);
    return SkColorSetARGB(0xff, GetRValue(color), GetGValue(color), GetBValue(color));
}

} // namespace BlinKit

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace BlinKit;

extern "C" {

BKEXPORT LRESULT CALLBACK BkDefWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    do {
        WebViewHostWindow *host = zed::query_value(g_hosts, hWnd, nullptr);
        if (nullptr == host)
        {
            if (WM_NCCREATE != Msg)
                break;

            host = WebViewHostWindow::CreateInstance(hWnd, reinterpret_cast<LPCREATESTRUCT>(lParam));
            if (nullptr == host)
                return FALSE;
        }

        LRESULT r = 0;
        if (host->ProcessMessage(hWnd, Msg, wParam, lParam, r))
            return r;
    } while (false);

    WNDPROC defProc = IsWindowUnicode(hWnd) ? DefWindowProcW : DefWindowProcA;
    return defProc(hWnd, Msg, wParam, lParam);
}

BKEXPORT BkWebView BKAPI BkGetWebView(HWND hWnd)
{
    if (WebViewHostWindow *host = zed::query_value(g_hosts, hWnd, nullptr))
        return host->GetView();
    return nullptr;
}

} // extern "C"
