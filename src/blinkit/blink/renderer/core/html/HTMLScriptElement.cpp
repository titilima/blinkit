// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: HTMLScriptElement.cpp
// Description: HTMLScriptElement Class
//      Author: Ziming Li
//     Created: 2021-07-24
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "./HTMLScriptElement.h"

#include "blinkit/blink/renderer/bindings/core/v8/ExceptionStatePlaceholder.h"
#include "blinkit/blink/renderer/core/HTMLNames.h"
#include "blinkit/blink/renderer/core/dom/Attribute.h"
#include "blinkit/blink/renderer/core/dom/Document.h"
#include "blinkit/blink/renderer/core/dom/ScriptLoader.h"
#include "blinkit/blink/renderer/core/dom/ScriptRunner.h"
#include "blinkit/blink/renderer/core/dom/Text.h"
#include "blinkit/blink/renderer/core/events/Event.h"

using namespace BlinKit;

namespace blink {

using namespace HTMLNames;

inline HTMLScriptElement::HTMLScriptElement(Document& document, bool wasInsertedByParser, bool alreadyStarted)
    : HTMLElement(scriptTag, document)
    , m_loader(ScriptLoader::create(this, wasInsertedByParser, alreadyStarted))
{
}

GCRefPtr<HTMLScriptElement> HTMLScriptElement::create(Document& document, bool wasInsertedByParser, bool alreadyStarted)
{
    return GCWrapShared(new HTMLScriptElement(document, wasInsertedByParser, alreadyStarted));
}

bool HTMLScriptElement::isURLAttribute(const Attribute& attribute) const
{
    return attribute.name() == srcAttr || HTMLElement::isURLAttribute(attribute);
}

bool HTMLScriptElement::hasLegalLinkAttribute(const QualifiedName& name) const
{
    return name == srcAttr || HTMLElement::hasLegalLinkAttribute(name);
}

const QualifiedName& HTMLScriptElement::subResourceAttributeName() const
{
    return srcAttr;
}

void HTMLScriptElement::childrenChanged(const ChildrenChange& change)
{
    HTMLElement::childrenChanged(change);
    if (change.isChildInsertion())
        m_loader->childrenChanged();
}

void HTMLScriptElement::didMoveToNewDocument(Document& oldDocument)
{
    ScriptRunner::movePendingScript(oldDocument, document(), m_loader.get());
    HTMLElement::didMoveToNewDocument(oldDocument);
}

void HTMLScriptElement::parseAttribute(const QualifiedName& name, const AtomicString& oldValue, const AtomicString& value)
{
    if (name == srcAttr)
        m_loader->handleSourceAttribute(value);
    else
        HTMLElement::parseAttribute(name, oldValue, value);
}

Node::InsertionNotificationRequest HTMLScriptElement::insertedInto(ContainerNode* insertionPoint)
{
    HTMLElement::insertedInto(insertionPoint);
    return InsertionShouldCallDidNotifySubtreeInsertions;
}

void HTMLScriptElement::didNotifySubtreeInsertionsToDocument()
{
    m_loader->didNotifySubtreeInsertionsToDocument();
}

void HTMLScriptElement::setText(const String &value)
{
    setTextContent(value);
}

KURL HTMLScriptElement::src() const
{
    return document().completeURL(sourceAttributeValue());
}

String HTMLScriptElement::sourceAttributeValue() const
{
    return getAttribute(srcAttr).string();
}

String HTMLScriptElement::charsetAttributeValue() const
{
    return getAttribute(charsetAttr).string();
}

String HTMLScriptElement::typeAttributeValue() const
{
    return getAttribute(typeAttr).string();
}

String HTMLScriptElement::languageAttributeValue() const
{
    return getAttribute(languageAttr).string();
}

#ifdef BLINKIT_CRAWLER_ENABLED
String HTMLScriptElement::forAttributeValue() const
{
    return String();
}

String HTMLScriptElement::eventAttributeValue() const
{
    return String();
}
#endif

bool HTMLScriptElement::asyncAttributeValue() const
{
    return false;
}

bool HTMLScriptElement::deferAttributeValue() const
{
    return false;
}

bool HTMLScriptElement::hasSourceAttribute() const
{
    return fastHasAttribute(srcAttr);
}

void HTMLScriptElement::dispatchLoadEvent()
{
    ASSERT(!m_loader->haveFiredLoadEvent());
    dispatchEvent(Event::create(EventTypeNames::load));
}

GCRefPtr<Element> HTMLScriptElement::cloneElementWithoutAttributesAndChildren()
{
    return GCWrapShared(new HTMLScriptElement(document(), false, m_loader->alreadyStarted()));
}

}
