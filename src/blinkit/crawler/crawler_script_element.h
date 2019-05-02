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

#include "core/dom/ScriptLoader.h"
#include "core/dom/ScriptLoaderClient.h"
#include "crawler_element.h"

namespace BlinKit {

class CrawlerScriptElement final : public CrawlerElement, public blink::ScriptLoaderClient
{
public:
    static PassRefPtrWillBeRawPtr<CrawlerScriptElement> Create(blink::Document &, bool wasInsertedByParser, bool alreadyStarted);
private:
    CrawlerScriptElement(blink::Document &, bool wasInsertedByParser, bool alreadyStarted);

    void parseAttribute(const blink::QualifiedName&, const AtomicString&, const AtomicString&) override;
    void didNotifySubtreeInsertionsToDocument(void) override;
    void childrenChanged(const ChildrenChange&) override;
    void didMoveToNewDocument(blink::Document& oldDocument) override;

    bool isURLAttribute(const blink::Attribute&) const override;
    bool hasLegalLinkAttribute(const blink::QualifiedName&) const override;
    const blink::QualifiedName& subResourceAttributeName(void) const override;

    String sourceAttributeValue(void) const override;
    String charsetAttributeValue(void) const override;
    String typeAttributeValue(void) const override;
    String languageAttributeValue(void) const override;
    String forAttributeValue(void) const override;
    String eventAttributeValue(void) const override;
    bool asyncAttributeValue(void) const override;
    bool deferAttributeValue(void) const override;
    bool hasSourceAttribute(void) const override;

    void dispatchLoadEvent(void) override;

    PassRefPtrWillBeRawPtr<blink::Element> cloneElementWithoutAttributesAndChildren(void) override;

    OwnPtrWillBeMember<blink::ScriptLoader> m_loader;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CRAWLER_SCRIPT_ELEMENT_H
