// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_context_menu_impl.cpp
// Description: WinWebView::ContextMenuImpl Class
//      Author: Ziming Li
//     Created: 2021-10-14
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./win_context_menu_impl.h"

#include "blinkit/app/caller.h"
#include "blinkit/blink/public/web/WebContextMenuData.h"
#include "blinkit/blink/renderer/core/editing/Editor.h"

using namespace blink;

namespace BlinKit {

class EditID {
public:
    static constexpr UINT Separator = 0;

    static constexpr UINT Undo      = 772;
    static constexpr UINT Cut       = 768;
    static constexpr UINT Copy      = 769;
    static constexpr UINT Paste     = 770;
    static constexpr UINT Delete    = 771;
    static constexpr UINT SelectAll = 177;
};

WinWebView::ContextMenuImpl::ContextMenuImpl(WinWebView &webView, int editFlags)
    : m_webView(webView), m_editFlags(editFlags)
{
}

WinWebView::ContextMenuImpl::~ContextMenuImpl(void)
{
    const UINT returnCmd = m_returnCmd.value_or(0);
    if (0 == returnCmd)
        return;

    std::function<void()> callback;
    if (EditID::SelectAll == returnCmd)
    {
        FrameSelection &selection = m_webView.GetFrame().selection();
        callback = std::bind(&FrameSelection::selectAll, &selection);
    }
    else
    {
        if (!BindEditorCommand(callback, returnCmd))
            return;
    }

    WebViewImpl *webView = &m_webView;
    m_webView.GetCaller().Call(BLINK_FROM_HERE, [webView, callback = std::move(callback)]{
        ScopedRenderingScheduler _(webView);
        callback();
    });
}

bool WinWebView::ContextMenuImpl::BindEditorCommand(std::function<void()> &dst, UINT cmd) const
{
    void (Editor::*pfn)(void);
    switch (cmd)
    {
        case EditID::Undo:
            pfn = &Editor::undo;
            break;
        case EditID::Cut:
            pfn = &Editor::cut;
            break;
        case EditID::Copy:
            pfn = &Editor::copy;
            break;
        case EditID::Paste:
            pfn = &Editor::pasteAsPlainText;
            break;
        case EditID::Delete:
            pfn = &Editor::performDelete;
            break;
        default:
            ASSERT_NOT_REACHED();
            return false;
    }

    Editor &editor = m_webView.focusedCoreFrame()->editor();
    dst = std::bind(pfn, &editor);
    return true;
}

zed::unique_menu WinWebView::ContextMenuImpl::BuildPopupMenu(void) const
{
    zed::unique_menu src(1, GetModuleHandle(TEXT("user32.dll")));
    if (!src)
    {
        ASSERT(src);
        return zed::unique_menu::null();
    }

    zed::unique_menu ret = zed::unique_menu::create_popup();
    do {
        if (!ret)
        {
            ASSERT(ret);
            break;
        }

        static const struct {
            UINT id;
            int editFlag;
        } items[] = {
            { EditID::Undo,      WebContextMenuData::CanUndo      },
            { EditID::Separator, 0                                },
            { EditID::Cut,       WebContextMenuData::CanCut       },
            { EditID::Copy,      WebContextMenuData::CanCopy      },
            { EditID::Paste,     WebContextMenuData::CanPaste     },
            { EditID::Delete,    WebContextMenuData::CanDelete    },
            { EditID::Separator, 0                                },
            { EditID::SelectAll, WebContextMenuData::CanSelectAll }
        };
        for (const auto &item : items)
        {
            if (EditID::Separator == item.id)
            {
                ret.append_separator();
                continue;
            }

            WCHAR text[MAX_PATH] = { 0 };

            MENUITEMINFOW mi = { 0 };
            mi.cbSize        = sizeof(mi);
            mi.fMask         = MIIM_STRING;
            mi.dwTypeData    = text;
            mi.cch           = MAX_PATH;
            if (!src.get_item_info_by_id(item.id, mi))
            {
                ASSERT_NOT_REACHED();
                return zed::unique_menu::null();
            }

            UINT flags = MF_ENABLED | MF_STRING;
            if (0 == (item.editFlag & m_editFlags))
                flags |= MF_DISABLED | MF_GRAYED;
            ret.append(item.id, text, flags);
        }
    } while (false);
    return ret;
}

void WinWebView::ContextMenuImpl::Show(void)
{
    zed::unique_menu popupMenu = BuildPopupMenu();
    if (!popupMenu)
        return;

    POINT pt;
    GetCursorPos(&pt);

    HWND hWnd = m_webView.GetHWND();
    SetForegroundWindow(hWnd);
    m_returnCmd = popupMenu.track_popup(TPM_RETURNCMD, pt.x, pt.y, hWnd);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<WebViewImpl::ContextMenu> WinWebView::CreateContextMenu(const WebContextMenuData &data)
{
    return std::make_shared<WinWebView::ContextMenuImpl>(*this, data.editFlags);
}

} // namespace BlinKit
