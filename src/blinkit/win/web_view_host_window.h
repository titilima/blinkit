#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: web_view_host_window.h
// Description: WebViewHostWindow Class
//      Author: Ziming Li
//     Created: 2020-10-16
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_WEB_VIEW_HOST_WINDOW_H
#define BLINKIT_WEB_VIEW_HOST_WINDOW_H

#include "blinkit/blink/renderer/platform/Timer.h"
#include "blinkit/ui/web_view_host.h"
#include "blinkit/win/mouse_session.h"
#include "third_party/zed/include/zed/mutex.hpp"

struct BkWebViewClient;

namespace BlinKit {

class WebViewHostWindow final : public WebViewHost
{
public:
    WebViewHostWindow(const BkWebViewClient &client, HWND hWnd, LPCREATESTRUCT cs);
    ~WebViewHostWindow(void) override;

    HWND GetHWND(void) const { return m_hWnd; }

    void Redraw(const IntRect &rect);
    void StartAnimationTimer(double delay) {
        //m_animationTimer.startOneShot(delay, BLINK_FROM_HERE);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Exports
    static bool ProcessWindowMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT *result);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
    void InitializeCanvas(HDC hdc);
    static float ScaleFactorFromDPI(UINT dpi);

    bool ProcessWindowMessageImpl(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT *result);

    void OnChar(HWND hwnd, TCHAR ch, int cRepeat);
    void OnDPIChanged(HWND hwnd, UINT newDPI, const RECT *rc);
    void OnIMEStartComposition(HWND hwnd);
    void OnKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);
    void OnMouse(UINT message, WPARAM wParam, LPARAM lParam);
    static BOOL OnNCCreate(HWND hwnd, LPCREATESTRUCT cs);
    void OnNCDestroy(void);
    void OnPaint(HWND hwnd);
    void OnShowWindow(HWND hwnd, BOOL fShow, UINT status);
    void OnSize(HWND hwnd, UINT state, int cx, int cy);

    void OnAnimationTimer(Timer<WebViewHostWindow> *);

    // WebViewHost
    void Invalidate(const IntRect &rect) override;
    void ScheduleAnimation(void) override;
    std::unique_ptr<PaintUITask> PreparePaintTask(void) override;
    void ChangeTitle(const std::string &title) override;
    void DidChangeCursor(const WebCursorInfo &cursorInfo) override;
    void ShowContextMenu(const WebContextMenuData &data) override;

    HWND m_hWnd;

    zed::mutex m_canvasLock;
    std::unique_ptr<SkCanvas> m_canvas;
    HDC m_memoryDC = nullptr;
    HBITMAP m_oldBitmap = nullptr;

    MouseSession m_mouseSession;
    // PaintSession m_paintSession;

    const std::shared_ptr<bool> m_hostAliveFlag;
    Timer<WebViewHostWindow> m_animationTimer;

    bool m_animationTaskScheduled = false;
    bool m_changingSizeOrPosition = false;
    WebCursorInfo m_cursorInfo;

    friend class ScopedPaintSession;
    class ScopedAnimationScheduler;
    bool m_animationScheduled = false;
    unsigned m_animationScheduledTimes = 0;
};

} // namespace BlinKit

#endif // BLINKIT_WEB_VIEW_HOST_WINDOW_H
