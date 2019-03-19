// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_context_menu.cpp
// Description: WinContextMenu Class
//      Author: Ziming Li
//     Created: 2019-03-19
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "win_context_menu.h"

using namespace blink;

namespace BlinKit {

enum class EditID {
    Separator = 0,

    Undo      = 772,
    Cut       = 768,
    Copy      = 769,
    Paste     = 770,
    Delete    = 771,
    SelectAll = 177
};

struct MenuItemEntry {
    unsigned editFlag;
    const char *command;
};

static const MenuItemEntry MenuItems[] = {
    { WebContextMenuData::CanUndo,        "Undo"      },
    { WebContextMenuData::CanCut,         "Cut"       },
    { WebContextMenuData::CanCopy,        "Copy"      },
    { WebContextMenuData::CanPaste,       "Paste"     },
    { WebContextMenuData::CanDelete,      "Delete"    },
    { WebContextMenuData::CanSelectAll,   "SelectAll" }
};

static const UINT BaseId = 1000;

WinContextMenu::WinContextMenu(WebView *webView) : ContextMenu(webView)
{
    HMODULE user32 = GetModuleHandle(TEXT("user32.dll"));

    HMENU sourceMenu = LoadMenu(user32, MAKEINTRESOURCE(1));
    if (nullptr == sourceMenu)
    {
        assert(nullptr != sourceMenu);
        return;
    }

    m_menu = BuildFrom(sourceMenu);
    DestroyMenu(sourceMenu);
}

WinContextMenu::~WinContextMenu(void)
{
    DestroyMenu(m_menu);
}

HMENU WinContextMenu::BuildFrom(HMENU sourceMenu)
{
    HMENU ret = CreatePopupMenu();
    if (nullptr == ret)
    {
        assert(nullptr != ret);
        return nullptr;
    }

    static const EditID ids[] = {
        EditID::Undo,
        EditID::Separator,
        EditID::Cut, EditID::Copy, EditID::Paste, EditID::Delete,
        EditID::Separator,
        EditID::SelectAll
    };

    UINT currentId = BaseId;
    for (const EditID id : ids)
    {
        if (EditID::Separator == id)
        {
            AppendMenu(ret, MF_SEPARATOR, 0, nullptr);
            continue;
        }

        WCHAR text[MAX_PATH] = { 0 };

        MENUITEMINFOW mi = { 0 };
        mi.cbSize = sizeof(mi);
        mi.fMask = MIIM_STRING;
        mi.dwTypeData = text;
        mi.cch = MAX_PATH;
        GetMenuItemInfoW(sourceMenu, static_cast<UINT>(id), FALSE, &mi);

        AppendMenuW(ret, MF_STRING, currentId++, text);
    }

    return ret;
}

void WinContextMenu::Show(BkView::NativeView nativeView, const WebContextMenuData &data)
{
    if (nullptr == m_menu)
    {
        assert(nullptr != m_menu);
        return;
    }

    for (size_t i = 0; i < ARRAYSIZE(MenuItems); ++i)
    {
        const MenuItemEntry &entry = MenuItems[i];
        if (0 != (entry.editFlag & data.editFlags))
            EnableMenuItem(m_menu, BaseId + i, MF_ENABLED);
        else
            EnableMenuItem(m_menu, BaseId + i, MF_DISABLED | MF_GRAYED);
    }

    POINT pt;
    GetCursorPos(&pt);

    UINT id = TrackPopupMenu(m_menu, TPM_RETURNCMD, pt.x, pt.y, 0, nativeView, nullptr);
    if (0 != id)
        ExecuteCommand(MenuItems[id - BaseId].command);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<ContextMenu> ContextMenu::CreateInstance(WebView *webView)
{
    return std::make_unique<WinContextMenu>(webView);
}

} // namespace BlinKit
