// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_element_factory.cpp
// Description: HTMLDocument Class
//      Author: Ziming Li
//     Created: 2020-11-01
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "html_document.h"

#include <unordered_map>
#include "third_party/blink/renderer/core/html/html_html_element.h"

namespace blink {

using HTMLElementCreators = std::unordered_map<AtomicString, HTMLElement::Creator>;

static void FillElementCreators(HTMLElementCreators &dst)
{
    ASSERT(dst.empty());
    struct {
        const QualifiedName &tag;
        HTMLElement::Creator creator;
    } data[] = {
        { html_names::kHTMLTag, HTMLHtmlElement::Create }
    };
    for (const auto &e : data)
    {
        dst.insert({ e.tag.LocalName(), e.creator });
    }
}

Element* HTMLDocument::CreateElement(const AtomicString &localName, CreateElementFlags)
{
    static std::unordered_map<AtomicString, HTMLElement::Creator> s_creators;
    if (s_creators.empty())
        FillElementCreators(s_creators);

    auto it = s_creators.find(localName);
    if (std::end(s_creators) == it)
    {
        ASSERT(std::end(s_creators) != it);
        return nullptr;
    }

    return it->second(*this);
}

} // namespace blink
