// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: context_menu.cpp
// Description: ContextMenu Class
//      Author: Ziming Li
//     Created: 2019-03-19
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "context_menu.h"

#include "public/web/WebElement.h"
#include "public/web/WebFrame.h"
#include "public/web/WebView.h"

using namespace blink;

namespace BlinKit {

ContextMenu::ContextMenu(WebView *webView) : m_webView(webView)
{
    // Nothing
}

void ContextMenu::ExecuteCommand(const char *command)
{
    m_webView->focusedFrame()->executeCommand(WebString::fromUTF8(command));
}

bool ContextMenu::ShouldShow(const WebContextMenuData &data)
{
    if (data.isEditable)
        return true;

    switch (data.inputFieldType)
    {
        case WebContextMenuData::InputFieldTypePlainText:
            return true;
        case WebContextMenuData::InputFieldTypePassword:
            return false;
    }

    if (!data.node.isElementNode())
        return false;

    const WebElement e = data.node.toConst<WebElement>();
    WebString name = e.tagName();
    if (name == "TEXTAREA")
        return true;

    return false;
}

} // namespace BlinKit
