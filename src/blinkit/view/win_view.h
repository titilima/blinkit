// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_view.h
// Description: WinView Class
//      Author: Ziming Li
//     Created: 2019-03-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_WIN_VIEW_H
#define BLINKIT_BLINKIT_WIN_VIEW_H

#pragma once

#include "public/platform/WebCursorInfo.h"
#include "view_impl.h"

namespace BlinKit {

class WinView final : public ViewImpl
{
public:
    WinView(BkViewClient &client);
    ~WinView(void);
private:
    void AdjustGlobalCoordinates(blink::WebMouseEvent &we);
    static bool BuildMouseEvent(MouseEvent &dst, UINT message, UINT keyFlags, int x, int y);
    void ProcessDoubleClick(UINT message, UINT keyFlags, int x, int y);
    void UpdateScaleFactor(void);

    void OnDpiChanged(HWND hwnd, UINT dpi, const RECT &rc);
    void OnMouse(UINT message, UINT keyFlags, int x, int y);
    bool OnNCCreate(HWND hwnd, LPCREATESTRUCT cs);
    void OnNCDestroy(HWND hwnd);
    void OnPaint(HWND hwnd);
    void OnSize(HWND hwnd, UINT state, int cx, int cy);

    // BkView
    HWND BKAPI GetNativeView(void) const override { return m_hWnd; }
    bool BKAPI ProcessMessage(HWND h, UINT m, WPARAM w, LPARAM l, LRESULT &r) override;
    void BKAPI Attach(NativeView nativeView) override;
    void BKAPI Paint(NativeCanvas nativeCanvas, const BkRect *rc) override;
    // blink::WebWidgetClient
    void didChangeCursor(const blink::WebCursorInfo &cursorInfo) override;
    // ViewImpl
    std::unique_ptr<SkCanvas> CreateMemoryCanvas(int width, int height) override;
    void DoUpdate(void) override;
    blink::WebMouseEvent Translate(const MouseEvent &e) override;
    void PreHandleInput(const blink::WebInputEvent &e) override;
    void PostHandleInput(const blink::WebInputEvent &e) override;

    HWND m_hWnd = nullptr;
    UINT m_dpi = 96;
    HDC m_memoryDC = nullptr;
    HGDIOBJ m_oldBitmap = nullptr;
    bool m_changingSizeOrPosition = false;
    blink::WebCursorInfo m_cursorInfo;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_WIN_VIEW_H
