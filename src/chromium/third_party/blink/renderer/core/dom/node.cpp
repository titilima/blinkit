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

#include <unordered_map>
#include "third_party/blink/renderer/core/dom/attr.h"
#include "third_party/blink/renderer/core/dom/child_list_mutation_scope.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/dom/document_fragment.h"
#include "third_party/blink/renderer/core/dom/element_rare_data.h"
#include "third_party/blink/renderer/core/dom/events/event.h"
#include "third_party/blink/renderer/core/dom/events/event_dispatch_forbidden_scope.h"
#include "third_party/blink/renderer/core/dom/events/event_dispatcher.h"
#include "third_party/blink/renderer/core/dom/node_lists_node_data.h"
#include "third_party/blink/renderer/core/dom/node_traversal.h"
#include "third_party/blink/renderer/core/dom/text.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/html_element_type_helpers.h"
#include "third_party/blink/renderer/platform/bindings/script_forbidden_scope.h"
#include "third_party/blink/renderer/platform/wtf/text/string_builder.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "third_party/blink/renderer/core/dom/shadow_root.h"
#   include "third_party/blink/renderer/core/css/style_engine.h"
#endif

using namespace BlinKit;

namespace blink {

using EventTargetDataMap = std::unordered_map<Node *, std::unique_ptr<EventTargetData>>;

static EventTargetDataMap& GetEventTargetDataMap(void)
{
    static EventTargetDataMap s_eventTargetDataMap;
    return s_eventTargetDataMap;
}

Node::Node(TreeScope *treeScope, ConstructionType type)
    : m_nodeFlags(type)
    , m_treeScope(treeScope)
{
    ASSERT(nullptr != treeScope || kCreateDocument == type  || kCreateShadowRoot == type || kCreateCrawlerDocument == type);
}

Node::~Node(void)
{
    if (HasEventTargetData())
        GetEventTargetDataMap().erase(this);

    if (HasRareData())
        ClearRareData();
#ifndef BLINKIT_CRAWLER_ONLY
    else if (!HasRareData() && !m_data.m_nodeLayoutData->IsSharedEmptyData())
        delete m_data.m_nodeLayoutData;
#endif
}

void Node::AddedEventListener(const AtomicString& eventType, RegisteredEventListener &registeredListener)
{
    EventTarget::AddedEventListener(eventType, registeredListener);
    GetDocument().AddListenerTypeIfNeeded(eventType, *this);
#if 0 // BKTODO: Check if necessary.
    if (LocalFrame *frame = GetDocument().GetFrame())
        frame->GetEventHandlerRegistry().DidAddEventHandler(*this, eventType, registeredListener.Options());
#endif
}

Node* Node::appendChild(Node *newChild, ExceptionState &exceptionState)
{
    if (IsContainerNode())
        return ToContainerNode(this)->AppendChild(newChild, exceptionState);

    exceptionState.ThrowDOMException(DOMExceptionCode::kHierarchyRequestError,
        "This node type does not support this method.");
    return nullptr;
}

#ifndef BLINKIT_CRAWLER_ONLY
void Node::AttachLayoutTree(AttachContext &context)
{
    ASSERT(GetDocument().InStyleRecalc() || IsDocumentNode());
    ASSERT(!GetDocument().Lifecycle().InDetach());
    ASSERT(NeedsAttach());

    LayoutObject *layoutObject = GetLayoutObject();
    ASSERT(nullptr == layoutObject
        || (nullptr != layoutObject->Style() && (nullptr != layoutObject->Parent() || layoutObject->IsLayoutView())));

    ClearNeedsStyleRecalc();
    ClearNeedsReattachLayoutTree();

#if 0 // BKTODO: Check if necessary
    if (AXObjectCache* cache = GetDocument().ExistingAXObjectCache())
        cache->UpdateCacheAfterNodeIsAttached(this);
#endif
}

bool Node::CanParticipateInFlatTree(void) const
{
    ASSERT(false); // BKTODO:
    return false;
}

bool Node::CanStartSelection(void) const
{
    ASSERT(false); // BKTODO: Check child classes.
    return false;
}

void Node::CheckSlotChange(SlotChangeType slotChangeType)
{
    ASSERT(false); // BKTODO:
}

void Node::ClearNeedsStyleRecalc(void)
{
    m_nodeFlags &= ~kStyleChangeMask;

    if (IsElementNode() && HasRareData())
        ASSERT(false); // BKTODO: ToElement(*this).SetAnimationStyleChange(false);
}
#endif

void Node::ClearRareData(void)
{
    if (IsElementNode())
        delete static_cast<ElementRareData *>(m_data.m_rareData);
    else
        delete static_cast<NodeRareData *>(m_data.m_rareData);
    m_data.m_rareData = nullptr;
    ClearFlag(kHasRareDataFlag);
}

Node* Node::cloneNode(bool deep, ExceptionState &exceptionState) const
{
    // https://dom.spec.whatwg.org/#dom-node-clonenode

    // 1. If context object is a shadow root, then throw a
    // "NotSupportedError" DOMException.
    if (IsShadowRoot())
    {
        exceptionState.ThrowDOMException(DOMExceptionCode::kNotSupportedError,
            "ShadowRoot nodes are not clonable.");
        return nullptr;
    }

    Document &document = GetDocument();

    // 2. Return a clone of the context object, with the clone children
    // flag set if deep is true.
    return Clone(document, deep ? CloneChildrenFlag::kClone : CloneChildrenFlag::kSkip);
}

NodeList* Node::childNodes(void)
{
    if (IsContainerNode())
        return EnsureRareData().EnsureNodeLists().EnsureChildNodeList(ToContainerNode(*this));
    return EnsureRareData().EnsureNodeLists().EnsureEmptyChildNodeList(*this);
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

ShadowRoot* Node::ContainingShadowRoot(void) const
{
#ifdef BLINKIT_CRAWLER_ONLY
    ASSERT(!GetTreeScope().RootNode().IsShadowRoot());
    return nullptr;
#else
    Node& root = GetTreeScope().RootNode();
    return root.IsShadowRoot() ? ToShadowRoot(&root) : nullptr;
#endif
}

bool Node::contains(const Node *node) const
{
    if (nullptr == node)
        return false;
    return this == node || node->IsDescendantOf(this);
}

bool Node::ContainsIncludingHostElements(const Node &node) const
{
    const Node *current = &node;
    do {
        if (current == this)
            return true;
        if (current->IsDocumentFragment() && ToDocumentFragment(current)->IsTemplateContent())
            ASSERT(false); // BKTODO: current = static_cast<const TemplateContentDocumentFragment *>(current)->Host();
        else
            current = current->ParentOrShadowHostNode();
    } while (nullptr != current);
    return false;
}

unsigned Node::CountChildren(void) const
{
    if (!IsContainerNode())
        return 0;
    return ToContainerNode(this)->CountChildren();
}

NodeRareData& Node::CreateRareData(void)
{
#ifdef BLINKIT_CRAWLER_ONLY
    if (IsElementNode())
        m_data.m_rareData = ElementRareData::Create();
    else
        m_data.m_rareData = NodeRareData::Create();
#else
    if (IsElementNode())
        m_data.m_rareData = ElementRareData::Create(m_data.m_nodeLayoutData);
    else
        m_data.m_rareData = NodeRareData::Create(m_data.m_nodeLayoutData);
#endif

    ASSERT(m_data.m_rareData);
    SetFlag(kHasRareDataFlag);
    return *RareData();
}

void Node::DefaultEventHandler(Event &event)
{
    if (event.target() != this)
        return;
#ifndef BLINKIT_CRAWLER_ONLY
    const AtomicString &eventType = event.type();
    BKLOG("// BKTODO: Process events.");
#if 0
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
#endif
}

#ifndef BLINKIT_CRAWLER_ONLY
void Node::DetachLayoutTree(const AttachContext &context)
{
    ASSERT(false); // BKTODO: Check child classes.
}
#endif

void Node::DidNotifySubtreeInsertionsToDocument(void)
{
    ASSERT(false); // BKTODO:
}

DispatchEventResult Node::DispatchEventInternal(Event &event)
{
    return EventDispatcher::DispatchEvent(*this, event);
}

void Node::DispatchSubtreeModifiedEvent(void)
{
    if (IsInShadowTree())
        return;

#if DCHECK_IS_ON()
    DCHECK(!EventDispatchForbiddenScope::IsEventDispatchForbidden());
#endif

    if (!GetDocument().HasListenerType(Document::kDOMSubtreeModifiedListener))
        return;

    ASSERT(false); // BKTODO:
#if 0
    DispatchScopedEvent(*MutationEvent::Create(EventTypeNames::DOMSubtreeModified, Event::Bubbles::kYes));
#endif
}

EventTargetData& Node::EnsureEventTargetData(void)
{
    EventTargetDataMap &targetMap = GetEventTargetDataMap();
    if (!HasEventTargetData())
    {
        ASSERT(std::end(targetMap) == targetMap.find(this));
        SetHasEventTargetData(true);
        targetMap[this] = std::make_unique<EventTargetData>();
    }
    return *targetMap.at(this);
}

Node* Node::firstChild(void) const
{
    if (!IsContainerNode())
        return nullptr;
    return ToContainerNode(this)->FirstChild();
}

EventTargetData* Node::GetEventTargetData(void)
{
    return HasEventTargetData() ? GetEventTargetDataMap()[this].get() : nullptr;
}

ExecutionContext* Node::GetExecutionContext(void) const
{
    return GetDocument().ContextDocument();
}

#ifndef BLINKIT_CRAWLER_ONLY
ContainerNode* Node::GetReattachParent(void) const
{
    ASSERT(false); // BKTODO:
    return nullptr;
}
#endif

#ifdef BLINKIT_CRAWLER_ONLY
ShadowRoot* Node::GetShadowRoot(void) const
{
    return nullptr;
}
#endif

void Node::HandleLocalEvents(Event &event)
{
    if (!HasEventTargetData())
        return;

    FireEventListeners(event);
}

bool Node::HasTagName(const HTMLQualifiedName &name) const
{
    return IsHTMLElement() && ToElement(*this).HasTagName(name);
}

bool Node::InActiveDocument(void) const
{
    return isConnected() && GetDocument().IsActive();
}

Node* Node::insertBefore(Node *newChild, Node *refChild, ExceptionState &exceptionState)
{
    if (IsContainerNode())
        return ToContainerNode(this)->InsertBefore(newChild, refChild, exceptionState);

    exceptionState.ThrowDOMException(DOMExceptionCode::kHierarchyRequestError,
        "This node type does not support this method.");
    return nullptr;
}

Node::InsertionNotificationRequest Node::InsertedInto(ContainerNode &insertionPoint)
{
#ifndef BLINKIT_CRAWLER_ONLY
    ASSERT(!ChildNeedsStyleInvalidation());
    ASSERT(!NeedsStyleInvalidation());
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
    if (ChildNeedsDistributionRecalc() && !insertionPoint.ChildNeedsDistributionRecalc())
        insertionPoint.MarkAncestorsWithChildNeedsDistributionRecalc();
#endif
    return kInsertionDone;
}

#ifndef BLINKIT_CRAWLER_ONLY
bool Node::IsChildOfV1ShadowHost(void) const
{
    if (ShadowRoot *parentShadowRoot = ParentElementShadowRoot())
        return parentShadowRoot->IsV1();
    return false;
}
#endif

bool Node::IsDescendantOf(const Node *other) const
{
    // Return true if other is an ancestor of this, otherwise false
    if (nullptr == other || !other->hasChildren() || isConnected() != other->isConnected())
        return false;
    if (other->GetTreeScope() != GetTreeScope())
        return false;
    if (other->IsTreeScope())
        return !IsTreeScope();
    for (const ContainerNode *n = parentNode(); nullptr != n; n = n->parentNode())
    {
        if (n == other)
            return true;
    }
    return false;
}

bool Node::IsDocumentNode(void) const
{
    return this == GetDocument();
}

bool Node::IsShadowIncludingInclusiveAncestorOf(const Node *node) const
{
    if (nullptr == node)
        return false;

    if (this == node)
        return true;

    if (GetDocument() != node->GetDocument())
        return false;

    if (isConnected() != node->isConnected())
        return false;

    bool has_children = IsContainerNode() && ToContainerNode(this)->HasChildren();
    bool has_shadow = IsShadowHost(this);
    if (!has_children && !has_shadow)
        return false;

    for (; node; node = node->OwnerShadowHost()) {
        if (GetTreeScope() == node->GetTreeScope())
            return contains(node);
    }

    return false;
}

bool Node::IsTreeScope(void) const
{
    return &GetTreeScope().RootNode() == this;
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
    return ToContainerNode(this)->LastChild();
}

#ifndef BLINKIT_CRAWLER_ONLY
void Node::MarkAncestorsWithChildNeedsDistributionRecalc(void)
{
    ASSERT(false); // BKTODO:
}

void Node::MarkAncestorsWithChildNeedsStyleInvalidation(void)
{
    ASSERT(false); // BKTODO:
}

void Node::MarkAncestorsWithChildNeedsStyleRecalc(void)
{
    ContainerNode *ancestor = ParentOrShadowHostNode();
    bool parentDirty = nullptr != ancestor && ancestor->NeedsStyleRecalc();
    while (nullptr != ancestor && !ancestor->ChildNeedsStyleRecalc())
    {
        ancestor->SetChildNeedsStyleRecalc();
        if (ancestor->NeedsStyleRecalc())
            break;
        ancestor = ancestor->ParentOrShadowHostNode();
    }

    if (!isConnected())
        return;

    // If the parent node is already dirty, we can keep the same recalc root. The
    // early return here is a performance optimization.
    if (parentDirty)
        return;

    Document &document = GetDocument();
    document.GetStyleEngine().UpdateStyleRecalcRoot(ancestor, this);
    document.ScheduleLayoutTreeUpdateIfNeeded();
}
#endif

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

#ifndef BLINKIT_CRAWLER_ONLY
bool Node::NeedsDistributionRecalc(void) const
{
    ASSERT(false); // BKTODO:
    return false;
}
#endif

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

NodeListsNodeData* Node::NodeLists(void)
{
    return HasRareData() ? RareData()->NodeLists() : nullptr;
}

String Node::nodeValue(void) const
{
    return String();
}

void Node::NotifyMutationObserversNodeWillDetach(void)
{
    if (!GetDocument().HasMutationObservers())
        return;

    ASSERT(false); // BKTODO:
#if 0
    ScriptForbiddenScope forbid_script_during_raw_iteration;
    for (Node* node = parentNode(); node; node = node->parentNode()) {
        if (const HeapVector<TraceWrapperMember<MutationObserverRegistration>>*
            registry = node->MutationObserverRegistry()) {
            for (const auto& registration : *registry)
                registration->ObservedSubtreeNodeWillDetach(*this);
        }

        if (const HeapHashSet<TraceWrapperMember<MutationObserverRegistration>>*
            transient_registry = node->TransientMutationObserverRegistry()) {
            for (auto& registration : *transient_registry)
                registration->ObservedSubtreeNodeWillDetach(*this);
        }
    }
#endif
}

Document* Node::ownerDocument(void) const
{
    Document *doc = &GetDocument();
    return doc == this ? nullptr : doc;
}

Element* Node::OwnerShadowHost(void) const
{
#ifndef BLINKIT_CRAWLER_ONLY
    if (ShadowRoot *root = ContainingShadowRoot())
        return &root->host();
#endif
    return nullptr;
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

ContainerNode* Node::ParentElementOrDocumentFragment(void) const
{
    if (ContainerNode *parent = parentNode())
    {
        if (parent->IsElementNode() || parent->IsDocumentFragment())
            return parent;
    }
    return nullptr;
}

ContainerNode* Node::ParentElementOrShadowRoot(void) const
{
    if (ContainerNode *parent = parentNode())
    {
        if (parent->IsElementNode() || parent->IsShadowRoot())
            return parent;
    }
    return nullptr;
}

#ifndef BLINKIT_CRAWLER_ONLY
ShadowRoot* Node::ParentElementShadowRoot(void) const
{
    if (Element* parent = parentElement())
        return parent->GetShadowRoot();
    return nullptr;
}
#endif

Element* Node::ParentOrShadowHostElement(void) const
{
    ContainerNode *parent = ParentOrShadowHostNode();
    if (nullptr == parent)
        return nullptr;

#ifdef BLINKIT_CRAWLER_ONLY
    ASSERT(!parent->IsShadowRoot());
#else
    if (parent->IsShadowRoot())
        return &ToShadowRoot(parent)->host();
#endif

    if (!parent->IsElementNode())
        return nullptr;

    return ToElement(parent);
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

void Node::remove(NodeVector &detached, ExceptionState &exceptionState)
{
    if (ContainerNode *parent = parentNode())
        parent->RemoveChild(this, detached, exceptionState);
}

void Node::RemoveAllEventListenersRecursively(void)
{
    ScriptForbiddenScope forbidScriptDuringRawIteration;
    for (Node &node : NodeTraversal::StartsAt(*this))
    {
        node.RemoveAllEventListeners();
#ifndef BLINKIT_CRAWLER_ONLY
        if (ShadowRoot *root = node.GetShadowRoot())
            root->RemoveAllEventListenersRecursively();
#endif
    }
}

Node* Node::removeChild(Node *child, NodeVector &detachedChildren, ExceptionState &exceptionState)
{
    if (IsContainerNode())
        return ToContainerNode(this)->RemoveChild(child, detachedChildren, exceptionState);

    exceptionState.ThrowDOMException(DOMExceptionCode::kNotFoundError,
        "This node type does not support this method.");
    return nullptr;
}

void Node::RemovedFrom(ContainerNode &insertionPoint)
{
    ASSERT(insertionPoint.isConnected() || IsContainerNode() || IsInShadowTree());
    if (insertionPoint.isConnected())
    {
        ClearFlag(kIsConnectedFlag);
        insertionPoint.GetDocument().DecrementNodeCount();
    }
    if (IsInShadowTree() && !ContainingTreeScope().RootNode().IsShadowRoot())
        ClearFlag(kIsInShadowTreeFlag);
}

#ifndef BLINKIT_CRAWLER_ONLY
void Node::SetCustomElementState(CustomElementState newState)
{
    CustomElementState oldState = GetCustomElementState();

    switch (newState)
    {
        case CustomElementState::kUncustomized:
            NOTREACHED();  // Everything starts in this state
            return;
        case CustomElementState::kUndefined:
            ASSERT(CustomElementState::kUncustomized == oldState);
            break;
        case CustomElementState::kCustom:
            ASSERT(CustomElementState::kUndefined == oldState);
            break;
        case CustomElementState::kFailed:
            ASSERT(CustomElementState::kFailed != oldState);
            break;
    }

    ASSERT(IsHTMLElement() && !ForCrawler());
    ASSERT(false); // BKTODO:
#if 0
    DCHECK_NE(kV0Upgraded, GetV0CustomElementState());

    Element* element = ToElement(this);
    bool was_defined = element->IsDefined();

    node_flags_ = (node_flags_ & ~kCustomElementStateMask) |
        static_cast<NodeFlags>(new_state);
    DCHECK(new_state == GetCustomElementState());

    if (element->IsDefined() != was_defined) {
        element->PseudoStateChanged(CSSSelector::kPseudoDefined);
        element->PseudoStateChanged(CSSSelector::kPseudoUnresolved);
    }
#endif
}

void Node::SetLayoutObject(LayoutObject *layoutObject)
{
    NodeRenderingData *nodeLayoutData = HasRareData()
        ? m_data.m_rareData->GetNodeRenderingData()
        : m_data.m_nodeLayoutData;

    // Already pointing to a non empty NodeRenderingData so just set the pointer
    // to the new LayoutObject.
    if (!nodeLayoutData->IsSharedEmptyData())
    {
        nodeLayoutData->SetLayoutObject(layoutObject);
        return;
    }

    if (nullptr == layoutObject)
        return;

    // Swap the NodeRenderingData to point to a new NodeRenderingData instead of
    // the static SharedEmptyData instance.
    ASSERT(nullptr == nodeLayoutData->GetNonAttachedStyle());
    nodeLayoutData = new NodeRenderingData(layoutObject);
    if (HasRareData())
        m_data.m_rareData->SetNodeRenderingData(nodeLayoutData);
    else
        m_data.m_nodeLayoutData = nodeLayoutData;
}

void Node::SetNeedsStyleInvalidation(void)
{
    ASSERT(IsContainerNode());
    SetFlag(kNeedsStyleInvalidationFlag);
    MarkAncestorsWithChildNeedsStyleInvalidation();
}

void Node::SetNeedsStyleRecalc(StyleChangeType changeType, const StyleChangeReasonForTracing &)
{
    ASSERT(false); // BKTODO:
}
#endif

void Node::setNodeValue(const String &nodeValue)
{
    // By default, setting nodeValue has no effect.
}

void Node::SetParentOrShadowHostNode(ContainerNode *parent)
{
    ASSERT(IsMainThread());
    m_parentOrShadowHostNode = parent;
}

void Node::setTextContent(const String &text, NodeVector &detachedChildren)
{
    switch (getNodeType())
    {
        case kAttributeNode:
        case kTextNode:
        case kCdataSectionNode:
        case kCommentNode:
        case kProcessingInstructionNode:
            setNodeValue(text);
            return;
        case kElementNode:
        case kDocumentFragmentNode:
        {
            // FIXME: Merge this logic into replaceChildrenWithText.
            ContainerNode *container = ToContainerNode(this);

            // Note: This is an intentional optimization.
            // See crbug.com/352836 also.
            // No need to do anything if the text is identical.
            if (container->HasOneTextChild() && ToText(container->firstChild())->data() == text
                && !text.IsEmpty())
            {
                return;
            }

            ChildListMutationScope mutation(*this);
            // Note: This API will not insert empty text nodes:
            // https://dom.spec.whatwg.org/#dom-node-textcontent
            if (text.IsEmpty())
            {
                container->RemoveChildren(detachedChildren, kDispatchSubtreeModifiedEvent);
            }
            else
            {
                ASSERT(false); // BKTODO:
#if 0
                container->RemoveChildren(kOmitSubtreeModifiedEvent);
                container->AppendChild(GetDocument().createTextNode(text), ASSERT_NO_EXCEPTION);
#endif
        }
            return;
        }
        case kDocumentNode:
        case kDocumentTypeNode:
            // Do nothing.
            return;
    }
    NOTREACHED();
}

String Node::textContent(bool convertBrsToNewlines) const
{
    // This covers ProcessingInstruction and Comment that should return their
    // value when .textContent is accessed on them, but should be ignored when
    // iterated over as a descendant of a ContainerNode.
    if (IsCharacterDataNode())
        return ToCharacterData(this)->data();

    // Attribute nodes have their attribute values as textContent.
    if (IsAttributeNode())
        return ToAttr(this)->value();

    // Documents and non-container nodes (that are not CharacterData)
    // have null textContent.
    if (IsDocumentNode() || !IsContainerNode())
        return String();

    StringBuilder content;
    for (const Node &node : NodeTraversal::InclusiveDescendantsOf(*this))
    {
        if (IsHTMLBRElement(node) && convertBrsToNewlines)
            content.Append('\n');
        else if (node.IsTextNode())
            content.Append(ToText(node).data());
    }
    return content.ToString();
}

Node& Node::TreeRoot(void) const
{
    if (IsInTreeScope())
        return ContainingTreeScope().RootNode();
    const Node *node = this;
    while (nullptr != node->parentNode())
        node = node->parentNode();
    return const_cast<Node &>(*node);
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

#ifndef BLINKIT_CRAWLER_ONLY
const ComputedStyle* Node::VirtualEnsureComputedStyle(PseudoId pseudoElementSpecifier)
{
    ASSERT(false); // BKTODO: Check child classes.
    return nullptr;
}
#endif

void Node::WillCallDefaultEventHandler(const Event &event)
{
#ifndef BLINKIT_CRAWLER_ONLY
    if (ForCrawler() || !event.IsKeyboardEvent())
        return;

    ASSERT(false); // BKTODO:
#if 0
    if (!IsFocused() || GetDocument().LastFocusType() != kWebFocusTypeMouse)
        return;

    if (event.type() != EventTypeNames::keydown ||
        GetDocument().HadKeyboardEvent())
        return;

    GetDocument().SetHadKeyboardEvent(true);
#endif

    // Changes to HadKeyboardEvent may affect :focus-visible matching,
    // ShouldHaveFocusAppearance and LayoutTheme::IsFocused().
    // Inform LayoutTheme if HadKeyboardEvent changes.
    if (LayoutObject *layoutObject = GetLayoutObject())
    {
        layoutObject->InvalidateIfControlStateChanged(kFocusControlState);

        if (RuntimeEnabledFeatures::CSSFocusVisibleEnabled() && IsContainerNode())
            ASSERT(false); // BKTODO: ToContainerNode(*this).FocusVisibleStateChanged();
    }
#endif
}

}  // namespace blink
