// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_web_view_host.cpp
// Description: WinWebViewHost Class
//      Author: Ziming Li
//     Created: 2020-10-16
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "./win_web_view_host.h"

#include <windowsx.h>
#include "blinkit/app/win_app.h"
#include "blinkit/blink/renderer/core/editing/Editor.h"
#include "blinkit/blink/renderer/platform/Task.h"
#include "blinkit/ui/win_context_menu_controller.h"
#include "blinkit/ui/web_view_impl.h"
#include "blinkit/win/bk_bitmap.h"
#include "blinkit/win/message_task.h"
#include "third_party/zed/include/zed/float.hpp"
#include "third_party/zed/include/zed/log.hpp"

using namespace blink;

namespace BlinKit {

static std::unordered_map<HWND, WinWebViewHost *> g_hosts;
static constexpr double AnimationFrameLimit = 1.0 / 24.0;

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

static SkColor WindowColor(void)
{
    COLORREF color = GetSysColor(COLOR_WINDOW);
    return SkColorSetARGB(0xff, GetRValue(color), GetGValue(color), GetBValue(color));
}

WinWebViewHost::WinWebViewHost(const BkWebViewClient &client, HWND hWnd, LPCREATESTRUCT cs)
    : m_hWnd(hWnd)
    , m_view(new WebViewImpl(client, GetPageVisibilityState(cs->style), WindowColor()))
    , m_hostAliveFlag(std::make_shared<bool>(true))
    , m_animationTimer(this, &WinWebViewHost::OnAnimationTimer)
{
    g_hosts.emplace(m_hWnd, this);

    m_animationTimer.SetHostAliveFlag(m_hostAliveFlag);

    HDC dc = GetDC(m_hWnd);
    m_dpi = GetDeviceCaps(dc, LOGPIXELSY);
    ASSERT(0 != m_dpi);
    ASSERT(GetDeviceCaps(dc, LOGPIXELSX) == m_dpi);
    m_memoryDC = CreateCompatibleDC(dc);
    ReleaseDC(m_hWnd, dc);

    m_cursorInfo.externalHandle = LoadCursor(nullptr, IDC_ARROW);

    if (GetClassLong(m_hWnd, GCL_STYLE) & CS_DBLCLKS)
        m_mouseSession.EnableDoubleClick();

    m_view->Initialize(this);
    UpdateScaleFactor();
}

WinWebViewHost::~WinWebViewHost(void)
{
    if (nullptr != m_oldBitmap)
        SelectBitmap(m_memoryDC, m_oldBitmap);
    DeleteDC(m_memoryDC);

    if (nullptr != m_hWnd)
        g_hosts.erase(m_hWnd);
    delete m_view;
}

void WinWebViewHost::ChangeTitle(const std::string &title)
{
    std::wstring ws = zed::multi_byte_to_wide_string(title, CP_UTF8);
    SetWindowTextW(m_hWnd, ws.c_str());
}

std::unique_ptr<SkCanvas> WinWebViewHost::CreateCanvas(int width, int height)
{
    BkBitmap bitmap;

    ASSERT(width > 0);
    ASSERT(height > 0);

    HBITMAP hBitmap = bitmap.InstallDIBSection(width, height, m_memoryDC);
    HBITMAP oldBitmap = SelectBitmap(m_memoryDC, hBitmap);
    if (nullptr == m_oldBitmap)
        m_oldBitmap = oldBitmap;
    return std::make_unique<SkCanvas>(bitmap);
}

void WinWebViewHost::DidChangeCursor(const WebCursorInfo &cursorInfo)
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

void WinWebViewHost::Invalidate(const IntRect &rect)
{
    m_paintSession.UniteDamagedRect(rect);
}

void WinWebViewHost::OnAnimationTimer(Timer<WinWebViewHost> *)
{
    ASSERT(false);
}

void WinWebViewHost::OnChar(HWND hwnd, TCHAR ch, int)
{
    m_view->ProcessKeyEvent(WebInputEvent::Char, ch, 0);
}

void WinWebViewHost::OnDPIChanged(HWND hwnd, UINT newDPI, const RECT *rc)
{
    m_dpi = newDPI;
    UpdateScaleFactor();
    SetWindowPos(hwnd, nullptr, rc->left, rc->top, rc->right - rc->left, rc->bottom - rc->top,
        SWP_NOZORDER | SWP_NOACTIVATE);
}

void WinWebViewHost::OnIMEStartComposition(HWND hwnd)
{
    IntRect anchor, focus;
    if (m_view->SelectionBounds(anchor, focus))
    {
        COMPOSITIONFORM cf = { 0 };
        cf.dwStyle = CFS_POINT | CFS_FORCE_POSITION;
        cf.ptCurrentPos = { focus.x(), focus.y() };

        HIMC hIMC = ImmGetContext(hwnd);
        ImmSetCompositionWindow(hIMC, &cf);
        ImmReleaseContext(hwnd, hIMC);
    }
}

void WinWebViewHost::OnKey(HWND, UINT vk, BOOL fDown, int, UINT)
{
    WebInputEvent::Type type = fDown ? WebInputEvent::RawKeyDown : WebInputEvent::KeyUp;
    int modifiers = 0;
    if (GetKeyState(VK_SHIFT) < 0)
        modifiers |= WebInputEvent::ShiftKey;
    if (GetKeyState(VK_CONTROL) < 0)
        modifiers |= WebInputEvent::ControlKey;
    if (GetKeyState(VK_MENU) < 0)
        modifiers |= WebInputEvent::AltKey;
    m_view->ProcessKeyEvent(type, vk, modifiers);
}

void WinWebViewHost::OnMouse(UINT message, WPARAM wParam, LPARAM lParam)
{
    if (m_changingSizeOrPosition)
        return;
    m_paintSession.Begin(*this);

    const auto callback = [this](const MouseEvent &e) {
        m_view->ProcessMouseEvent(e);
    };
    m_mouseSession.Process(m_hWnd, message, wParam, lParam, callback);

    if (m_paintSession.IsAttached())
    {
        m_paintSession.DetachFromScheduler();

        m_paintSession.Flush(*this);
        UpdateWindow(m_hWnd);
    }
}

BOOL WinWebViewHost::OnNCCreate(HWND hwnd, LPCREATESTRUCT cs)
{
    BkWebViewClient *client = reinterpret_cast<BkWebViewClient *>(cs->lpCreateParams);
    if (nullptr == client)
    {
        ASSERT(nullptr != client);
        return FALSE;
    }

    new WinWebViewHost(*client, hwnd, cs);
    return TRUE;
}

void WinWebViewHost::OnNCDestroy(HWND hwnd)
{
    delete this;
}

void WinWebViewHost::OnPaint(HWND hwnd)
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

void WinWebViewHost::OnShowWindow(HWND, BOOL fShow, UINT)
{
    m_view->SetVisibilityState(fShow ? PageVisibilityStateVisible : PageVisibilityStateHidden);
}

void WinWebViewHost::OnSize(HWND, UINT state, int cx, int cy)
{
    if (SIZE_MINIMIZED == state)
        return;

    bool resetCanvas = true;
    if (m_canvas)
    {
        SkImageInfo imageInfo = m_canvas->imageInfo();
        if (imageInfo.width() != cx && imageInfo.height() != cy)
            resetCanvas = false;
    }
    if (resetCanvas)
    {
        m_canvas = CreateCanvas(cx, cy);
        m_canvas->drawColor(m_view->BaseBackgroundColor());
    }
    m_view->Resize(IntSize(cx, cy));
}

bool WinWebViewHost::ProcessWindowMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT *result)
{
    // MessageLogger _(Msg);
    if (WM_NCCREATE == Msg)
    {
        *result = HANDLE_WM_NCCREATE(hWnd, wParam, lParam, OnNCCreate);
        return true;
    }

    WinWebViewHost *host = zed::query_value(g_hosts, hWnd, nullptr);
    if (nullptr == host)
        return false;
    return host->ProcessWindowMessageImpl(hWnd, Msg, wParam, lParam, result);
}

