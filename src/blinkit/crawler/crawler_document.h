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

#include "core/dom/document_impl.h"

namespace blink {
class DocumentInit;
}

namespace BlinKit {

class CrawlerDocument final : public blink::DocumentImpl
{
public:
    CrawlerDocument(const blink::DocumentInit &init, blink::DocumentClassFlags extendedDocumentClasses = blink::DefaultDocumentClass);
private:
    PassRefPtrWillBeRawPtr<Node> cloneNode(bool deep) override;
    PassRefPtrWillBeRawPtr<blink::ElementImpl> CreateElement(const blink::QualifiedName &tag) override;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CRAWLER_DOCUMENT_H
