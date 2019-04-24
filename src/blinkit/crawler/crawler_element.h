// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: crawler_element.h
// Description: CrawlerElement Class
//      Author: Ziming Li
//     Created: 2019-04-24
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_CRAWLER_ELEMENT_H
#define BLINKIT_BLINKIT_CRAWLER_ELEMENT_H

#pragma once

#include "core/dom/element_impl.h"

namespace BlinKit {

class CrawlerElement : public blink::ElementImpl
{
public:
    static PassRefPtrWillBeRawPtr<CrawlerElement> Create(const blink::QualifiedName &tagName, blink::DocumentImpl *document) {
        return adoptRefWillBeNoop(new CrawlerElement(tagName, document));
    }
protected:
    PassRefPtrWillBeRawPtr<Node> cloneNode(bool deep) override;
    CrawlerElement(const blink::QualifiedName &tagName, blink::DocumentImpl *document);
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CRAWLER_ELEMENT_H
