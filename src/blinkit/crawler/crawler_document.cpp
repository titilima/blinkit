// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: crawler_document.cpp
// Description: CrawlerDocument Class
//      Author: Ziming Li
//     Created: 2019-04-14
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "crawler_document.h"

#include "core/dom/DocumentInit.h"

#include "crawler/crawler_form_element.h"

using namespace blink;

namespace BlinKit {

CrawlerDocument::CrawlerDocument(const DocumentInit &init)
    : Document(init, HTMLDocumentClass, true)
{
    // Nothing
}

PassRefPtrWillBeRawPtr<Element> CrawlerDocument::createElement(const AtomicString &localName, ExceptionState &)
{
    if (localName == HTMLNames::formTag)
        return CrawlerFormElement::Create(*this);
    return CrawlerElement::Create(localName, this);
}

} // namespace BlinKit
