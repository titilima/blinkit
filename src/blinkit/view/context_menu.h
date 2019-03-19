// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: context_menu.h
// Description: ContextMenu Class
//      Author: Ziming Li
//     Created: 2019-03-19
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_CONTEXT_MENU_H
#define BLINKIT_BLINKIT_CONTEXT_MENU_H

#pragma once

#include "sdk/include/BlinKit.h"
#include "public/web/WebContextMenuData.h"

namespace blink {
class WebView;
}

namespace BlinKit {

class ContextMenu
{
public:
    static std::unique_ptr<ContextMenu> CreateInstance(blink::WebView *webView);
    virtual ~ContextMenu(void) = default;

    static bool ShouldShow(const blink::WebContextMenuData &data);
    virtual void Show(BkView::NativeView nativeView, const blink::WebContextMenuData &data) = 0;
protected:
    ContextMenu(blink::WebView *webView);

    void ExecuteCommand(const char *command);
private:
    blink::WebView *m_webView;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CONTEXT_MENU_H
