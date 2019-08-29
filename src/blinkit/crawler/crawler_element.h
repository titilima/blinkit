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

#include "core/dom/Element.h"

namespace BlinKit {

class CrawlerElement : public blink::Element
{
public:
    static PassRefPtrWillBeRawPtr<CrawlerElement> Create(const AtomicString &localName, blink::Document *document) {
        return adoptRefWillBeNoop(new CrawlerElement(localName, document));
    }
protected:
    CrawlerElement(const AtomicString &localName, blink::Document *document);

    bool isURLAttribute(const blink::Attribute &attribute) const;
private:
    bool isFormControlElement(void) const final;
    bool isDisabledFormControl(void) const final;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CRAWLER_ELEMENT_H
