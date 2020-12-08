// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_element_type_helpers.h
// Description: HTML Element Type Helpers
//      Author: Ziming Li
//     Created: 2019-10-30
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_HTML_ELEMENT_TYPE_HELPERS_H
#define BLINKIT_BLINK_HTML_ELEMENT_TYPE_HELPERS_H

#pragma once

#include "third_party/blink/renderer/core/dom/element.h"
#include "third_party/blink/renderer/core/html_names.h"
#include "third_party/blink/renderer/platform/runtime_enabled_features.h"

namespace blink {

inline bool IsHTMLAnchorElement(const Element &element)
{
    return element.HasTagName(html_names::kATag);
}

inline bool IsHTMLBodyElement(const Element &element)
{
    return element.HasTagName(html_names::kBodyTag);
}

inline bool IsHTMLBodyElement(const Element *element)
{
    return nullptr != element && IsHTMLBodyElement(*element);
}

inline bool IsHTMLBodyElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLBodyElement(ToElement(node));
}

inline bool IsHTMLBRElement(const Element &element)
{
    return element.HasTagName(html_names::kBrTag);
}

inline bool IsHTMLBRElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLBRElement(ToElement(node));
}

inline bool IsHTMLButtonElement(const Element &element)
{
    return element.HasTagName(html_names::kButtonTag);
}

inline bool IsHTMLButtonElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLButtonElement(ToElement(node));
}

inline bool IsHTMLDialogElement(const Element &element)
{
    return element.HasTagName(html_names::kDialogTag);
}

inline bool IsHTMLDialogElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLDialogElement(ToElement(node));
}

inline bool IsHTMLDialogElement(const Node *node)
{
    return nullptr != node && IsHTMLDialogElement(*node);
}

inline bool IsHTMLDivElement(const Element &element)
{
    return element.HasTagName(html_names::kDivTag);
}

inline bool IsHTMLFieldSetElement(const Element &element)
{
    return element.HasTagName(html_names::kFieldsetTag);
}

inline bool IsHTMLFieldSetElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLFieldSetElement(ToElement(node));
}

inline bool IsHTMLFieldSetElement(const Node *node)
{
    return nullptr != node && IsHTMLFieldSetElement(*node);
}

inline bool IsHTMLFontElement(const Element &element)
{
    return element.HasTagName(html_names::kFontTag);
}

inline bool IsHTMLFormElement(const Element &element)
{
    return element.HasTagName(html_names::kFormTag);
}

inline bool IsHTMLFormElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLFormElement(ToElement(node));
}

inline bool IsHTMLHeadElement(const Element &element)
{
    return element.HasTagName(html_names::kHeadTag);
}

inline bool IsHTMLHeadElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLHeadElement(ToElement(node));
}

inline bool IsHTMLHtmlElement(const Element &element)
{
    return element.HasTagName(html_names::kHTMLTag);
}

inline bool IsHTMLHtmlElement(const Element *element)
{
    return nullptr != element && IsHTMLHtmlElement(*element);
}

inline bool IsHTMLImageElement(const Element &element)
{
    return element.HasTagName(html_names::kImgTag);
}

inline bool IsHTMLImageElement(const Element *element)
{
    return nullptr != element && IsHTMLImageElement(*element);
}

inline bool IsHTMLInputElement(const Element &element)
{
    return element.HasTagName(html_names::kInputTag);
}

inline bool IsHTMLInputElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLInputElement(ToElement(node));
}

inline bool IsHTMLLegendElement(const Element &element)
{
    return element.HasTagName(html_names::kLegendTag);
}

inline bool IsHTMLLegendElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLLegendElement(ToElement(node));
}

inline bool IsHTMLLegendElement(const Node *node)
{
    return nullptr != node && IsHTMLLegendElement(*node);
}

inline bool IsHTMLMarqueeElement(const Element &element)
{
    return element.HasTagName(html_names::kMarqueeTag);
}

inline bool IsHTMLMarqueeElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLMarqueeElement(ToElement(node));
}

inline bool IsHTMLMarqueeElement(const Node *node)
{
    return nullptr != node && IsHTMLMarqueeElement(*node);
}

inline bool IsHTMLScriptElement(const Element &element)
{
    return element.HasTagName(html_names::kScriptTag);
}

inline bool IsHTMLScriptElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLScriptElement(ToElement(node));
}

inline bool IsHTMLSelectElement(const Element &element)
{
    return element.HasTagName(html_names::kSelectTag);
}

inline bool IsHTMLSelectElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLSelectElement(ToElement(node));
}

inline bool IsHTMLSlotElement(const Element &element)
{
    return element.HasTagName(html_names::kSlotTag);
}

inline bool IsHTMLSlotElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLSlotElement(ToElement(node));
}

inline bool IsHTMLSpanElement(const Element &element)
{
    return element.HasTagName(html_names::kSpanTag);
}

inline bool IsHTMLStyleElement(const Element &element)
{
    return element.HasTagName(html_names::kStyleTag);
}

inline bool IsHTMLStyleElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLStyleElement(ToElement(node));
}

inline bool IsHTMLTableCellElement(const Element &element)
{
    return element.HasTagName(html_names::kTdTag) || element.HasTagName(html_names::kThTag);
}

inline bool IsHTMLTableElement(const Element &element)
{
    return element.HasTagName(html_names::kTableTag);
}

inline bool IsHTMLTemplateElement(const Element &element)
{
    return element.HasTagName(html_names::kTemplateTag);
}

inline bool IsHTMLTemplateElement(const Element *element)
{
    return nullptr != element && IsHTMLTemplateElement(*element);
}

inline bool IsHTMLTemplateElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLTemplateElement(ToElement(node));
}

inline bool IsHTMLTemplateElement(const Node *node)
{
    return nullptr != node && IsHTMLTemplateElement(*node);
}

inline bool IsHTMLTextAreaElement(const Element &element)
{
    return element.HasTagName(html_names::kTextareaTag);
}

inline bool IsHTMLTextAreaElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLTextAreaElement(ToElement(node));
}

inline bool IsHTMLTitleElement(const Element &element)
{
    return element.HasTagName(html_names::kTitleTag);
}

} // namespace blink

#endif // BLINKIT_BLINK_HTML_ELEMENT_TYPE_HELPERS_H
