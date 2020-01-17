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

#include "blinkit/crawler/crawler_element.h"
#include "third_party/blink/renderer/core/dom/create_element_flags.h"
#include "third_party/blink/renderer/core/script/script_element_base.h"

namespace BlinKit {

class CrawlerScriptElement final : public CrawlerElement, public blink::ScriptElementBase
{
public:
    static CrawlerScriptElement* Create(blink::Document &document, const CreateElementFlags flags)
    {
        return new CrawlerScriptElement(document, flags);
    }
private:
    CrawlerScriptElement(blink::Document &document, const CreateElementFlags flags);

    // Node overrides
    InsertionNotificationRequest InsertedInto(ContainerNode &insertionPoint) override;
    void DidNotifySubtreeInsertionsToDocument(void) override;
    // ContainerNode overrides
    void ChildrenChanged(const ChildrenChange &change) override;
    // Element overrides
    void ParseAttribute(const AttributeModificationParams &params) override;
#if 0
    void DidMoveToNewDocument(Document& old_document) override;

    bool IsURLAttribute(const blink::Attribute&) const override;
    bool HasLegalLinkAttribute(const blink::QualifiedName&) const override;
    const blink::QualifiedName& SubResourceAttributeName() const override;
#endif

    // ScriptElementBase
    const blink::Element& GetElement(void) const override { return *this; }
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CRAWLER_SCRIPT_ELEMENT_H
