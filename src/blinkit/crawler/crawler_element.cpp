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

using namespace blink;

namespace BlinKit {

CrawlerElement::CrawlerElement(const QualifiedName &tagName, DocumentImpl *document)
    : ElementImpl(tagName, document, CreateCrawlerElement)
{
    // Nothing
}

PassRefPtrWillBeRawPtr<Node> CrawlerElement::cloneNode(bool deep)
{
    assert(false); // BKTODO:
    return nullptr;
}

} // namespace BlinKit
