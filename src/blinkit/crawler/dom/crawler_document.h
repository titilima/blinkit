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

#include "base/memory/ptr_util.h"
#include "third_party/blink/renderer/core/dom/document.h"

namespace BlinKit {

class CrawlerDocument final : public blink::Document
{
public:
    static std::unique_ptr<blink::Document> Create(const blink::DocumentInit &init)
    {
        return base::WrapUnique(new (ObjectType::Owner) CrawlerDocument(init));
    }
private:
    CrawlerDocument(const blink::DocumentInit &init);

    blink::Element* CreateElement(const AtomicString &localName, CreateElementFlags flags) override;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CRAWLER_DOCUMENT_H
