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
#include "third_party/blink/renderer/core/html/forms/html_button_element.h"
#include "third_party/blink/renderer/core/html/forms/html_data_list_element.h"
#include "third_party/blink/renderer/core/html/forms/html_field_set_element.h"
#include "third_party/blink/renderer/core/html/forms/html_input_element.h"
#include "third_party/blink/renderer/core/html/html_body_element.h"
#include "third_party/blink/renderer/core/html/html_div_element.h"
#include "third_party/blink/renderer/core/html/html_head_element.h"
#include "third_party/blink/renderer/core/html/html_html_element.h"
#include "third_party/blink/renderer/core/html/html_image_element.h"
#include "third_party/blink/renderer/core/html/html_link_element.h"
#include "third_party/blink/renderer/core/html/html_meta_element.h"
#include "third_party/blink/renderer/core/html/html_slot_element.h"
#include "third_party/blink/renderer/core/html/html_span_element.h"
#include "third_party/blink/renderer/core/html/html_style_element.h"
#include "third_party/blink/renderer/core/html/html_title_element.h"

#define DEFINE_CONSTRUCTOR(ClassName)  \
    static HTMLElement* ClassName ## Constructor(Document &document, const CreateElementFlags) {    \
        return ClassName::Create(document);                                                         \
    }
#define DEFINE_CONSTRUCTOR_WITH_FLAGS(ClassName)  \
    static HTMLElement* ClassName ## Constructor(Document &document, const CreateElementFlags flags) {  \
        return ClassName::Create(document, flags);                                                      \
    }

#define CONSTRUCTOR_ENTRY(Tag, ClassName)   \
    { Tag, ClassName ## Constructor }

namespace blink {

using HTMLElementCreators = std::unordered_map<AtomicString, HTMLElement::Creator>;

DEFINE_CONSTRUCTOR_WITH_FLAGS(HTMLBodyElement)
DEFINE_CONSTRUCTOR(HTMLButtonElement)
DEFINE_CONSTRUCTOR(HTMLDataListElement)
DEFINE_CONSTRUCTOR_WITH_FLAGS(HTMLDivElement)
DEFINE_CONSTRUCTOR(HTMLFieldSetElement)
DEFINE_CONSTRUCTOR_WITH_FLAGS(HTMLHeadElement)
DEFINE_CONSTRUCTOR_WITH_FLAGS(HTMLHtmlElement)
DEFINE_CONSTRUCTOR_WITH_FLAGS(HTMLImageElement)
DEFINE_CONSTRUCTOR_WITH_FLAGS(HTMLInputElement)
DEFINE_CONSTRUCTOR_WITH_FLAGS(HTMLLinkElement)
DEFINE_CONSTRUCTOR_WITH_FLAGS(HTMLMetaElement)
DEFINE_CONSTRUCTOR(HTMLSlotElement)
DEFINE_CONSTRUCTOR_WITH_FLAGS(HTMLSpanElement)
DEFINE_CONSTRUCTOR_WITH_FLAGS(HTMLStyleElement)
DEFINE_CONSTRUCTOR_WITH_FLAGS(HTMLTitleElement)

static void FillElementCreators(HTMLElementCreators &dst)
{
    using namespace html_names;

    ASSERT(dst.empty());
    struct {
        const QualifiedName &tag;
        HTMLElement::Creator creator;
    } data[] = {
        CONSTRUCTOR_ENTRY(kBodyTag,     HTMLBodyElement),
        CONSTRUCTOR_ENTRY(kButtonTag,   HTMLButtonElement),
        CONSTRUCTOR_ENTRY(kDatalistTag, HTMLDataListElement),
        CONSTRUCTOR_ENTRY(kDivTag,      HTMLDivElement),
        CONSTRUCTOR_ENTRY(kFieldsetTag, HTMLFieldSetElement),
        CONSTRUCTOR_ENTRY(kHeadTag,     HTMLHeadElement),
        CONSTRUCTOR_ENTRY(kHTMLTag,     HTMLHtmlElement),
        CONSTRUCTOR_ENTRY(kImgTag,      HTMLImageElement),
        CONSTRUCTOR_ENTRY(kInputTag,    HTMLInputElement),
        CONSTRUCTOR_ENTRY(kLinkTag,     HTMLLinkElement),
        CONSTRUCTOR_ENTRY(kMetaTag,     HTMLMetaElement),
        CONSTRUCTOR_ENTRY(kSlotTag,     HTMLSlotElement),
        CONSTRUCTOR_ENTRY(kSpanTag,     HTMLSpanElement),
        CONSTRUCTOR_ENTRY(kStyleTag,    HTMLStyleElement),
        CONSTRUCTOR_ENTRY(kTitleTag,    HTMLTitleElement)
    };
    for (const auto &e : data)
    {
        dst.insert({ e.tag.LocalName(), e.creator });
    }
}

Element* HTMLDocument::CreateElement(const AtomicString &localName, CreateElementFlags flags)
{
    static std::unordered_map<AtomicString, HTMLElement::Creator> s_creators;
    if (s_creators.empty())
        FillElementCreators(s_creators);

    auto it = s_creators.find(localName);
    if (std::end(s_creators) == it)
    {
        BKLOG("Unknown tag name: %s", localName.StdUtf8().c_str());
        ASSERT(std::end(s_creators) != it);
        return nullptr;
    }

    return it->second(*this, flags);
}

} // namespace blink
