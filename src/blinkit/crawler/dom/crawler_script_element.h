// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: crawler_script_element.h
// Description: CrawlerScriptElement Class
//      Author: Ziming Li
//     Created: 2019-04-30
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_CRAWLER_SCRIPT_ELEMENT_H
#define BLINKIT_BLINKIT_CRAWLER_SCRIPT_ELEMENT_H

#pragma once

#include "blinkit/blink/renderer/core/dom/script_element_impl.h"
#include "blinkit/crawler/dom/crawler_element.h"

namespace BlinKit {

class CrawlerScriptElement final : public ScriptElementImpl<CrawlerElement>
{
public:
    static GCRefPtr<CrawlerScriptElement> Create(Document &document, bool insertedByParser,
        bool alreadyStarted = false)
    {
        return GCWrapShared(new CrawlerScriptElement(document, insertedByParser, alreadyStarted));
    }
private:
    CrawlerScriptElement(Document &document, bool insertedByParser, bool alreadyStarted);

    // Element overrides
    GCRefPtr<Element> cloneElementWithoutAttributesAndChildren(void) override;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CRAWLER_SCRIPT_ELEMENT_H
