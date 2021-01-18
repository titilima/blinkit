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

inline bool IsHTMLAnchorElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLAnchorElement(ToElement(node));
}

inline bool IsHTMLAnchorElement(const Node *node)
{
    return nullptr != node && IsHTMLAnchorElement(*node);
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

inline bool IsHTMLButtonElement(const Node *node)
{
    return nullptr != node && IsHTMLButtonElement(*node);
}

class HTMLDataListElement;

inline bool IsHTMLDataListElement(const Element &element)
{
    return element.HasTagName(html_names::kDatalistTag);
}

inline bool IsHTMLDataListElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLDataListElement(ToElement(node));
}

template<>
inline bool IsElementOfType<const HTMLDataListElement>(const Node &node)
{
    return IsHTMLDataListElement(node);
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

class HTMLFieldSetElement;

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

template<>
inline bool IsElementOfType<const HTMLFieldSetElement>(const Node &node)
{
    return IsHTMLFieldSetElement(node);
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

inline bool IsHTMLHRElement(const Element &element)
{
    return element.HasTagName(html_names::kHrTag);
}

inline bool IsHTMLHRElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLHRElement(ToElement(node));
}

inline bool IsHTMLHRElement(const Node *node)
{
    return nullptr != node && IsHTMLHRElement(*node);
}

inline bool IsHTMLHtmlElement(const Element &element)
{
    return element.HasTagName(html_names::kHTMLTag);
}

inline bool IsHTMLHtmlElement(const Element *element)
{
    return nullptr != element && IsHTMLHtmlElement(*element);
}

class HTMLImageElement;

inline bool IsHTMLImageElement(const Element &element)
{
    return element.HasTagName(html_names::kImgTag);
}

inline bool IsHTMLImageElement(const Element *element)
{
    return nullptr != element && IsHTMLImageElement(*element);
}

inline bool IsHTMLImageElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLImageElement(ToElement(node));
}

inline bool IsHTMLImageElement(const Node *node)
{
    return nullptr != node && IsHTMLImageElement(*node);
}

template<>
inline bool IsElementOfType<const HTMLImageElement>(const Node &node)
{
    return IsHTMLImageElement(node);
}

class HTMLInputElement;

inline bool IsHTMLInputElement(const Element &element)
{
    return element.HasTagName(html_names::kInputTag);
}

inline bool IsHTMLInputElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLInputElement(ToElement(node));
}

inline bool IsHTMLInputElement(const Node *node)
{
    return nullptr != node && IsHTMLInputElement(*node);
}

template<>
inline bool IsElementOfType<const HTMLInputElement>(const Node &node)
{
    return IsHTMLInputElement(node);
}

class HTMLLegendElement;

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

class HTMLLinkElement;

inline bool IsHTMLLinkElement(const Element &element)
{
    return element.HasTagName(html_names::kLinkTag);
}

inline bool IsHTMLLinkElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLLinkElement(ToElement(node));
}

inline bool IsHTMLLinkElement(const Node *node)
{
    return nullptr != node && IsHTMLLinkElement(*node);
}

template<>
inline bool IsElementOfType<const HTMLLinkElement>(const Node &node)
{
    return IsHTMLLinkElement(node);
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

class HTMLOListElement;

inline bool IsHTMLOListElement(const Element &element)
{
    return element.HasTagName(html_names::kOlTag);
}

inline bool IsHTMLOListElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLOListElement(ToElement(node));
}

template<>
inline bool IsElementOfType<const HTMLOListElement>(const Node &node)
{
    return IsHTMLOListElement(node);
}

inline bool IsHTMLOptGroupElement(const Element &element)
{
    return element.HasTagName(html_names::kOptgroupTag);
}

inline bool IsHTMLOptGroupElement(const Element *element)
{
    return nullptr != element && IsHTMLOptGroupElement(*element);
}

inline bool IsHTMLOptGroupElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLOptGroupElement(ToElement(node));
}

inline bool IsHTMLOptionElement(const Element &element)
{
    return element.HasTagName(html_names::kOptionTag);
}

inline bool IsHTMLOptionElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLOptionElement(ToElement(node));
}

inline bool IsHTMLPictureElement(const Element &element)
{
    return element.HasTagName(html_names::kPictureTag);
}

inline bool IsHTMLPictureElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLPictureElement(ToElement(node));
}

inline bool IsHTMLPictureElement(const Node *node)
{
    return nullptr != node && IsHTMLPictureElement(*node);
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

inline bool IsHTMLSelectElement(const Node *node)
{
    return nullptr != node && IsHTMLSelectElement(*node);
}

class HTMLSlotElement;

inline bool IsHTMLSlotElement(const Element &element)
{
    return element.HasTagName(html_names::kSlotTag);
}

inline bool IsHTMLSlotElement(const Element *element)
{
    return nullptr != element && IsHTMLSlotElement(*element);
}

inline bool IsHTMLSlotElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLSlotElement(ToElement(node));
}

