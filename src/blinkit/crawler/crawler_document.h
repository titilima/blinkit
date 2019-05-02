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

#include "core/dom/Document.h"

namespace blink {
class DocumentInit;
}

namespace BlinKit {

class CrawlerDocument final : public blink::Document
{
public:
    CrawlerDocument(const blink::DocumentInit &init);
private:
    PassRefPtrWillBeRawPtr<blink::Element> createElement(const AtomicString &localName, blink::ExceptionState&) override;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CRAWLER_DOCUMENT_H
