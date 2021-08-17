// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_web_view.h
// Description: WinWebView Class
//      Author: Ziming Li
//     Created: 2020-10-16
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_WIN_WEB_VIEW_H
#define BLINKIT_BLINKIT_WIN_WEB_VIEW_H

#pragma once

#include "blinkit/ui/web_view_impl.h"

namespace BlinKit {

class WinWebView final : public WebViewImpl
{
public:
    WinWebView(HWND hWnd, ClientCaller &clientCaller, bool isWindowVisible);
    ~WinWebView(void);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Exports
    static bool ProcessWindowMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT *result);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
    void UpdateScaleFactor(void);

    bool ProcessWindowMessageImpl(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT *result);
    void OnDPIChanged(HWND hwnd, UINT newDPI, const RECT *rc);
    static BOOL OnNCCreate(HWND hwnd, LPCREATESTRUCT cs);
    void OnNCDestroy(HWND hwnd);
    void OnPaint(HWND hwnd);
    void OnShowWindow(HWND hwnd, BOOL fShow, UINT status);
    void OnSize(HWND hwnd, UINT state, int cx, int cy);

    // FrameLoaderClient
    void dispatchDidReceiveTitle(const String &title) override;
    // WebViewImpl
    void InvalidateNativeView(const blink::IntRect &rect) override;
    SkBitmap PrepareBitmapForCanvas(const blink::IntSize &size) override;

    HWND m_hWnd;
    HDC m_memoryDC = nullptr;
    HBITMAP m_oldBitmap = nullptr;
    UINT m_dpi = 96;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_WIN_WEB_VIEW_H
