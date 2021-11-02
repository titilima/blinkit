// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: HTMLMarqueeElement.cpp
// Description: HTMLMarqueeElement Class
//      Author: Ziming Li
//     Created: 2021-07-30
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 * Copyright (C) 2003, 2007, 2010 Apple Inc. All rights reserved.
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
 *
 */

#include "core/html/HTMLMarqueeElement.h"

#include "core/HTMLNames.h"
#include "core/dom/Document.h"
#include "core/frame/UseCounter.h"

namespace blink {

inline HTMLMarqueeElement::HTMLMarqueeElement(Document& document)
    : HTMLElement(HTMLNames::marqueeTag, document)
{
    ASSERT(false); // BKTODO:
#if 0
    if (document.contextDocument()) {
        v8::Local<v8::Value> classObject = PrivateScriptRunner::installClassIfNeeded(&document, "HTMLMarqueeElement");
        RELEASE_ASSERT(!classObject.IsEmpty());
    }
    UseCounter::count(document, UseCounter::HTMLMarqueeElement);
#endif
}

PassRefPtrWillBeRawPtr<HTMLMarqueeElement> HTMLMarqueeElement::create(Document& document)
{
    ASSERT(false); // BKTODO:
    return nullptr;
#if 0
    RefPtrWillBeRawPtr<HTMLMarqueeElement> marqueeElement(adoptRefWillBeNoop(new HTMLMarqueeElement(document)));
    V8HTMLMarqueeElement::PrivateScript::createdCallbackMethod(document.frame(), marqueeElement.get());
    return marqueeElement.release();
#endif
}

void HTMLMarqueeElement::attributeChanged(const QualifiedName& name, const AtomicString& oldValue, const AtomicString& newValue, AttributeModificationReason reason)
{
    HTMLElement::attributeChanged(name, oldValue, newValue, reason);
    ASSERT(false); // BKTODO: V8HTMLMarqueeElement::PrivateScript::attributeChangedCallbackMethod(document().frame(), this, name.toString(), oldValue, newValue);
}

Node::InsertionNotificationRequest HTMLMarqueeElement::insertedInto(ContainerNode* insertionPoint)
{
    HTMLElement::insertedInto(insertionPoint);
    if (inDocument()) {
        ASSERT(false); // BKTODO: V8HTMLMarqueeElement::PrivateScript::attachedCallbackMethod(document().frame(), this);
    }
    return InsertionDone;
}

void HTMLMarqueeElement::removedFrom(ContainerNode* insertionPoint)
{
    HTMLElement::removedFrom(insertionPoint);
    if (insertionPoint->inDocument()) {
        ASSERT(false); // BKTODO: V8HTMLMarqueeElement::PrivateScript::detachedCallbackMethod(insertionPoint->document().frame(), this);
    }
}

bool HTMLMarqueeElement::isHorizontal() const
{
    AtomicString direction = getAttribute(HTMLNames::directionAttr);
    return direction != "down" && direction != "up";
}

} // namespace blink
