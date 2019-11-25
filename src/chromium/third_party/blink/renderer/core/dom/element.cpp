// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: element.cpp
// Description: Element Class
//      Author: Ziming Li
//     Created: 2019-10-18
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Peter Kelly (pmk@post.com)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 *           (C) 2007 David Smith (catfish.man@gmail.com)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2012, 2013 Apple Inc.
 * All rights reserved.
 *           (C) 2007 Eric Seidel (eric@webkit.org)
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

#include "element.h"

#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/dom/element_data.h"
#include "third_party/blink/renderer/core/dom/element_rare_data.h"
#include "third_party/blink/renderer/core/html_names.h"
#include "third_party/blink/renderer/platform/runtime_enabled_features.h"

namespace blink {

Element::Element(const QualifiedName &tagName, Document *document, ConstructionType type)
    : ContainerNode(document, type), m_tagName(tagName)
{
    // Nothing
}

Element::~Element(void)
{
    ASSERT(false); // BKTODO: DCHECK(NeedsAttach());
}

void Element::AttributeChanged(const AttributeModificationParams &params)
{
    ASSERT(false); // BKTODO:
}

AttributeCollection Element::Attributes(void) const
{
    ASSERT(false); // BKTODO:
    return AttributeCollection();
}

AttributeCollection Element::AttributesWithoutUpdate(void) const
{
    ASSERT(false); // BKTODO:
    return AttributeCollection();
}

#ifndef BLINKIT_CRAWLER_ONLY
void Element::ChildrenChanged(const ChildrenChange &change)
{
    ContainerNode::ChildrenChanged(change);

    ASSERT(false); // BKTODO:
#if 0
    CheckForEmptyStyleChange(change.sibling_before_change,
        change.sibling_after_change);

    if (!change.by_parser && change.IsChildElementChange())
        CheckForSiblingStyleChanges(
            change.type == kElementRemoved ? kSiblingElementRemoved
            : kSiblingElementInserted,
            ToElement(change.sibling_changed), change.sibling_before_change,
            change.sibling_after_change);

    if (ShadowRoot* shadow_root = GetShadowRoot())
        shadow_root->SetNeedsDistributionRecalcWillBeSetNeedsAssignmentRecalc();
#endif
}
#endif

Node* Element::Clone(Document &document, CloneChildrenFlag flag) const
{
    ASSERT(false); // BKTODO:
    return nullptr;
}

#ifndef BLINKIT_CRAWLER_ONLY
void Element::DefaultEventHandler(Event & event)
{
    if (RuntimeEnabledFeatures::InvisibleDOMEnabled() &&
        event.type() == EventTypeNames::activateinvisible &&
        event.target() == this) {
        removeAttribute(invisibleAttr);
        event.SetDefaultHandled();
        return;
    }
    ContainerNode::DefaultEventHandler(event);
}
#endif

const AtomicString& Element::FastGetAttribute(const QualifiedName &name) const
{
    ASSERT(false); // BKTODO:
    return g_null_atom;
}

void Element::FinishParsingChildren(void)
{
    SetIsFinishedParsingChildren(true);
#ifndef BLINKIT_CRAWLER_ONLY
    ASSERT(false); // BKTODO:
    CheckForEmptyStyleChange(this, this);
    CheckForSiblingStyleChanges(kFinishedParsingChildren, nullptr, lastChild(), nullptr);
#endif
    // BKTODO: Check HTML element overrides.
}

const AtomicString& Element::getAttribute(const QualifiedName &name) const
{
    ASSERT(false); // BKTODO:
    return g_null_atom;
}

Attr* Element::getAttributeNode(const AtomicString &name)
{
    ASSERT(false); // BKTODO:
    return nullptr;
}

Attr* Element::getAttributeNodeNS(const AtomicString &namespaceURI, const AtomicString &localName)
{
    ASSERT(false); // BKTODO:
    return nullptr;
}

ElementRareData* Element::GetElementRareData(void) const
{
    ASSERT(HasRareData());
    ASSERT(false); // BKTODO:
    return nullptr;
}

const AtomicString& Element::GetIdAttribute(void) const
{
    return HasID() ? FastGetAttribute(html_names::kIdAttr) : g_null_atom;
}

const AtomicString& Element::GetNameAttribute(void) const
{
    return HasName() ? FastGetAttribute(html_names::kNameAttr) : g_null_atom;
}

bool Element::HasID(void) const
{
    if (const ElementData *elementData = GetElementData())
        return elementData->HasID();
    return false;
}

Node::InsertionNotificationRequest Element::InsertedInto(ContainerNode &insertionPoint)
{
    // need to do superclass processing first so isConnected() is true
    // by the time we reach updateId
    ContainerNode::InsertedInto(insertionPoint);

    ASSERT(!HasRareData() || !GetElementRareData()->HasPseudoElements());

    if (!insertionPoint.IsInTreeScope())
        return kInsertionDone;

    if (HasRareData())
    {
        ASSERT(false); // BKTODO:
#if 0
        ElementRareData* rare_data = GetElementRareData();
        if (rare_data->IntersectionObserverData() &&
            rare_data->IntersectionObserverData()->HasObservations()) {
            GetDocument().EnsureIntersectionObserverController().AddTrackedTarget(
                *this);
            if (LocalFrameView* frame_view = GetDocument().View())
                frame_view->SetIntersectionObservationState(LocalFrameView::kRequired);
        }
#endif
    }

#ifndef BLINKIT_CRAWLER_ONLY
    if (isConnected())
    {
        ASSERT(false); // BKTODO:
#if 0
        if (GetCustomElementState() == CustomElementState::kCustom)
            CustomElement::EnqueueConnectedCallback(this);
        else if (IsUpgradedV0CustomElement())
            V0CustomElement::DidAttach(this, GetDocument());
        else if (GetCustomElementState() == CustomElementState::kUndefined)
            CustomElement::TryToUpgrade(this);
#endif
    }
#endif

    TreeScope &scope = insertionPoint.GetTreeScope();
#ifdef BLINKIT_CRAWLER_ONLY
    ASSERT(GetTreeScope() == scope);
#else
    ASSERT(false); // BKTODO:
    if (GetTreeScope() != scope)
        return kInsertionDone;
#endif

    const AtomicString &idValue = GetIdAttribute();
    if (!idValue.IsNull())
        UpdateId(scope, g_null_atom, idValue);

    const AtomicString &nameValue = GetNameAttribute();
    if (!nameValue.IsNull())
        UpdateName(g_null_atom, nameValue);

    return kInsertionDone;
}

AtomicString Element::LowercaseIfNecessary(const AtomicString &name) const
{
    ASSERT(false); // BKTODO:
    return g_null_atom;
}

String Element::nodeName(void) const
{
    return m_tagName.ToString();
}

void Element::ParserSetAttributes(const Vector<Attribute> &attributeVector)
{
    ASSERT(!isConnected());
    ASSERT(nullptr == parentNode());
    ASSERT(!m_elementData);

    if (!attributeVector.IsEmpty())
    {
        ASSERT(false); // BKTODO:
#if 0
        if (GetDocument().GetElementDataCache())
            element_data_ =
            GetDocument()
            .GetElementDataCache()
            ->CachedShareableElementDataWithAttributes(attribute_vector);
        else
            element_data_ =
            ShareableElementData::CreateWithAttributes(attribute_vector);
#endif
    }

    ASSERT(!HasTagName(html_names::kInputTag)); // BKTODO: ParserDidSetAttributes
    ParserDidSetAttributes();

    for (const auto &attribute : attributeVector)
    {
        AttributeModificationParams params(attribute.GetName(), g_null_atom, attribute.Value(), AttributeModificationReason::kByParser);
        AttributeChanged(params);
    }
}

void Element::setAttribute(const QualifiedName &name, const AtomicString &value)
{
    ASSERT(false); // BKTODO:
}

void Element::StripScriptingAttributes(Vector<Attribute> &attributeVector) const
{
    ASSERT(false); // BKTODO:
}

void Element::UpdateId(TreeScope &scope, const AtomicString &oldId, const AtomicString &newId)
{
    ASSERT(false); // BKTODO:
}

void Element::UpdateName(const AtomicString &oldName, const AtomicString &newName)
{
    ASSERT(false); // BKTODO:
}

} // namespace blink