template<>
inline bool IsElementOfType<const HTMLSlotElement>(const Node &node)
{
    return IsHTMLSlotElement(node);
}

class HTMLSpanElement;

inline bool IsHTMLSpanElement(const Element &element)
{
    return element.HasTagName(html_names::kSpanTag);
}

inline bool IsHTMLSpanElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLSpanElement(ToElement(node));
}

inline bool IsHTMLSpanElement(const Node *node)
{
    return nullptr != node && IsHTMLSpanElement(*node);
}

template<>
inline bool IsElementOfType<const HTMLSpanElement>(const Node &node)
{
    return IsHTMLSpanElement(node);
}

class HTMLStyleElement;

inline bool IsHTMLStyleElement(const Element &element)
{
    return element.HasTagName(html_names::kStyleTag);
}

inline bool IsHTMLStyleElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLStyleElement(ToElement(node));
}

inline bool IsHTMLStyleElement(const Node *node)
{
    return nullptr != node && IsHTMLStyleElement(node);
}

template<>
inline bool IsElementOfType<const HTMLStyleElement>(const Node &node)
{
    return IsHTMLStyleElement(node);
}

class HTMLTableElement;

inline bool IsHTMLTableElement(const Element &element)
{
    return element.HasTagName(html_names::kTableTag);
}

inline bool IsHTMLTableElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLTableElement(ToElement(node));
}

inline bool IsHTMLTableElement(const Node *node)
{
    return nullptr != node && IsHTMLTableElement(*node);
}

template<>
inline bool IsElementOfType<const HTMLTableElement>(const Node &node)
{
    return IsHTMLTableElement(node);
}

class HTMLTableCaptionElement;

inline bool IsHTMLTableCaptionElement(const Element &element)
{
    return element.HasTagName(html_names::kCaptionTag);
}

inline bool IsHTMLTableCaptionElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLTableCaptionElement(ToElement(node));
}

template<>
inline bool IsElementOfType<const HTMLTableCaptionElement>(const Node &node)
{
    return IsHTMLTableCaptionElement(node);
}

class HTMLTableRowElement;

inline bool IsHTMLTableRowElement(const Element &element)
{
    return element.HasTagName(html_names::kTrTag);
}

inline bool IsHTMLTableRowElement(const Element *element)
{
    return nullptr != element && IsHTMLTableRowElement(*element);
}

inline bool IsHTMLTableRowElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLTableRowElement(ToElement(node));
}

template<>
inline bool IsElementOfType<const HTMLTableRowElement>(const Node &node)
{
    return IsHTMLTableRowElement(node);
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

inline bool IsHTMLTextAreaElement(const Node *node)
{
    return nullptr != node && IsHTMLTextAreaElement(*node);
}

inline bool IsHTMLTitleElement(const Element &element)
{
    return element.HasTagName(html_names::kTitleTag);
}

inline bool IsHTMLUListElement(const Element &element)
{
    return element.HasTagName(html_names::kUlTag);
}

inline bool IsHTMLUListElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLUListElement(ToElement(node));
}

} // namespace blink

#define ToHTMLFieldSetElement(x)        blink::ToElement<blink::HTMLFieldSetElement>(x)
#define ToHTMLImageElement(x)           blink::ToElement<blink::HTMLImageElement>(x)
#define ToHTMLImageElementOrNull(x)     blink::ToElementOrNull<blink::HTMLImageElement>(x)
#define ToHTMLInputElement(x)           blink::ToElement<blink::HTMLInputElement>(x)
#define ToHTMLInputElementOrNull(x)     blink::ToElementOrNull<blink::HTMLInputElement>(x)
#define ToHTMLLinkElement(x)            blink::ToElement<blink::HTMLLinkElement>(x)
#define ToHTMLOListElementOrNull(x)     blink::ToElementOrNull<blink::HTMLOListElement>(x)
#define ToHTMLSlotElementOrNull(x)      blink::ToElementOrNull<blink::HTMLSlotElement>(x)
#define ToHTMLSpanElement(x)            blink::ToElement<blink::HTMLSpanElement>(x)
#define ToHTMLStyleElement(x)           blink::ToElement<blink::HTMLStyleElement>(x)
#define ToHTMLTableElement(x)           blink::ToElement<blink::HTMLTableElement>(x)
#define ToHTMLTableElementOrNull(x)     blink::ToElementOrNull<blink::HTMLTableElement>(x)
#define ToHTMLTableRowElement(x)        blink::ToElement<blink::HTMLTableRowElement>(x)
#define ToHTMLTableRowElementOrNull(x)  blink::ToElementOrNull<blink::HTMLTableRowElement>(x)

#endif // BLINKIT_BLINK_HTML_ELEMENT_TYPE_HELPERS_H
