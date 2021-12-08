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

struct BkWebViewClient;

namespace BlinKit {

class LayerTreeHost;

class WebViewHostWindow final : public WebViewHost
{
public:
    WebViewHostWindow(const BkWebViewClient &client, HWND hWnd, LPCREATESTRUCT cs);
    ~WebViewHostWindow(void);

    HWND GetHWND(void) const { return m_hWnd; }
    WebViewImpl* GetView(void) const { return m_view; }

    void Redraw(const blink::IntRect &rect);
    void StartAnimationTimer(double delay) {
        //m_animationTimer.startOneShot(delay, BLINK_FROM_HERE);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Exports
    static bool ProcessWindowMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT *result);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
    std::unique_ptr<SkCanvas> CreateCanvas(int width, int height);
    void UpdateScaleFactor(void);

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

    void OnAnimationTimer(blink::Timer<WebViewHostWindow> *);

    // WebViewHost
    blink::WebLayerTreeView* GetLayerTreeView(void) const override;
    void Invalidate(const blink::IntRect &rect) override;
    void ScheduleAnimation(void) override;
    void ChangeTitle(const std::string &title) override;
    void DidChangeCursor(const blink::WebCursorInfo &cursorInfo) override;
    void ShowContextMenu(const blink::WebContextMenuData &data) override;

    HWND m_hWnd;
    WebViewImpl *m_view;
    UINT m_dpi = 96;

    std::unique_ptr<LayerTreeHost> m_layerTreeHost;
    std::unique_ptr<SkCanvas> m_canvas;
    HDC m_memoryDC = nullptr;
    HBITMAP m_oldBitmap = nullptr;

    MouseSession m_mouseSession;
    // PaintSession m_paintSession;

    const std::shared_ptr<bool> m_hostAliveFlag;
    blink::Timer<WebViewHostWindow> m_animationTimer;

    bool m_animationTaskScheduled = false;
    bool m_changingSizeOrPosition = false;
    blink::WebCursorInfo m_cursorInfo;

    friend class ScopedPaintSession;
    ScopedPaintSession *m_currentPaintSession = nullptr;
    class ScopedAnimationScheduler;
    bool m_animationScheduled = false;
    unsigned m_animationScheduledTimes = 0;
};

} // namespace BlinKit

#endif // BLINKIT_WEB_VIEW_HOST_WINDOW_H
