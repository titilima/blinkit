// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CustomContextMenuProvider.cpp
// Description: CustomContextMenuProvider Class
//      Author: Ziming Li
//     Created: 2021-07-31
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "./CustomContextMenuProvider.h"

#include "blinkit/blink/renderer/core/dom/element_traversal.h"
#include "blinkit/blink/renderer/core/events/EventDispatcher.h"
#include "blinkit/blink/renderer/core/events/MouseEvent.h"
#include "blinkit/blink/renderer/core/html/HTMLMenuElement.h"
#include "blinkit/blink/renderer/core/html/HTMLMenuItemElement.h"
#include "blinkit/blink/renderer/core/page/ContextMenuController.h"
#include "blinkit/blink/renderer/core/page/Page.h"
#include "blinkit/blink/renderer/platform/ContextMenu.h"

namespace blink {

using namespace HTMLNames;

CustomContextMenuProvider::CustomContextMenuProvider(HTMLMenuElement& menu, HTMLElement& subject)
    : m_menu(menu)
    , m_subjectElement(subject)
{
}

CustomContextMenuProvider::~CustomContextMenuProvider()
{
}

DEFINE_TRACE(CustomContextMenuProvider)
{
    visitor->trace(m_menu);
    visitor->trace(m_subjectElement);
    visitor->trace(m_menuItems);
    ContextMenuProvider::trace(visitor);
}

void CustomContextMenuProvider::populateContextMenu(ContextMenu* menu)
{
    populateContextMenuItems(*m_menu, *menu);
}

void CustomContextMenuProvider::contextMenuItemSelected(const ContextMenuItem* item)
{
    if (HTMLElement* element = menuItemAt(item->action())) {
        GCRefPtr<MouseEvent> click = MouseEvent::create(EventTypeNames::click, m_menu->document().domWindow(), Event::create(), SimulatedClickCreationScope::FromUserAgent);
        click->setRelatedTarget(m_subjectElement.get());
        element->dispatchEvent(click);
    }
}

void CustomContextMenuProvider::contextMenuCleared()
{
    m_menuItems.clear();
    m_subjectElement = nullptr;
}

void CustomContextMenuProvider::appendSeparator(ContextMenu& contextMenu)
{
    // Avoid separators at the start of any menu and submenu.
    if (contextMenu.items().empty())
        return;

    // Collapse all sequences of two or more adjacent separators in the menu or
    // any submenus to a single separator.
    const ContextMenuItem &lastItem = contextMenu.items().back();
    if (lastItem.type() == SeparatorType)
        return;

    contextMenu.appendItem(ContextMenuItem(SeparatorType, ContextMenuItemCustomTagNoAction, String(), String()));
}

void CustomContextMenuProvider::appendMenuItem(HTMLMenuItemElement* menuItem, ContextMenu& contextMenu)
{
    // Avoid menuitems with no label.
    String labelString = menuItem->fastGetAttribute(labelAttr);
    if (labelString.isEmpty())
        return;

    m_menuItems.append(menuItem);

    ASSERT(false); // BKTODO:
#if 0
    bool enabled = !menuItem->fastHasAttribute(disabledAttr);
    String icon = menuItem->fastGetAttribute(iconAttr);
    if (!icon.isEmpty()) {
        // To obtain the absolute URL of the icon when the attribute's value is not the empty string,
        // the attribute's value must be resolved relative to the element.
        KURL iconURL = KURL(menuItem->baseURI(), icon);
        icon = iconURL.string();
    }
    ContextMenuAction action = static_cast<ContextMenuAction>(ContextMenuItemBaseCustomTag + m_menuItems.size() - 1);
    if (equalIgnoringCase(menuItem->fastGetAttribute(typeAttr), "checkbox") || equalIgnoringCase(menuItem->fastGetAttribute(typeAttr), "radio"))
        contextMenu.appendItem(ContextMenuItem(CheckableActionType, action, labelString, icon, enabled, menuItem->fastHasAttribute(checkedAttr)));
    else
        contextMenu.appendItem(ContextMenuItem(ActionType, action, labelString, icon, enabled, false));
#endif
}

void CustomContextMenuProvider::populateContextMenuItems(const HTMLMenuElement& menu, ContextMenu& contextMenu)
{
    HTMLElement* nextElement = Traversal<HTMLElement>::firstWithin(menu);
    while (nextElement) {
        if (isHTMLHRElement(*nextElement)) {
            appendSeparator(contextMenu);
            nextElement = Traversal<HTMLElement>::next(*nextElement, &menu);
        } else if (isHTMLMenuElement(*nextElement)) {
            ContextMenu subMenu;
            String labelString = nextElement->fastGetAttribute(labelAttr);
            if (labelString.isNull()) {
                appendSeparator(contextMenu);
                populateContextMenuItems(*toHTMLMenuElement(nextElement), contextMenu);
                appendSeparator(contextMenu);
            } else if (!labelString.isEmpty()) {
                populateContextMenuItems(*toHTMLMenuElement(nextElement), subMenu);
                contextMenu.appendItem(ContextMenuItem(SubmenuType, ContextMenuItemCustomTagNoAction, labelString, String(), &subMenu));
            }
            nextElement = Traversal<HTMLElement>::nextSibling(*nextElement);
        } else if (isHTMLMenuItemElement(*nextElement)) {
            appendMenuItem(toHTMLMenuItemElement(nextElement), contextMenu);
            if (ContextMenuItemBaseCustomTag + m_menuItems.size() >= ContextMenuItemLastCustomTag)
                break;
            nextElement = Traversal<HTMLElement>::next(*nextElement, &menu);
        } else {
            nextElement = Traversal<HTMLElement>::next(*nextElement, &menu);
        }
    }

    // Remove separators at the end of the menu and any submenus.
    while (!contextMenu.items().empty() && contextMenu.items().back().type() == SeparatorType)
        contextMenu.removeLastItem();
}

HTMLElement* CustomContextMenuProvider::menuItemAt(unsigned menuId)
{
    int itemIndex = menuId - ContextMenuItemBaseCustomTag;
    if (itemIndex < 0 || static_cast<unsigned long>(itemIndex) >= m_menuItems.size())
        return nullptr;
    return m_menuItems[itemIndex].get();
}

} // namespace blink
