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

namespace blink {

inline bool IsHTMLHeadElement(const Element &element)
{
    return element.HasTagName(html_names::kHeadTag);
}

inline bool IsHTMLHeadElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLHeadElement(ToElement(node));
}

inline bool IsHTMLScriptElement(const Element &element)
{
    return element.HasTagName(html_names::kScriptTag);
}

inline bool IsHTMLScriptElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLScriptElement(ToElement(node));
}

inline bool IsHTMLStyleElement(const Element &element)
{
    return element.HasTagName(html_names::kStyleTag);
}

inline bool IsHTMLStyleElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLStyleElement(ToElement(node));
}

inline bool IsHTMLTemplateElement(const Element &element)
{
    return element.HasTagName(html_names::kTemplateTag);
}

inline bool IsHTMLTemplateElement(const Node &node)
{
    return node.IsHTMLElement() && IsHTMLTemplateElement(ToElement(node));
}

} // namespace blink

#endif // BLINKIT_BLINK_HTML_ELEMENT_TYPE_HELPERS_H
