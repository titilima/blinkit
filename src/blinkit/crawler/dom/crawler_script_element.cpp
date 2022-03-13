// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: crawler_script_element.cpp
// Description: CrawlerScriptElement Class
//      Author: Ziming Li
//     Created: 2019-04-30
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "./crawler_script_element.h"

namespace BlinKit {

CrawlerScriptElement::CrawlerScriptElement(Document &document, bool insertedByParser, bool alreadyStarted)
    : ScriptElementImpl(document, insertedByParser, alreadyStarted)
{
}

GCRefPtr<Element> CrawlerScriptElement::cloneElementWithoutAttributesAndChildren(void)
{
    return GCWrapShared(new CrawlerScriptElement(document(), false, loader()->alreadyStarted()));
}

} // namespace BlinKit
