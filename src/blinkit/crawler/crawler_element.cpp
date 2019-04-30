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

} // namespace BlinKit
