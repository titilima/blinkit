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

#include "blinkit/blink/renderer/core/HTMLNames.h"
#include "blinkit/crawler/dom/crawler_script_element.h"

namespace BlinKit {

CrawlerDocument::CrawlerDocument(const DocumentInit &init) : Document(init, HTMLDocumentClass | CrawlerDocumentClass)
{
    // Nothing
}

GCRefPtr<Element> CrawlerDocument::createElement(const AtomicString &name, Element *form, bool createdByParser)
{
    using namespace HTMLNames;

    ASSERT(nullptr == form); // BKTODO:
    if (scriptTag.localName() == name)
        ASSERT(false); // BKTODO:
    return GCWrapShared(new CrawlerElement(name, this));
}

} // namespace BlinKit
