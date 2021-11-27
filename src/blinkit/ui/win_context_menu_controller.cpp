// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_context_menu_controller.cpp
// Description: WinContextMenuController Class
//      Author: Ziming Li
//     Created: 2021-11-27
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./win_context_menu_controller.h"

#include "blinkit/blink/public/web/WebContextMenuData.h"
#include "blinkit/blink/renderer/core/editing/Editor.h"
#include "third_party/zed/include/zed/win/hmenu.hpp"

using namespace blink;

namespace BlinKit {

WinContextMenuController::~WinContextMenuController(void)
{
    if (nullptr != m_hPopupMenu)
        DestroyMenu(m_hPopupMenu);
}

HMENU WinContextMenuController::BuildMenu(void)
{
    HMODULE hUser32 = GetModuleHandle(TEXT("user32.dll"));

    zed::unique_menu src(1, hUser32);
    if (!src)
    {
        ASSERT(src);
        return nullptr;
    }

    zed::unique_menu ret = zed::unique_menu::create_popup();
    do {
        if (!ret)
        {
            ASSERT(ret);
            break;
        }

        static const UINT items[] = {
            CommandID::Undo,
            CommandID::Separator,
            CommandID::Cut, CommandID::Copy, CommandID::Paste, CommandID::Delete,
            CommandID::Separator,
            CommandID::SelectAll
        };
        for (const UINT id : items)
        {
            if (CommandID::Separator == id)
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
            if (!src.get_item_info_by_id(id, mi))
            {
                ASSERT_NOT_REACHED();
                return nullptr;
            }

            ret.append(id, text);
        }
    } while (false);
    return ret.release();
}

HMENU WinContextMenuController::RequireMenu(int editFlags)
{
    if (nullptr == m_hPopupMenu)
        m_hPopupMenu = BuildMenu();

    static const struct {
        UINT id;
        int enableFlag;
    } policies[] = {
        { CommandID::Undo,      WebContextMenuData::CanUndo      },
        { CommandID::Cut,       WebContextMenuData::CanCut       },
        { CommandID::Copy,      WebContextMenuData::CanCopy      },
        { CommandID::Paste,     WebContextMenuData::CanPaste     },
        { CommandID::Delete,    WebContextMenuData::CanDelete    },
        { CommandID::SelectAll, WebContextMenuData::CanSelectAll }
    };
    for (const auto &policy : policies)
    {
        if (0 != (editFlags & policy.enableFlag))
            EnableMenuItem(m_hPopupMenu, policy.id, MF_BYCOMMAND | MF_ENABLED);
        else
            EnableMenuItem(m_hPopupMenu, policy.id, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
    }
    return m_hPopupMenu;
}

void WinContextMenuController::RunEditorFunction(Editor &editor, EditorFunction pfn)
{
    (editor.*pfn)();
}

} // namespace BlinKit
