#pragma once
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

#ifndef BLINKIT_CRAWLER_ELEMENT_H
#define BLINKIT_CRAWLER_ELEMENT_H

#pragma once

#include "blinkit/blink/renderer/core/dom/element.h"

namespace BlinKit {

class CrawlerElement : public Element
{
public:
    CrawlerElement(const AtomicString &localName, Document *document);
protected:
    CrawlerElement(const QualifiedName &tagName, Document &document);

    bool isURLAttribute(const blink::Attribute &attribute) const override;
private:
    bool isFormControlElement(void) const final;
    bool isDisabledFormControl(void) const final;

#ifndef NDEBUG
    const std::string m_localNameForDebug;
#endif
};

} // namespace BlinKit

#endif // BLINKIT_CRAWLER_ELEMENT_H
