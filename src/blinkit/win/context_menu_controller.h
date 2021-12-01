#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: context_menu_controller.h
// Description: ContextMenuController Class
//      Author: Ziming Li
//     Created: 2021-11-27
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_CONTEXT_MENU_CONTROLLER_H
#define BLINKIT_CONTEXT_MENU_CONTROLLER_H

namespace blink {
class Editor;
}

namespace BlinKit {

class CommandID {
public:
    static constexpr UINT Separator = 0;

    static constexpr UINT Undo      = 772;
    static constexpr UINT Cut       = 768;
    static constexpr UINT Copy      = 769;
    static constexpr UINT Paste     = 770;
    static constexpr UINT Delete    = 771;
    static constexpr UINT SelectAll = 177;
};

class ContextMenuController
{
public:
    ~ContextMenuController(void);

    HMENU RequireMenu(int editFlags);

    using EditorFunction = void (blink::Editor::*)(void);
    void RunEditorFunction(blink::Editor &editor, EditorFunction pfn);
private:
    static HMENU BuildMenu(void);

    HMENU m_hPopupMenu = nullptr;
};

} // namespace BlinKit

#endif // BLINKIT_CONTEXT_MENU_CONTROLLER_H
