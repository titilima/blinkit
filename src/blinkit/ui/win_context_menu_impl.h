#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_context_menu_impl.h
// Description: WinWebView::ContextMenuImpl Class
//      Author: Ziming Li
//     Created: 2021-10-14
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_WIN_CONTEXT_MENU_IMPL_H
#define BLINKIT_WIN_CONTEXT_MENU_IMPL_H

#include "blinkit/ui/rendering_scheduler.h"
#include "blinkit/ui/win_web_view.h"
#include "third_party/zed/include/zed/win/hmenu.hpp"

namespace BlinKit {

class WinWebView::ContextMenuImpl final : public WebViewImpl::ContextMenu
{
public:
    ContextMenuImpl(WinWebView &webView, int editFlags);
    ~ContextMenuImpl(void) override;
private:
    zed::unique_menu BuildPopupMenu(void) const;
    bool BindEditorCommand(std::function<void()> &dst, UINT cmd) const;

    void Show(void) override;

    WinWebView &m_webView;
    const int m_editFlags;
    std::optional<UINT> m_returnCmd;
};

} // namespace BlinKit

#endif // BLINKIT_WIN_CONTEXT_MENU_IMPL_H
