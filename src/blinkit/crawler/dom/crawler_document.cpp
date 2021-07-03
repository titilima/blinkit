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

#include "blinkit/crawler/dom/crawler_script_element.h"
// BKTODO: #include "third_party/blink/renderer/core/html_names.h"

using namespace blink;

namespace BlinKit {

CrawlerDocument::CrawlerDocument(const DocumentInit &init) : Document(init, HTMLDocumentClass | CrawlerDocumentClass)
{
    // Nothing
}

#if 0 // BKTODO:
Element* CrawlerDocument::CreateElement(const AtomicString &localName, CreateElementFlags flags)
{
    using namespace html_names;
    if (localName == kScriptTag.LocalName())
        return CrawlerScriptElement::Create(*this, flags);
    return new CrawlerElement(localName, this);
}
#endif

} // namespace BlinKit
