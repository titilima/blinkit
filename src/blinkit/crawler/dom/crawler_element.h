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

#include "third_party/blink/renderer/core/dom/element.h"

namespace BlinKit {

class CrawlerElement : public blink::Element
{
public:
    CrawlerElement(const AtomicString &localName, blink::Document *document);
protected:
    bool IsURLAttribute(const blink::Attribute &attribute) const override;
private:
#if 0 // BKTODO:
    bool IsFormControlElement(void) const final;
    bool IsDisabledFormControl(void) const final;
#endif
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CRAWLER_ELEMENT_H
