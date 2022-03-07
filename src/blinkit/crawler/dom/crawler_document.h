// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: crawler_document.h
// Description: CrawlerDocument Class
//      Author: Ziming Li
//     Created: 2019-04-14
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_CRAWLER_DOCUMENT_H
#define BLINKIT_BLINKIT_CRAWLER_DOCUMENT_H

#pragma once

#include "blinkit/blink/renderer/core/dom/document.h"

namespace BlinKit {

class CrawlerDocument final : public Document
{
public:
    static GCRefPtr<CrawlerDocument> Create(const DocumentInit &initializer)
    {
        return GCWrapShared(new CrawlerDocument(initializer));
    }
private:
    CrawlerDocument(const DocumentInit &init);

    GCRefPtr<Element> createElement(const AtomicString &name, Element *form, bool createdByParser) override;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CRAWLER_DOCUMENT_H
