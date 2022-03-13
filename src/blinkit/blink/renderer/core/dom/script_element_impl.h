#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: script_element_impl.h
// Description: ScriptElementImpl Class
//      Author: Ziming Li
//     Created: 2022-03-13
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SCRIPT_ELEMENT_IMPL_H
#define BLINKIT_SCRIPT_ELEMENT_IMPL_H

#include "blinkit/blink/renderer/core/dom/Attribute.h"
#include "blinkit/blink/renderer/core/dom/container_node.h"
#include "blinkit/blink/renderer/core/dom/script_loader.h"
#include "blinkit/blink/renderer/core/dom/script_loader_client.h"
#include "blinkit/blink/renderer/core/dom/ScriptRunner.h"
#include "blinkit/blink/renderer/core/events/Event.h"
#include "blinkit/blink/renderer/core/HTMLNames.h"

namespace BlinKit {

template <class ElementBase>
class ScriptElementImpl : public ElementBase, public ScriptLoaderClient
{
public:
    ScriptLoader* loader(void) const { return m_loader.get(); }
protected:
    ScriptElementImpl(Document &document, bool insertedByParser, bool alreadyStarted)
        : ElementBase(HTMLNames::scriptTag, document)
        , m_loader(ScriptLoader::create(this, insertedByParser, alreadyStarted))
    {
    }

    // ScriptLoaderClient
    String sourceAttributeValue(void) const final {
        return ElementBase::getAttribute(HTMLNames::srcAttr).string();
    }
private:
    // Node
    Node::InsertionNotificationRequest insertedInto(ContainerNode *insertionPoint) final
    {
        ElementBase::insertedInto(insertionPoint);
        return Node::InsertionShouldCallDidNotifySubtreeInsertions;
    }
    void didNotifySubtreeInsertionsToDocument(void) final
    {
        m_loader->didNotifySubtreeInsertionsToDocument();
    }
    void didMoveToNewDocument(Document &oldDocument) final
    {
        ScriptRunner::movePendingScript(oldDocument, ElementBase::document(), m_loader.get());
        ElementBase::didMoveToNewDocument(oldDocument);
    }
    // ContainerNode
    void childrenChanged(const ContainerNode::ChildrenChange &change) final
    {
        ElementBase::childrenChanged(change);
        if (change.isChildInsertion())
            m_loader->childrenChanged();
    }
    // Element
    void parseAttribute(const QualifiedName &name, const AtomicString &oldValue, const AtomicString &value) final
    {
        if (name == HTMLNames::srcAttr)
            m_loader->handleSourceAttribute(value);
        else
            ElementBase::parseAttribute(name, oldValue, value);
    }
    bool hasLegalLinkAttribute(const QualifiedName &name) const final {
        return name == HTMLNames::srcAttr || ElementBase::hasLegalLinkAttribute(name);
    }
    const QualifiedName& subResourceAttributeName(void) const final {
        return HTMLNames::srcAttr;
    }
    bool isURLAttribute(const Attribute &attribute) const final {
        return attribute.name() == HTMLNames::srcAttr || ElementBase::isURLAttribute(attribute);
    }

    // ScriptLoaderClient
    void dispatchLoadEvent(void) final
    {
        ASSERT(!m_loader->haveFiredLoadEvent());
        ElementBase::dispatchEvent(Event::create(EventTypeNames::load));
    }
    String charsetAttributeValue(void) const final {
        return ElementBase::getAttribute(HTMLNames::charsetAttr).string();
    }
    String typeAttributeValue(void) const final {
        return ElementBase::getAttribute(HTMLNames::typeAttr).string();
    }
    String languageAttributeValue(void) const final {
        return ElementBase::getAttribute(HTMLNames::languageAttr).string();
    }
    bool hasSourceAttribute(void) const final {
        return ElementBase::fastHasAttribute(HTMLNames::srcAttr);
    }

    std::unique_ptr<ScriptLoader> m_loader;
};

} // namespace BlinKit

#endif // BLINKIT_SCRIPT_ELEMENT_IMPL_H
