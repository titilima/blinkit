// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: crawler_element.cpp
// Description: CrawlerElement Class
//      Author: Ziming Li
//     Created: 2019-04-24
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "crawler_element.h"

#if 0 // BKTODO:
#include "third_party/blink/renderer/core/dom/attr.h"
#include "third_party/blink/renderer/core/html_names.h"
#endif

using namespace blink;

namespace BlinKit {

CrawlerElement::CrawlerElement(const AtomicString &localName, Document *document)
#if 0 // BKTODO:
    : Element(QualifiedName(g_null_atom, localName, html_names::xhtmlNamespaceURI), document, kCreateCrawlerElement)
#ifndef NDEBUG
    , m_localNameForDebug(localName.StdUtf8())
#endif
#endif
{
    // Nothing
}

#if 0 // BKTODO:
bool CrawlerElement::isDisabledFormControl(void) const
{
    if (fastHasAttribute(HTMLNames::disabledAttr))
        return isFormControlElement();
    return false;
}

bool CrawlerElement::isFormControlElement(void) const
{
    using namespace HTMLNames;
    static const HTMLQualifiedName *formTags[] = {
        &buttonTag, &fieldsetTag, &inputTag, &outputTag, &selectTag, &textareaTag
    };
    for (const auto *tag : formTags)
    {
        if (hasTagName(*tag))
            return true;
    }
    return false;
}

bool CrawlerElement::IsURLAttribute(const Attribute &attribute) const
{
    const QualifiedName &name = attribute.GetName();
    if (HasTagName(html_names::kATag))
        return name == html_names::kHrefAttr;
    if (HasTagName(html_names::kImgTag))
        return name == html_names::kSrcAttr;
    return false;
}
#endif // 0

} // namespace BlinKit
