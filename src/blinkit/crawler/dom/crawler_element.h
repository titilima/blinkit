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

#include "blinkit/blink/renderer/core/dom/Element.h"

namespace BlinKit {

class CrawlerElement : public blink::Element
{
public:
    CrawlerElement(const AtomicString &localName, blink::Document *document);
protected:
    bool isURLAttribute(const blink::Attribute &attribute) const override;
private:
    bool isFormControlElement(void) const final;
    bool isDisabledFormControl(void) const final;

#ifndef NDEBUG
    const std::string m_localNameForDebug;
#endif
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CRAWLER_ELEMENT_H