bool WinWebViewHost::ProcessWindowMessageImpl(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT *result)
{
    switch (Msg)
    {
        case WM_ERASEBKGND:
            *result = FALSE;
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
            *result = TRUE;
            break;
        }

        case WM_LBUTTONDOWN:
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

        case WM_SETFOCUS:
            m_view->SetFocus(true);
            break;
        case WM_KILLFOCUS:
            m_view->SetFocus(false);
            break;

        case WM_ENTERSIZEMOVE:
            m_changingSizeOrPosition = true;
            break;
        case WM_EXITSIZEMOVE:
            m_changingSizeOrPosition = false;
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
            return false;

        default:
            if (!MessageTask::Process(Msg, wParam, lParam))
                return false;
    }

    return true;
}

void WinWebViewHost::Redraw(const IntRect &rect)
{
    m_view->PaintContent(m_canvas.get(), rect);
}

void WinWebViewHost::ScheduleAnimation(void)
{
    if (!m_paintSession.IsAttached())
        m_paintSession.AttachToScheduler(*this);
}

#if 0 // BKTODO:
void WinWebViewHost::ScheduleAnimationTaskIfNecessary(void)
{
    if (m_changingSizeOrPosition || m_animationTaskScheduled)
        return;

    double delta = m_paintSession.TimeDeltaSinceLastLeft();
    double delay = std::max(0.0, AnimationFrameLimit - delta);
    ScheduleNextAnimationTask(delay);
}

