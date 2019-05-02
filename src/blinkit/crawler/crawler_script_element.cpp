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

#include "crawler_script_element.h"

#include "core/HTMLNames.h"
#include "core/dom/ScriptRunner.h"
#include "core/events/Event.h"

using namespace blink;
using namespace HTMLNames;

namespace BlinKit {

inline CrawlerScriptElement::CrawlerScriptElement(Document &document, bool wasInsertedByParser, bool alreadyStarted)
    : CrawlerElement(scriptTag.localName(), &document)
    , m_loader(ScriptLoader::create(this, wasInsertedByParser, alreadyStarted))
{
}

bool CrawlerScriptElement::asyncAttributeValue(void) const
{
    return fastHasAttribute(asyncAttr);
}

String CrawlerScriptElement::charsetAttributeValue(void) const
{
    return getAttribute(charsetAttr).string();
}

void CrawlerScriptElement::childrenChanged(const ChildrenChange& change)
{
    CrawlerElement::childrenChanged(change);
    if (change.isChildInsertion())
        m_loader->childrenChanged();
}

PassRefPtrWillBeRawPtr<Element> CrawlerScriptElement::cloneElementWithoutAttributesAndChildren(void)
{
    return adoptRefWillBeNoop(new CrawlerScriptElement(document(), false, m_loader->alreadyStarted()));
}

PassRefPtrWillBeRawPtr<CrawlerScriptElement> CrawlerScriptElement::Create(Document& document, bool wasInsertedByParser, bool alreadyStarted)
{
    return adoptRefWillBeNoop(new CrawlerScriptElement(document, wasInsertedByParser, alreadyStarted));
}

bool CrawlerScriptElement::deferAttributeValue(void) const
{
    return fastHasAttribute(deferAttr);
}

void CrawlerScriptElement::didMoveToNewDocument(Document &oldDocument)
{
    ScriptRunner::movePendingScript(oldDocument, document(), m_loader.get());
    CrawlerElement::didMoveToNewDocument(oldDocument);
}

void CrawlerScriptElement::didNotifySubtreeInsertionsToDocument(void)
{
    m_loader->didNotifySubtreeInsertionsToDocument();
}

void CrawlerScriptElement::dispatchLoadEvent(void)
{
    ASSERT(!m_loader->haveFiredLoadEvent());
    dispatchEvent(Event::create(EventTypeNames::load));
}

String CrawlerScriptElement::eventAttributeValue(void) const
{
    return getAttribute(eventAttr).string();
}

String CrawlerScriptElement::forAttributeValue(void) const
{
    return getAttribute(forAttr).string();
}

bool CrawlerScriptElement::hasLegalLinkAttribute(const QualifiedName &name) const
{
    return name == srcAttr || CrawlerElement::hasLegalLinkAttribute(name);
}

bool CrawlerScriptElement::hasSourceAttribute(void) const
{
    return fastHasAttribute(srcAttr);
}

bool CrawlerScriptElement::isURLAttribute(const Attribute &attribute) const
{
    return attribute.name() == srcAttr || CrawlerElement::isURLAttribute(attribute);
}

String CrawlerScriptElement::languageAttributeValue(void) const
{
    return getAttribute(languageAttr).string();
}

void CrawlerScriptElement::parseAttribute(const QualifiedName &name, const AtomicString &oldValue, const AtomicString &value)
{
    if (name == srcAttr)
    {
        m_loader->handleSourceAttribute(value);
        logUpdateAttributeIfIsolatedWorldAndInDocument("script", srcAttr, oldValue, value);
    }
    else if (name == asyncAttr)
    {
        m_loader->handleAsyncAttribute();
    }
    else
    {
        CrawlerElement::parseAttribute(name, oldValue, value);
    }
}

String CrawlerScriptElement::sourceAttributeValue(void) const
{
    return getAttribute(srcAttr).string();
}

const QualifiedName& CrawlerScriptElement::subResourceAttributeName(void) const
{
    return srcAttr;
}

String CrawlerScriptElement::typeAttributeValue(void) const
{
    return getAttribute(typeAttr).string();
}

} // namespace BlinKit
