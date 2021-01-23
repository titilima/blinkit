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
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Exports
    static WinWebView* Lookup(HWND hWnd);
    static bool ProcessWindowMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT *result);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
    WinWebView(HWND hWnd, bool isWindowVisible);
    ~WinWebView(void);

    void UpdateScaleFactor(void);

    bool ProcessWindowMessageImpl(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT *result);
    void OnDPIChanged(HWND hwnd, UINT newDPI, const RECT *rc);
    static BOOL OnNCCreate(HWND hwnd, LPCREATESTRUCT cs);
    void OnPaint(HWND hwnd);
    void OnShowWindow(HWND hwnd, BOOL fShow, UINT status);
    void OnSize(HWND hwnd, UINT state, int cx, int cy);

    // LocalFrameClient
    void DispatchDidReceiveTitle(const String &title) override;
    // WebViewImpl
    void InvalidateNativeView(const blink::IntRect &rect) override;
    SkBitmap PrepareBitmapForCanvas(const blink::WebSize &size) override;

    static std::unordered_map<HWND, WinWebView *> s_viewMap;
    HWND m_hWnd;
    HDC m_memoryDC = nullptr;
    HBITMAP m_oldBitmap = nullptr;
    UINT m_dpi = 96;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_WIN_WEB_VIEW_H
