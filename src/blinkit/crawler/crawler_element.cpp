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

#include "core/HTMLNames.h"

using namespace blink;

namespace BlinKit {

CrawlerElement::CrawlerElement(const AtomicString &localName, Document *document)
    : Element(QualifiedName(nullAtom, localName, HTMLNames::xhtmlNamespaceURI), document, CreateCrawlerElement)
{
    // Nothing
}

bool CrawlerElement::isDisabledFormControl(void) const
{
    if (fastHasAttribute(HTMLNames::disabledAttr))
        return isFormControlElement();
    return false;
}

bool CrawlerElement::isFormControlElement(void) const
{
    using namespace HTMLNames;
    static const HTMLQualifiedName formTags[] = {
        buttonTag, fieldsetTag, inputTag, outputTag, selectTag, textareaTag
    };
    for (const auto &tag : formTags)
    {
        if (hasTagName(tag))
            return true;
    }
    return false;
}

bool CrawlerElement::isURLAttribute(const Attribute &attribute) const
{
    const QualifiedName &name = attribute.name();
    if (hasTagName(HTMLNames::aTag))
        return name == HTMLNames::hrefAttr;
    if (hasTagName(HTMLNames::imgTag))
        return name == HTMLNames::srcAttr;
    return false;
}

} // namespace BlinKit
