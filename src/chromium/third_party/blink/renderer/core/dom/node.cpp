// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: node.cpp
// Description: Node Class
//      Author: Ziming Li
//     Created: 2019-09-16
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011 Apple Inc. All
 * rights reserved.
 * Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2009 Torch Mobile Inc. All rights reserved.
 * (http://www.torchmobile.com/)
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

#include "node.h"

#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/dom/element.h"
#include "third_party/blink/renderer/core/dom/events/event.h"
#include "third_party/blink/renderer/core/dom/events/event_dispatcher.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"

namespace blink {

Node::Node(TreeScope *treeScope, ConstructionType type)
    : m_nodeFlags(type)
    , m_treeScope(treeScope)
{
    ASSERT(nullptr != treeScope || kCreateDocument == type  || kCreateShadowRoot == type || kCreateCrawlerDocument == type);
}

Node::~Node(void)
{
    ASSERT(false); // BKTODO:
}

Node* Node::CommonAncestor(const Node &other, ParentGetter getParent) const
{
    if (this == other)
        return const_cast<Node *>(this);
    if (GetDocument() != other.GetDocument())
        return nullptr;
    int thisDepth = 0;
    for (const Node *node = this; nullptr != node; node = getParent(*node))
    {
        if (node == &other)
            return const_cast<Node *>(node);
        ++thisDepth;
    }
    int otherDepth = 0;
    for (const Node *node = &other; nullptr != node; node = getParent(*node))
    {
        if (node == this)
            return const_cast<Node *>(this);
        ++otherDepth;
    }
    const Node *thisIterator = this;
    const Node *otherIterator = &other;
    if (thisDepth > otherDepth)
    {
        for (int i = thisDepth; i > otherDepth; --i)
            thisIterator = getParent(*thisIterator);
    }
    else if (otherDepth > thisDepth)
    {
        for (int i = otherDepth; i > thisDepth; --i)
            otherIterator = getParent(*otherIterator);
    }
    while (nullptr != thisIterator)
    {
        if (thisIterator == otherIterator)
            return const_cast<Node *>(thisIterator);
        thisIterator = getParent(*thisIterator);
        otherIterator = getParent(*otherIterator);
    }
    ASSERT(nullptr == otherIterator);
    return nullptr;
}

unsigned Node::CountChildren(void) const
{
    if (!IsContainerNode())
        return 0;
    return ToContainerNode(this)->CountChildren();
}

void Node::DefaultEventHandler(Event &event)
{
    if (event.target() != this)
        return;
#ifndef BLINKIT_CRAWLER_ONLY
    ASSERT(false); // BKTODO:
    const AtomicString &eventType = event.type();
    if (event_type == EventTypeNames::keydown ||
        event_type == EventTypeNames::keypress) {
        if (event.IsKeyboardEvent()) {
            if (LocalFrame* frame = GetDocument().GetFrame()) {
                frame->GetEventHandler().DefaultKeyboardEventHandler(
                    ToKeyboardEvent(&event));
            }
        }
    }
    else if (event_type == EventTypeNames::click) {
        int detail = event.IsUIEvent() ? ToUIEvent(event).detail() : 0;
        if (DispatchDOMActivateEvent(detail, event) !=
            DispatchEventResult::kNotCanceled)
            event.SetDefaultHandled();
    }
    else if (event_type == EventTypeNames::contextmenu &&
        event.IsMouseEvent()) {
        if (Page* page = GetDocument().GetPage()) {
            page->GetContextMenuController().HandleContextMenuEvent(
                ToMouseEvent(&event));
        }
    }
    else if (event_type == EventTypeNames::textInput) {
        if (event.HasInterface(EventNames::TextEvent)) {
            if (LocalFrame* frame = GetDocument().GetFrame()) {
                frame->GetEventHandler().DefaultTextInputEventHandler(
                    ToTextEvent(&event));
            }
        }
    }
    else if (RuntimeEnabledFeatures::MiddleClickAutoscrollEnabled() &&
        event_type == EventTypeNames::mousedown && event.IsMouseEvent()) {
        auto& mouse_event = ToMouseEvent(event);
        if (mouse_event.button() ==
            static_cast<short>(WebPointerProperties::Button::kMiddle)) {
            if (EnclosingLinkEventParentOrSelf())
                return;

            // Avoid that canBeScrolledAndHasScrollableArea changes layout tree
            // structure.
            // FIXME: We should avoid synchronous layout if possible. We can
            // remove this synchronous layout if we avoid synchronous layout in
            // LayoutTextControlSingleLine::scrollHeight
            GetDocument().UpdateStyleAndLayoutIgnorePendingStylesheets();
            LayoutObject* layout_object = GetLayoutObject();
            while (
                layout_object &&
                (!layout_object->IsBox() ||
                    !ToLayoutBox(layout_object)->CanBeScrolledAndHasScrollableArea())) {
                if (auto* document = DynamicTo<Document>(layout_object->GetNode())) {
                    Element* owner = document->LocalOwner();
                    layout_object = owner ? owner->GetLayoutObject() : nullptr;
                }
                else {
                    layout_object = layout_object->Parent();
                }
            }
            if (layout_object) {
                if (LocalFrame* frame = GetDocument().GetFrame())
                    frame->GetEventHandler().StartMiddleClickAutoscroll(layout_object);
            }
        }
    }
    else if (event_type == EventTypeNames::mouseup && event.IsMouseEvent()) {
        auto& mouse_event = ToMouseEvent(event);
        if (mouse_event.button() ==
            static_cast<short>(WebPointerProperties::Button::kBack)) {
            if (LocalFrame* frame = GetDocument().GetFrame()) {
                if (frame->Client()->NavigateBackForward(-1))
                    event.SetDefaultHandled();
            }
        }
        else if (mouse_event.button() ==
            static_cast<short>(WebPointerProperties::Button::kForward)) {
            if (LocalFrame* frame = GetDocument().GetFrame()) {
                if (frame->Client()->NavigateBackForward(1))
                    event.SetDefaultHandled();
            }
        }
    }
#endif
}

void Node::DidNotifySubtreeInsertionsToDocument(void)
{
    ASSERT(false); // BKTODO:
}

DispatchEventResult Node::DispatchEventInternal(Event &event)
{
    return EventDispatcher::DispatchEvent(*this, event);
}

Node* Node::firstChild(void) const
{
    if (!IsContainerNode())
        return nullptr;
    return ToContainerNode(this)->firstChild();
}

void Node::HandleLocalEvents(Event &event)
{
    if (!HasEventTargetData())
        return;

    ASSERT(false); // BKTODO:
#if 0
    if (IsDisabledFormControl(this) && event.IsMouseEvent() &&
        !RuntimeEnabledFeatures::SendMouseEventsDisabledFormControlsEnabled()) {
        if (HasEventListeners(event.type())) {
            UseCounter::Count(GetDocument(),
                WebFeature::kDispatchMouseEventOnDisabledFormControl);
            if (event.type() == EventTypeNames::mousedown ||
                event.type() == EventTypeNames::mouseup) {
                UseCounter::Count(
                    GetDocument(),
                    WebFeature::kDispatchMouseUpDownEventOnDisabledFormControl);
            }
        }
        return;
    }

    FireEventListeners(event);
#endif
}

bool Node::HasTagName(const HTMLQualifiedName &name) const
{
    ASSERT(false); // BKTODO:
    return false;
}

Node::InsertionNotificationRequest Node::InsertedInto(ContainerNode &insertionPoint)
{
#ifndef BLINKIT_CRAWLER_ONLY
    ASSERT(false); // BKTODO:
    DCHECK(!ChildNeedsStyleInvalidation());
    DCHECK(!NeedsStyleInvalidation());
#endif
    ASSERT(insertionPoint.isConnected() || insertionPoint.IsInShadowTree() || IsContainerNode());
    if (insertionPoint.isConnected())
    {
        SetFlag(kIsConnectedFlag);
        insertionPoint.GetDocument().IncrementNodeCount();
    }
    if (ParentOrShadowHostNode()->IsInShadowTree())
        SetFlag(kIsInShadowTreeFlag);
#ifndef BLINKIT_CRAWLER_ONLY
    ASSERT(false); // BKTODO:
    if (ChildNeedsDistributionRecalc() && !insertion_point.ChildNeedsDistributionRecalc())
        insertion_point.MarkAncestorsWithChildNeedsDistributionRecalc();
#endif
    return kInsertionDone;
}

bool Node::IsDescendantOf(const Node *other) const
{
    ASSERT(false); // BKTODO:
    return false;
}

bool Node::IsDocumentNode(void) const
{
    return this == GetDocument();
}

bool Node::IsTreeScope(void) const
{
    ASSERT(false); // BKTODO:
    return false;
}

bool Node::IsUserActionElementActive(void) const
{
    ASSERT(false); // BKTODO:
    return false;
}

Node* Node::lastChild(void) const
{
    if (!IsContainerNode())
        return nullptr;
    return ToContainerNode(this)->lastChild();
}

bool Node::MayContainLegacyNodeTreeWhereDistributionShouldBeSupported(void) const
{
    if (isConnected() && !GetDocument().MayContainV0Shadow())
    {
        // TODO(crbug.com/787717): Some built-in elements still use <content>
        // elements in their user-agent shadow roots. DCHECK() fails if such an
        // element is used.
        ASSERT(!GetDocument().ChildNeedsDistributionRecalc());
        return false;
    }
    return true;
}

unsigned Node::NodeIndex(void) const
{
    unsigned count = 0;
    const Node *n = previousSibling();
    while (nullptr != n)
    {
        n = n->previousSibling();
        ++count;
    }
    return count;
}

String Node::nodeValue(void) const
{
    return String();
}

Element* Node::parentElement(void) const
{
    if (ContainerNode *parent = parentNode())
    {
        if (parent->IsElementNode())
            return ToElement(parent);
    }
    return nullptr;
}

ContainerNode* Node::ParentOrShadowHostNode(void) const
{
    ASSERT(IsMainThread());
    return reinterpret_cast<ContainerNode *>(m_parentOrShadowHostNode.Get());
}

ContainerNode* Node::parentNode(void) const
{
    return IsShadowRoot() ? nullptr : ParentOrShadowHostNode();
}

Node* Node::PseudoAwareNextSibling(void) const
{
    ASSERT(false); // BKTODO:
    return nullptr;
}

Node* Node::PseudoAwarePreviousSibling(void) const
{
    ASSERT(false); // BKTODO:
    return nullptr;
}

Node* Node::PseudoAwareFirstChild(void) const
{
    ASSERT(false); // BKTODO:
    return nullptr;
}

Node* Node::PseudoAwareLastChild(void) const
{
    ASSERT(false); // BKTODO:
    return nullptr;
}

void Node::RemovedFrom(ContainerNode &insertionPoint)
{
    ASSERT(false); // BKTODO:
}

void Node::setNodeValue(const String &nodeValue)
{
    // By default, setting nodeValue has no effect.
}

void Node::SetParentOrShadowHostNode(ContainerNode *parent)
{
    ASSERT(IsMainThread());
    m_parentOrShadowHostNode = parent;
}

void Node::UpdateDistributionInternal(void)
{
    if (!MayContainLegacyNodeTreeWhereDistributionShouldBeSupported())
        return;
    ASSERT(false); // BKTODO:
#if 0
    // Extra early out to avoid spamming traces.
    if (isConnected() && !GetDocument().ChildNeedsDistributionRecalc())
        return;
    ScriptForbiddenScope forbid_script;
    Node& root = ShadowIncludingRoot();
    if (root.ChildNeedsDistributionRecalc())
        root.RecalcDistribution();
#endif
}

void Node::WillCallDefaultEventHandler(const Event &event)
{
#ifndef BLINKIT_CRAWLER_ONLY
    ASSERT(false); // BKTODO:
#endif
}

}  // namespace blink
