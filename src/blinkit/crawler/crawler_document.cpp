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

#include "crawler/crawler_element.h"

using namespace blink;

namespace BlinKit {

CrawlerDocument::CrawlerDocument(const DocumentInit &init, DocumentClassFlags extendedDocumentClasses)
    : DocumentImpl(init, HTMLDocumentClass | extendedDocumentClasses, CreateCrawlerDocument)
{
    // Nothing
}

PassRefPtrWillBeRawPtr<ElementImpl> CrawlerDocument::CreateElement(const QualifiedName &tag)
{
    return CrawlerElement::Create(tag, this);
}

PassRefPtrWillBeRawPtr<Node> CrawlerDocument::cloneNode(bool deep)
{
    assert(false); // BKTODO:
    return nullptr;
}

} // namespace BlinKit
