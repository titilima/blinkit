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

#include "./crawler_element.h"

#include "blinkit/blink/renderer/core/dom/Attr.h"
#include "blinkit/blink/renderer/core/HTMLNames.h"

using namespace blink::HTMLNames;

namespace BlinKit {

CrawlerElement::CrawlerElement(const QualifiedName &tagName, Document &document)
    : Element(tagName, &document, CreateCrawlerElement)
#ifndef NDEBUG
    , m_localNameForDebug(tagName.localName().stdUTF8())
#endif
{
    // Nothing
}

CrawlerElement::CrawlerElement(const AtomicString &localName, Document *document)
    : CrawlerElement(QualifiedName(nullAtom, localName, xhtmlNamespaceURI), *document)
{
}

bool CrawlerElement::isDisabledFormControl(void) const
{
    if (fastHasAttribute(disabledAttr))
        return isFormControlElement();
    return false;
}

bool CrawlerElement::isFormControlElement(void) const
{
    static const HTMLQualifiedName *formTags[] = {
        &buttonTag, &fieldsetTag, &inputTag, &selectTag, &textareaTag
    };
    for (const auto *tag : formTags)
    {
        if (hasTagName(*tag))
            return true;
    }
    return false;
}

bool CrawlerElement::isURLAttribute(const Attribute &attribute) const
{
    const QualifiedName &name = attribute.name();
    if (hasTagName(aTag))
        return name == hrefAttr;
    if (hasTagName(imgTag))
        return name == srcAttr;
    return false;
}

} // namespace BlinKit
