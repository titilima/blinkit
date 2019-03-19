// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_context_menu.h
// Description: WinContextMenu Class
//      Author: Ziming Li
//     Created: 2019-03-19
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_WIN_CONTEXT_MENU_H
#define BLINKIT_BLINKIT_WIN_CONTEXT_MENU_H

#pragma once

#include "context_menu.h"

namespace BlinKit {

class WinContextMenu final : public ContextMenu
{
public:
    WinContextMenu(blink::WebView *webView);
    ~WinContextMenu(void);
private:
    static HMENU BuildFrom(HMENU sourceMenu);

    // ContextMenu
    void Show(BkView::NativeView nativeView, const blink::WebContextMenuData &data) override;

    HMENU m_menu = nullptr;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_WIN_CONTEXT_MENU_H