void WinWebViewHost::ScheduleNextAnimationTask(double delay)
{
    ASSERT(!m_animationTaskScheduled);
    m_animationTaskScheduled = true;

    if (!zed::almost_equals(delay, 0.0))
        delay *= base::Time::kMillisecondsPerSecond;
    auto task = std::bind(&WinWebViewHost::ScheduledAnimationTask, this);
    AppImpl::Get().taskRunner()->postDelayedTask(BLINK_FROM_HERE, new Task(std::move(task)), delay);
}

void WinWebViewHost::ScheduledAnimationTask(void)
{
    double delta = m_paintSession.TimeDeltaSinceLastLeft();
    if (delta < AnimationFrameLimit)
    {
        double delay = AnimationFrameLimit - delta;
        if (delay > 0.01)
        {
            m_animationTaskScheduled = false;
            ScheduleNextAnimationTask(delay);
            return;
        }
    }

    ASSERT(m_animationTaskScheduled);
    m_animationTaskScheduled = false;

    m_paintSession.Update(*m_view);
}
#endif

void WinWebViewHost::ShowContextMenu(const WebContextMenuData &data)
{
    WinContextMenuController &menuController = WinApp::Get().ContextMenuController();

    POINT pt;
    GetCursorPos(&pt);

    HMENU hPopupMenu = menuController.RequireMenu(data.editFlags);

    SetForegroundWindow(m_hWnd);
    UINT returnCmd = TrackPopupMenu(hPopupMenu, TPM_RETURNCMD, pt.x, pt.y, 0, m_hWnd, nullptr);
    if (0 == returnCmd)
        return;

    if (CommandID::SelectAll == returnCmd)
    {
        m_view->GetFrame().selection().selectAll();
        return;
    }

    WinContextMenuController::EditorFunction pfn = nullptr;
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
    menuController.RunEditorFunction(m_view->GetFrame().editor(), pfn);
}

void WinWebViewHost::StartAnimationTimer(double delay)
{
    ASSERT(false);
}

void WinWebViewHost::UpdateScaleFactor(void)
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
    m_view->SetScaleFactor(scaleFactor);
}

} // namespace BlinKit

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace BlinKit;

extern "C" {

BKEXPORT LRESULT CALLBACK BkDefWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT r = 0;
    bool handled = WinWebViewHost::ProcessWindowMessage(hWnd, Msg, wParam, lParam, &r);
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
    if (WinWebViewHost *host = zed::query_value(g_hosts, hWnd, nullptr))
        return host->GetView();
    return nullptr;
}

BKEXPORT bool_t BKAPI BkProcessWindowMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT *result)
{
    return WinWebViewHost::ProcessWindowMessage(hWnd, Msg, wParam, lParam, result);
}

} // extern "C"
