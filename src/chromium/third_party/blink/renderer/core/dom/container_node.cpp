// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: container_node.cpp
// Description: ContainerNode Class
//      Author: Ziming Li
//     Created: 2019-09-16
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2013 Apple Inc. All rights
 * reserved.
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

#include "container_node.h"

#include <stack>
#include "third_party/blink/renderer/core/css/selector_query.h"
#include "third_party/blink/renderer/core/dom/child_list_mutation_scope.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/dom/document_fragment.h"
#include "third_party/blink/renderer/core/dom/element_traversal.h"
#include "third_party/blink/renderer/core/dom/events/event_dispatch_forbidden_scope.h"
#include "third_party/blink/renderer/core/dom/node_lists_node_data.h"
#include "third_party/blink/renderer/core/dom/node_traversal.h"
#include "third_party/blink/renderer/core/dom/static_node_list.h"
#include "third_party/blink/renderer/core/dom/tree_ordered_map.h"
#include "third_party/blink/renderer/core/html_element_type_helpers.h"
#include "third_party/blink/renderer/core/html/html_tag_collection.h"
#include "third_party/blink/renderer/platform/bindings/exception_state.h"
#include "third_party/blink/renderer/platform/bindings/script_forbidden_scope.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "third_party/blink/renderer/core/css/style_change_reason.h"
#   include "third_party/blink/renderer/core/css/style_engine.h"
#   include "third_party/blink/renderer/core/dom/flat_tree_traversal.h"
#   include "third_party/blink/renderer/core/dom/node_computed_style.h"
#   include "third_party/blink/renderer/core/dom/shadow_root.h"
#   include "third_party/blink/renderer/core/dom/slot_assignment_recalc_forbidden_scope.h"
#   include "third_party/blink/renderer/core/dom/text.h"
#   include "third_party/blink/renderer/core/html/html_slot_element.h"
#endif

using namespace BlinKit;

namespace blink {

namespace {

// This class is helpful to detect necessity of
// RecheckNodeInsertionStructuralPrereq() after removeChild*() inside
// InsertBefore(), AppendChild(), and ReplaceChild().
//
// After removeChild*(), we can detect necessity of
// RecheckNodeInsertionStructuralPrereq() by
//  - DOM tree version of |node_document_| was increased by at most one.
//  - If |node| and |parent| are in different documents, Document for
//    |parent| must not be changed.
class DOMTreeMutationDetector
{
    STACK_ALLOCATED();

public:
    DOMTreeMutationDetector(const Node &node, const Node &parent)
        : m_node(&node)
        , m_nodeDocument(node.GetDocument())
        , m_parentDocument(parent.GetDocument())
        , m_parent(parent)
        , m_originalNodeDocumentVersion(m_nodeDocument->DomTreeVersion())
        , m_originalParentDocumentVersion(m_parentDocument->DomTreeVersion())
    {
    }

    bool NeedsRecheck(void)
    {
        if (m_nodeDocument != m_node->GetDocument())
            return false;
        if (m_nodeDocument->DomTreeVersion() > m_originalNodeDocumentVersion + 1)
            return false;
        if (m_parentDocument != m_parent->GetDocument())
            return false;
        if (m_nodeDocument == m_parentDocument)
            return true;
        return m_parentDocument->DomTreeVersion() == m_originalParentDocumentVersion;
    }

private:
    const Member<const Node> m_node;
    const Member<Document> m_nodeDocument;
    const Member<Document> m_parentDocument;
    const Member<const Node> m_parent;
    const uint64_t m_originalNodeDocumentVersion;
    const uint64_t m_originalParentDocumentVersion;
};

} // namespace

class ContainerNode::AdoptAndAppendChild
{
public:
    inline void operator()(ContainerNode &container, Node &child, Node *) const
    {
        container.GetTreeScope().AdoptIfNeeded(child);
        container.AppendChildCommon(child);
    }
};

class ContainerNode::AdoptAndInsertBefore
{
public:
    inline void operator()(ContainerNode &container, Node &child, Node *next) const
    {
        ASSERT(nullptr != next);
        ASSERT(next->parentNode() == &container);
        container.GetTreeScope().AdoptIfNeeded(child);
        container.InsertBeforeCommon(*next, child);
    }
};

static inline bool CheckReferenceChildParent(const Node &parent, const Node *next, const Node *oldChild, ExceptionState &exceptionState)
{
    if (nullptr != next && next->parentNode() != &parent)
    {
        exceptionState.ThrowDOMException(DOMExceptionCode::kNotFoundError,
            "The node before which the new node is to be inserted is not a child of this node.");
        return false;
    }
    if (nullptr != oldChild && oldChild->parentNode() != &parent)
    {
        exceptionState.ThrowDOMException(DOMExceptionCode::kNotFoundError,
            "The node to be replaced is not a child of this node.");
        return false;
    }
    return true;
}

// This dispatches various events; DOM mutation events, blur events, IFRAME
// unload events, etc.
// Returns true if DOM mutation should be proceeded.
static inline bool CollectChildrenAndRemoveFromOldParent(Node &node, NodeVector &nodes, ExceptionState &exceptionState)
{
    if (node.IsDocumentFragment())
    {
        DocumentFragment &fragment = ToDocumentFragment(node);
        GetChildNodes(fragment, nodes);
        fragment.RemoveChildren();
        return !nodes.empty();
    }
    nodes.push_back(&node);
    if (ContainerNode *oldParent = node.parentNode())
        oldParent->removeChild(&node, exceptionState);
    return !exceptionState.HadException() && !nodes.empty();
}

static void DispatchChildRemovalEvents(Node &child)
{
    if (child.IsInShadowTree())
        return;

#if DCHECK_IS_ON()
    DCHECK(!EventDispatchForbiddenScope::IsEventDispatchForbidden());
#endif

    Node *c = &child;
    Document &document = child.GetDocument();

    // Dispatch pre-removal mutation events.
    if (nullptr != c->parentNode() && document.HasListenerType(Document::kDOMNodeRemovedListener))
    {
        ASSERT(false); // BKTODO:
#if 0
        bool original_node_flag = c->InDOMNodeRemovedHandler();
        auto original_document_state = document.GetInDOMNodeRemovedHandlerState();
        if (ScopedEventQueue::Instance()->ShouldQueueEvents()) {
            UseCounter::Count(document, WebFeature::kDOMNodeRemovedEventDelayed);
        }
        else {
            c->SetInDOMNodeRemovedHandler(true);
            document.SetInDOMNodeRemovedHandlerState(
                Document::InDOMNodeRemovedHandlerState::kDOMNodeRemoved);
        }
        NodeChildRemovalTracker scope(child);
        c->DispatchScopedEvent(*MutationEvent::Create(
            EventTypeNames::DOMNodeRemoved, Event::Bubbles::kYes, c->parentNode()));
        document.SetInDOMNodeRemovedHandlerState(original_document_state);
        c->SetInDOMNodeRemovedHandler(original_node_flag);
#endif
    }

    // Dispatch the DOMNodeRemovedFromDocument event to all descendants.
    if (c->isConnected() && document.HasListenerType(Document::kDOMNodeRemovedFromDocumentListener))
    {
        ASSERT(false); // BKTODO:
#if 0
        bool original_node_flag = c->InDOMNodeRemovedHandler();
        auto original_document_state = document.GetInDOMNodeRemovedHandlerState();
        if (ScopedEventQueue::Instance()->ShouldQueueEvents()) {
            UseCounter::Count(document,
                WebFeature::kDOMNodeRemovedFromDocumentEventDelayed);
        }
        else {
            c->SetInDOMNodeRemovedHandler(true);
            document.SetInDOMNodeRemovedHandlerState(
                Document::InDOMNodeRemovedHandlerState::kDOMNodeRemovedFromDocument);
        }
        NodeChildRemovalTracker scope(child);
        for (; c; c = NodeTraversal::Next(*c, &child)) {
            c->DispatchScopedEvent(*MutationEvent::Create(
                EventTypeNames::DOMNodeRemovedFromDocument, Event::Bubbles::kNo));
        }
        document.SetInDOMNodeRemovedHandlerState(original_document_state);
        child.SetInDOMNodeRemovedHandler(original_node_flag);
#endif
    }
}

ContainerNode::ContainerNode(TreeScope *treeScope, ConstructionType type) : Node(treeScope, type)
{
}

Node* ContainerNode::AppendChild(Node *newChild, ExceptionState &exceptionState)
{
    ASSERT(nullptr != newChild);
    // Make sure adding the new child is ok
    if (!EnsurePreInsertionValidity(*newChild, nullptr, nullptr, exceptionState))
        return newChild;

    NodeVector targets;
    DOMTreeMutationDetector detector(*newChild, *this);
    if (!CollectChildrenAndRemoveFromOldParent(*newChild, targets, exceptionState))
        return newChild;
    if (!detector.NeedsRecheck())
    {
        if (!RecheckNodeInsertionStructuralPrereq(targets, nullptr, exceptionState))
            return newChild;
    }

    NodeVector postInsertionNotificationTargets;
    {
#ifndef BLINKIT_CRAWLER_ONLY
        SlotAssignmentRecalcForbiddenScope forbidSlotRecalc(GetDocument());
#endif
        ChildListMutationScope mutation(*this);
        InsertNodeVector(targets, nullptr, AdoptAndAppendChild(), &postInsertionNotificationTargets);
    }
    DidInsertNodeVector(targets, nullptr, postInsertionNotificationTargets);
    return newChild;
}

Node* ContainerNode::AppendChild(Node *newChild)
{
    return AppendChild(newChild, ASSERT_NO_EXCEPTION);
}

void ContainerNode::AppendChildCommon(Node &child)
{
#if DCHECK_IS_ON()
    ASSERT(EventDispatchForbiddenScope::IsEventDispatchForbidden());
#endif
    ASSERT(ScriptForbiddenScope::IsScriptForbidden());

    child.SetParentOrShadowHostNode(this);
    if (m_lastChild)
    {
        child.SetPreviousSibling(m_lastChild);
        m_lastChild->SetNextSibling(&child);
    }
    else
    {
        SetFirstChild(&child);
    }
    SetLastChild(&child);
}

#ifndef BLINKIT_CRAWLER_ONLY

#if DCHECK_IS_ON()
namespace {

bool AttachedAllowedWhenAttaching(Node *node)
{
    return node->getNodeType() == Node::kCommentNode || node->getNodeType() == Node::kProcessingInstructionNode;
}

bool ChildAttachedAllowedWhenAttachingChildren(ContainerNode *node)
{
    if (node->IsShadowRoot())
        return true;
    if (node->IsV0InsertionPoint())
        return true;
    if (IsHTMLSlotElement(*node))
        return true;
    if (IsShadowHost(node))
        return true;
    return false;
}

}  // namespace
#endif // DCHECK_IS_ON

void ContainerNode::AttachLayoutTree(AttachContext &context)
{
    for (Node *child = firstChild(); nullptr != child; child = child->nextSibling())
    {
#if DCHECK_IS_ON()
        DCHECK(child->NeedsAttach() || AttachedAllowedWhenAttaching(child)
            || ChildAttachedAllowedWhenAttachingChildren(this));
#endif
        if (child->NeedsAttach())
            child->AttachLayoutTree(context);
    }

    ClearChildNeedsStyleRecalc();
    ClearChildNeedsReattachLayoutTree();
    Node::AttachLayoutTree(context);
}

void ContainerNode::CheckForSiblingStyleChanges(
    SiblingCheckType changeType,
    Element *changedElement,
    Node *nodeBeforeChange, Node *nodeAfterChange)
{
    if (!InActiveDocument() || GetDocument().HasPendingForcedStyleRecalc()
        || GetStyleChangeType() >= kSubtreeStyleChange)
    {
        return;
    }

    if (!HasRestyleFlag(DynamicRestyleFlags::kChildrenAffectedByStructuralRules))
        return;

    ASSERT(false); // BKTODO:
#if 0
    Element* element_after_change =
        !node_after_change || node_after_change->IsElementNode()
        ? ToElement(node_after_change)
        : ElementTraversal::NextSibling(*node_after_change);
    Element* element_before_change =
        !node_before_change || node_before_change->IsElementNode()
        ? ToElement(node_before_change)
        : ElementTraversal::PreviousSibling(*node_before_change);

    // TODO(futhark@chromium.org): move this code into StyleEngine and collect the
    // various invalidation sets into a single InvalidationLists object and
    // schedule with a single scheduleInvalidationSetsForNode for efficiency.

    // Forward positional selectors include :nth-child, :nth-of-type,
    // :first-of-type, and only-of-type. Backward positional selectors include
    // :nth-last-child, :nth-last-of-type, :last-of-type, and :only-of-type.
    if ((ChildrenAffectedByForwardPositionalRules() && element_after_change) ||
        (ChildrenAffectedByBackwardPositionalRules() && element_before_change)) {
        GetDocument().GetStyleEngine().ScheduleNthPseudoInvalidations(*this);
    }

    if (ChildrenAffectedByFirstChildRules() && !element_before_change &&
        element_after_change &&
        element_after_change->AffectedByFirstChildRules()) {
        DCHECK_NE(change_type, kFinishedParsingChildren);
        element_after_change->PseudoStateChanged(CSSSelector::kPseudoFirstChild);
        element_after_change->PseudoStateChanged(CSSSelector::kPseudoOnlyChild);
    }

    if (ChildrenAffectedByLastChildRules() && !element_after_change &&
        element_before_change &&
        element_before_change->AffectedByLastChildRules()) {
        element_before_change->PseudoStateChanged(CSSSelector::kPseudoLastChild);
        element_before_change->PseudoStateChanged(CSSSelector::kPseudoOnlyChild);
    }

    // For ~ and + combinators, succeeding siblings may need style invalidation
    // after an element is inserted or removed.

    if (!element_after_change)
        return;

    if (!ChildrenAffectedByIndirectAdjacentRules() &&
        !ChildrenAffectedByDirectAdjacentRules())
        return;

    if (change_type == kSiblingElementInserted) {
        GetDocument().GetStyleEngine().ScheduleInvalidationsForInsertedSibling(
            element_before_change, *changed_element);
        return;
    }

    DCHECK(change_type == kSiblingElementRemoved);
    GetDocument().GetStyleEngine().ScheduleInvalidationsForRemovedSibling(
        element_before_change, *changed_element, *element_after_change);
#endif
}
#endif

bool ContainerNode::CheckParserAcceptChild(const Node &newChild) const
{
    if (!IsDocumentNode())
        return true;

    const Document *document = static_cast<const Document *>(this);
    return document->CanAcceptChild(newChild, nullptr, nullptr, IGNORE_EXCEPTION_FOR_TESTING);
}

void ContainerNode::ChildrenChanged(const ChildrenChange &change)
{
    Document &document = GetDocument();
    document.IncDOMTreeVersion();
    document.NotifyChangeChildren(*this);
    InvalidateNodeListCachesInAncestors(nullptr, nullptr, &change);
#ifndef BLINKIT_CRAWLER_ONLY
    if (change.IsChildInsertion())
    {
        if (change.sibling_changed->NeedsStyleRecalc())
            MarkAncestorsWithChildNeedsStyleRecalc(change.sibling_changed);
    }
    else if (change.IsChildRemoval() || change.type == kAllChildrenRemoved)
    {
        GetDocument().GetStyleEngine().ChildrenRemoved(*this);
    }
#endif
}

void ContainerNode::CloneChildNodesFrom(const ContainerNode &node)
{
    for (const Node &child : NodeTraversal::ChildrenOf(node))
        AppendChild(child.Clone(GetDocument(), CloneChildrenFlag::kClone));
}

unsigned ContainerNode::CountChildren(void) const
{
    unsigned count = 0;
    for (Node *node = firstChild(); nullptr != node; node = node->nextSibling())
        ++count;
    return count;
}

#ifndef BLINKIT_CRAWLER_ONLY
void ContainerNode::DetachLayoutTree(const AttachContext &context)
{
    AttachContext childrenContext(context);
    childrenContext.clear_invalidation = true;

    for (Node *child = firstChild(); nullptr != child; child = child->nextSibling())
        child->DetachLayoutTree(childrenContext);

    SetChildNeedsStyleRecalc();
    Node::DetachLayoutTree(context);
}
#endif

static void DispatchChildInsertionEvents(Node &child)
{
    if (child.IsInShadowTree())
        return;

#if DCHECK_IS_ON()
    DCHECK(!EventDispatchForbiddenScope::IsEventDispatchForbidden());
#endif

    Node *c = &child;
    Document *document = &child.GetDocument();

    if (nullptr != c->parentNode() && document->HasListenerType(Document::kDOMNodeInsertedListener))
    {
        ASSERT(false); // BKTODO:
#if 0
        c->DispatchScopedEvent(
            *MutationEvent::Create(EventTypeNames::DOMNodeInserted,
                Event::Bubbles::kYes, c->parentNode()));
#endif
    }

    // dispatch the DOMNodeInsertedIntoDocument event to all descendants
    if (c->isConnected() && document->HasListenerType(Document::kDOMNodeInsertedIntoDocumentListener))
    {
        ASSERT(false); // BKTODO:
#if 0
        for (; c; c = NodeTraversal::Next(*c, &child)) {
            c->DispatchScopedEvent(*MutationEvent::Create(
                EventTypeNames::DOMNodeInsertedIntoDocument, Event::Bubbles::kNo));
        }
#endif
    }
}
void ContainerNode::DidInsertNodeVector(const NodeVector &targets, Node *next, const NodeVector& postInsertionNotificationTargets)
{
    Node *unchangedPrevious = targets.size() > 0 ? targets[0]->previousSibling() : nullptr;
    for (Node *targetNode : targets)
    {
        ChildrenChange change = ChildrenChange::ForInsertion(*targetNode, unchangedPrevious, next,
            kChildrenChangeSourceAPI);
        ChildrenChanged(change);
    }
    for (Node *descendant : postInsertionNotificationTargets)
    {
        if (descendant->isConnected())
            descendant->DidNotifySubtreeInsertionsToDocument();
    }
    for (Node *targetNode : targets)
    {
        if (targetNode->parentNode() == this)
            DispatchChildInsertionEvents(*targetNode);
    }
    DispatchSubtreeModifiedEvent();
}

NodeListsNodeData& ContainerNode::EnsureNodeLists(void)
{
    return EnsureRareData().EnsureNodeLists();
}

bool ContainerNode::EnsurePreInsertionValidity(const Node &newChild, const Node *next, const Node *oldChild, ExceptionState &exceptionState) const
{
    ASSERT(!(nullptr != next && nullptr != oldChild));

    // Use common case fast path if possible.
    if ((newChild.IsElementNode() || newChild.IsTextNode()) && IsElementNode())
    {
        ASSERT(IsChildTypeAllowed(newChild));
        // 2. If node is a host-including inclusive ancestor of parent, throw a
        // HierarchyRequestError.
        if (IsHostIncludingInclusiveAncestorOfThis(newChild, exceptionState))
            return false;
        // 3. If child is not null and its parent is not parent, then throw a
        // NotFoundError.
        return CheckReferenceChildParent(*this, next, oldChild, exceptionState);
    }

    // This should never happen, but also protect release builds from tree
    // corruption.
    ASSERT(!newChild.IsPseudoElement());
    if (newChild.IsPseudoElement())
    {
        exceptionState.ThrowDOMException(DOMExceptionCode::kHierarchyRequestError,
            "The new child element is a pseudo-element.");
        return false;
    }

    if (auto *document = DynamicTo<Document>(this))
    {
        ASSERT(false); // BKTODO:
#if 0
        // Step 2 is unnecessary. No one can have a Document child.
        // Step 3:
        if (!CheckReferenceChildParent(*this, next, old_child, exception_state))
            return false;
        // Step 4-6.
        return document->CanAcceptChild(new_child, next, old_child,
            exception_state);
#endif
    }

    // 2. If node is a host-including inclusive ancestor of parent, throw a
    // HierarchyRequestError.
    if (IsHostIncludingInclusiveAncestorOfThis(newChild, exceptionState))
        return false;

    // 3. If child is not null and its parent is not parent, then throw a
    // NotFoundError.
    if (!CheckReferenceChildParent(*this, next, oldChild, exceptionState))
        return false;

    // 4. If node is not a DocumentFragment, DocumentType, Element, Text,
    // ProcessingInstruction, or Comment node, throw a HierarchyRequestError.
    // 5. If either node is a Text node and parent is a document, or node is a
    // doctype and parent is not a document, throw a HierarchyRequestError.
    if (!IsChildTypeAllowed(newChild))
    {
        exceptionState.ThrowDOMException(DOMExceptionCode::kHierarchyRequestError,
            "Nodes of type '" + newChild.nodeName() +
            "' may not be inserted inside nodes of type '" + nodeName() + "'.");
        return false;
    }

    // Step 6 is unnecessary for non-Document nodes.
    return true;
}

Element* ContainerNode::GetElementById(const AtomicString &id) const
{
    if (IsInTreeScope())
    {
        // Fast path if we are in a tree scope: call getElementById() on tree scope
        // and check if the matching element is in our subtree.
        Element *element = ContainingTreeScope().getElementById(id);
        if (nullptr == element)
            return nullptr;
        if (element->IsDescendantOf(this))
            return element;
    }

    // Fall back to traversing our subtree. In case of duplicate ids, the first
    // element found will be returned.
    for (Element &element : ElementTraversal::DescendantsOf(*this))
    {
        if (element.GetIdAttribute() == id)
            return &element;
    }
    return nullptr;
}

HTMLCollection* ContainerNode::getElementsByTagName(const AtomicString &qualifiedName)
{
    ASSERT(!qualifiedName.IsNull());
    return EnsureCachedCollection<HTMLTagCollection>(kHTMLTagCollectionType, qualifiedName);
}

bool ContainerNode::HasRestyleFlagInternal(DynamicRestyleFlags mask) const
{
    return RareData()->HasRestyleFlag(mask);
}

Node* ContainerNode::InsertBefore(Node *newChild, Node *refChild, ExceptionState &exceptionState)
{
    ASSERT(nullptr != newChild);
    // https://dom.spec.whatwg.org/#concept-node-pre-insert

    // insertBefore(node, null) is equivalent to appendChild(node)
    if (nullptr == refChild)
        return AppendChild(newChild, exceptionState);

    // 1. Ensure pre-insertion validity of node into parent before child.
    if (!EnsurePreInsertionValidity(*newChild, refChild, nullptr, exceptionState))
        return newChild;

    // 2. Let reference child be child.
    // 3. If reference child is node, set it to node¡¯s next sibling.
    if (refChild == newChild)
    {
        refChild = newChild->nextSibling();
        if (nullptr == refChild)
            return AppendChild(newChild, exceptionState);
    }

    // 4. Adopt node into parent¡¯s node document.
    NodeVector targets;
    DOMTreeMutationDetector detector(*newChild, *this);
    if (!CollectChildrenAndRemoveFromOldParent(*newChild, targets, exceptionState))
        return newChild;
    if (!detector.NeedsRecheck())
    {
        if (!RecheckNodeInsertionStructuralPrereq(targets, refChild, exceptionState))
            return newChild;
    }

    // 5. Insert node into parent before reference child.
    NodeVector postInsertionNotificationTargets;
    {
#ifndef BLINKIT_CRAWLER_ONLY
        SlotAssignmentRecalcForbiddenScope forbidSlotRecalc(GetDocument());
#endif
        ChildListMutationScope mutation(*this);
        InsertNodeVector(targets, refChild, AdoptAndInsertBefore(), &postInsertionNotificationTargets);
    }
    DidInsertNodeVector(targets, refChild, postInsertionNotificationTargets);
    return newChild;
}

Node* ContainerNode::InsertBefore(Node *newChild, Node *refChild)
{
    return InsertBefore(newChild, refChild, ASSERT_NO_EXCEPTION);
}

void ContainerNode::InsertBeforeCommon(Node &nextChild, Node &newChild)
{
#if DCHECK_IS_ON()
    ASSERT(EventDispatchForbiddenScope::IsEventDispatchForbidden());
#endif
    ASSERT(ScriptForbiddenScope::IsScriptForbidden());
    // Use insertBefore if you need to handle reparenting (and want DOM mutation
    // events).
    ASSERT(nullptr == newChild.parentNode());
    ASSERT(nullptr == newChild.nextSibling());
    ASSERT(nullptr == newChild.previousSibling());
    ASSERT(!newChild.IsShadowRoot());

    Node *prev = nextChild.previousSibling();
    ASSERT(m_lastChild != prev);
    nextChild.SetPreviousSibling(&newChild);
    if (nullptr != prev)
    {
        ASSERT(firstChild() != nextChild);
        ASSERT(prev->nextSibling() == nextChild);
        prev->SetNextSibling(&newChild);
    }
    else
    {
        ASSERT(firstChild() == nextChild);
        SetFirstChild(&newChild);
    }
    newChild.SetParentOrShadowHostNode(this);
    newChild.SetPreviousSibling(prev);
    newChild.SetNextSibling(&nextChild);
}

template <typename Functor>
void ContainerNode::InsertNodeVector(const NodeVector &targets, Node *next, const Functor &mutator, NodeVector *postInsertionNotificationTargets)
{
    ASSERT(nullptr != postInsertionNotificationTargets);
    EventDispatchForbiddenScope assertNoEventDispatch;
    ScriptForbiddenScope forbidScript;
    for (Node *targetNode : targets)
    {
        ASSERT(nullptr != targetNode);
        ASSERT(nullptr == targetNode->parentNode());
        mutator(*this, *targetNode, next);
        ChildListMutationScope(*this).ChildAdded(*targetNode);
#ifndef BLINKIT_CRAWLER_ONLY
        if (GetDocument().ContainsV1ShadowTree())
            targetNode->CheckSlotChangeAfterInserted();
#endif
        NotifyNodeInsertedInternal(*targetNode, *postInsertionNotificationTargets);
    }
}

void ContainerNode::InvalidateNodeListCachesInAncestors(
    const QualifiedName *attrName,
    Element *attributeOwnerElement,
    const ChildrenChange *change)
{
    // This is a performance optimization, NodeList cache invalidation is
    // not necessary for a text change.
    if (nullptr != change && change->type == kTextChanged)
        return;

    if (HasRareData() && (nullptr == attrName || IsAttributeNode()))
    {
        if (NodeListsNodeData *lists = RareData()->NodeLists())
        {
            if (ChildNodeList *childNodeList = lists->GetChildNodeList(*this))
            {
                if (change)
                    childNodeList->ChildrenChanged(*change);
                else
                    childNodeList->InvalidateCache();
            }
        }
    }

    // Modifications to attributes that are not associated with an Element can't
    // invalidate NodeList caches.
    if (nullptr != attrName && nullptr == attributeOwnerElement)
        return;

    Document &document = GetDocument();
    if (!document.ShouldInvalidateNodeListCaches(attrName))
        return;

    document.InvalidateNodeListCaches(attrName);

    for (ContainerNode *node = this; nullptr != node; node = node->parentNode())
    {
        if (NodeListsNodeData *lists = node->NodeLists())
            lists->InvalidateCaches(attrName);
    }
}

bool ContainerNode::IsChildTypeAllowed(const Node &child) const
{
    if (!child.IsDocumentFragment())
        return ChildTypeAllowed(child.getNodeType());

    for (Node *node = ToDocumentFragment(child).FirstChild(); nullptr != node; node = node->nextSibling())
    {
        if (!ChildTypeAllowed(node->getNodeType()))
            return false;
    }
    return true;
}

bool ContainerNode::IsHostIncludingInclusiveAncestorOfThis(const Node &newChild, ExceptionState &exceptionState) const
{
    // Non-ContainerNode can contain nothing.
    if (!newChild.IsContainerNode())
        return false;

    bool childContainsParent = false;
#ifndef BLINKIT_CRAWLER_ONLY
    if (IsInShadowTree() || GetDocument().IsTemplateDocument())
    {
        childContainsParent = newChild.ContainsIncludingHostElements(*this);
    }
    else
#endif
    {
        const Node &root = TreeRoot();
        if (root.IsDocumentFragment() && ToDocumentFragment(root).IsTemplateContent())
            childContainsParent = newChild.ContainsIncludingHostElements(*this);
        else
            childContainsParent = newChild.contains(this);
    }
    if (childContainsParent)
    {
        exceptionState.ThrowDOMException(DOMExceptionCode::kHierarchyRequestError,
            "The new child element contains the parent.");
    }
    return childContainsParent;
}

void ContainerNode::NotifyNodeInserted(Node &root, ChildrenChangeSource source)
{
#if DCHECK_IS_ON()
    DCHECK(!EventDispatchForbiddenScope::IsEventDispatchForbidden());
#endif
    DCHECK(!root.IsShadowRoot());

#ifndef BLINKIT_CRAWLER_ONLY
    if (GetDocument().ContainsV1ShadowTree())
        root.CheckSlotChangeAfterInserted();
#endif

    NodeVector postInsertionNotificationTargets;
    NotifyNodeInsertedInternal(root, postInsertionNotificationTargets);

    const ChildrenChange change = ChildrenChange::ForInsertion(root, root.previousSibling(), root.nextSibling(), source);
    ChildrenChanged(change);

    for (Node *targetNode : postInsertionNotificationTargets)
    {
        if (targetNode->isConnected())
            targetNode->DidNotifySubtreeInsertionsToDocument();
    }
}

void ContainerNode::NotifyNodeInsertedInternal(Node &root, NodeVector &postInsertionNotificationTargets)
{
    EventDispatchForbiddenScope assertNoEventDispatch;
    ScriptForbiddenScope forbidScript;

    for (Node &node : NodeTraversal::InclusiveDescendantsOf(root))
    {
        // As an optimization we don't notify leaf nodes when when inserting
        // into detached subtrees that are not in a shadow tree.
        if (!isConnected() && !IsInShadowTree() && !node.IsContainerNode())
            continue;
        if (Node::kInsertionShouldCallDidNotifySubtreeInsertions == node.InsertedInto(*this))
            postInsertionNotificationTargets.push_back(&node);
#ifndef BLINKIT_CRAWLER_ONLY
        if (ShadowRoot *shadowRoot = node.GetShadowRoot())
            NotifyNodeInsertedInternal(*shadowRoot, postInsertionNotificationTargets);
#endif
    }
}

void ContainerNode::NotifyNodeRemoved(Node &root)
{
    ScriptForbiddenScope forbidScript;
    EventDispatchForbiddenScope assertNoEventDispatch;

    for (Node &node : NodeTraversal::InclusiveDescendantsOf(root))
    {
        // As an optimization we skip notifying Text nodes and other leaf nodes
        // of removal when they're not in the Document tree and not in a shadow root
        // since the virtual call to removedFrom is not needed.
        if (!node.IsContainerNode() && !node.IsInTreeScope())
            continue;
        node.RemovedFrom(*this);
#ifndef BLINKIT_CRAWLER_ONLY
        if (ShadowRoot* shadow_root = node.GetShadowRoot())
            NotifyNodeRemoved(*shadow_root);
#endif
    }
}

void ContainerNode::ParserAppendChild(Node *newChild)
{
    ASSERT(nullptr != newChild);
    ASSERT(!newChild->IsDocumentFragment());
    ASSERT(!IsHTMLTemplateElement(*this));

    if (!CheckParserAcceptChild(*newChild))
        return;

    // FIXME: parserRemoveChild can run script which could then insert the
    // newChild back into the page. Loop until the child is actually removed.
    // See: fast/parser/execute-script-during-adoption-agency-removal.html
    while (ContainerNode *parent = newChild->parentNode())
        parent->ParserRemoveChild(*newChild);

    ASSERT(GetDocument() == newChild->GetDocument());
    {
        EventDispatchForbiddenScope assertNoEventDispatch;
        ScriptForbiddenScope forbidScript;

        AdoptAndAppendChild()(*this, *newChild, nullptr);
        ChildListMutationScope(*this).ChildAdded(*newChild);
    }

    NotifyNodeInserted(*newChild, kChildrenChangeSourceParser);
}

void ContainerNode::ParserInsertBefore(Node *newChild, Node &refChild)
{
    ASSERT(false); // BKTODO:
}

void ContainerNode::ParserRemoveChild(Node &oldChild)
{
    ASSERT(false); // BKTODO:
}

void ContainerNode::ParserTakeAllChildrenFrom(ContainerNode &oldParent)
{
    while (Node *child = oldParent.FirstChild())
    {
        // Explicitly remove since appending can fail, but this loop shouldn't be
        // infinite.
        oldParent.ParserRemoveChild(*child);
        ParserAppendChild(child);
    }
}

Element* ContainerNode::querySelector(const AtomicString &selectors, ExceptionState &exceptionState)
{
    Document &document = GetDocument();
    SelectorQuery *selectorQuery = document.GetSelectorQueryCache().Add(selectors, document, exceptionState);
    if (nullptr == selectorQuery)
        return nullptr;

    Element *element = selectorQuery->QueryFirst(*this);
    if (nullptr != element && element->GetDocument().InDOMNodeRemovedHandler())
    {
        if (NodeChildRemovalTracker::IsBeingRemoved(element))
            document.CountDetachingNodeAccessInDOMNodeRemovedHandler();
    }
    return element;
}

StaticElementList* ContainerNode::querySelectorAll(const AtomicString &selectors, ExceptionState &exceptionState)
{
    Document &document = GetDocument();
    SelectorQuery *selectorQuery = document.GetSelectorQueryCache().Add(selectors, document, exceptionState);
    if (nullptr == selectorQuery)
        return nullptr;

    return selectorQuery->QueryAll(*this);
}

bool ContainerNode::RecheckNodeInsertionStructuralPrereq(const NodeVector &newChildren, const Node *next, ExceptionState &exceptionState)
{
    ASSERT(false); // BKTODO:
    return false;
}

void ContainerNode::RemoveBetween(Node *previousChild, Node* nextChild, Node &oldChild)
{
    EventDispatchForbiddenScope assertNoEventDispatch;

    ASSERT(oldChild.parentNode() == this);

#ifndef BLINKIT_CRAWLER_ONLY
    if (!oldChild.NeedsAttach())
    {
        AttachContext context;
        context.clear_invalidation = true;
        oldChild.DetachLayoutTree(context);
    }
#endif

    if (nullptr != nextChild)
        nextChild->SetPreviousSibling(previousChild);
    if (nullptr != previousChild)
        previousChild->SetNextSibling(nextChild);
    if (m_firstChild == &oldChild)
        SetFirstChild(nextChild);
    if (m_lastChild == &oldChild)
        SetLastChild(previousChild);

    oldChild.SetPreviousSibling(nullptr);
    oldChild.SetNextSibling(nullptr);
    oldChild.SetParentOrShadowHostNode(nullptr);

    GetDocument().AdoptIfNeeded(oldChild);
}

Node* ContainerNode::RemoveChild(Node *oldChild, ExceptionState &exceptionState)
{
    // NotFoundError: Raised if oldChild is not a child of this node.
    // FIXME: We should never really get PseudoElements in here, but editing will
    // sometimes attempt to remove them still. We should fix that and enable this
    // DCHECK.  DCHECK(!oldChild->isPseudoElement())
    if (nullptr == oldChild || oldChild->parentNode() != this || oldChild->IsPseudoElement())
    {
        exceptionState.ThrowDOMException(DOMExceptionCode::kNotFoundError,
            "The node to be removed is not a child of this node.");
        return nullptr;
    }

    Node *child = oldChild;

#ifndef BLINKIT_CRAWLER_ONLY
    GetDocument().RemoveFocusedElementOfSubtree(child);
#endif

    // Events fired when blurring currently focused node might have moved this
    // child into a different parent.
    if (child->parentNode() != this)
    {
        exceptionState.ThrowDOMException(DOMExceptionCode::kNotFoundError,
            "The node to be removed is no longer a child of this node. Perhaps it was moved in a 'blur' event handler?");
        return nullptr;
    }

    WillRemoveChild(*child);

    // Mutation events might have moved this child into a different parent.
    if (child->parentNode() != this)
    {
        exceptionState.ThrowDOMException(DOMExceptionCode::kNotFoundError,
            "The node to be removed is no longer a child of this node. Perhaps it was moved in response to a mutation?");
        return nullptr;
    }

    {
        TreeOrderedMap::RemoveScope treeRemoveScope;
        Node* prev = child->previousSibling();
        Node* next = child->nextSibling();
        {
#ifndef BLINKIT_CRAWLER_ONLY
            SlotAssignmentRecalcForbiddenScope forbidSlotRecalc(GetDocument());
            StyleEngine::DOMRemovalScope styleScope(GetDocument().GetStyleEngine());
#endif
            RemoveBetween(prev, next, *child);
            NotifyNodeRemoved(*child);
        }
        ChildrenChanged(ChildrenChange::ForRemoval(*child, prev, next, kChildrenChangeSourceAPI));
    }
    DispatchSubtreeModifiedEvent();
    return child;
}

void ContainerNode::RemoveChildren(SubtreeModificationAction action)
{
    if (!m_firstChild)
    {
        ASSERT(!m_lastChild);
        return;
    }

    // Do any prep work needed before actually starting to detach
    // and remove... e.g. stop loading frames, fire unload events.
    WillRemoveChildren();

#ifndef BLINKIT_CRAWLER_ONLY
    // Exclude this node when looking for removed focusedElement since only
    // children will be removed.
    // This must be later than willRemoveChildren, which might change focus
    // state of a child.
    GetDocument().RemoveFocusedElementOfSubtree(this, true);
#endif

    // Removing a node from a selection can cause widget updates.
    GetDocument().NodeChildrenWillBeRemoved(*this);

    {
        TreeOrderedMap::RemoveScope treeRemoveScope;
        {
#ifndef BLINKIT_CRAWLER_ONLY
            SlotAssignmentRecalcForbiddenScope forbidSlotRecalc(GetDocument());
            StyleEngine::DOMRemovalScope styleScope(GetDocument().GetStyleEngine());
#endif
            EventDispatchForbiddenScope assertNoEventDispatch;
            ScriptForbiddenScope forbidScript;

            while (Node *child = m_firstChild)
            {
                RemoveBetween(nullptr, child->nextSibling(), *child);
                NotifyNodeRemoved(*child);
            }
        }

        ChildrenChange change = { kAllChildrenRemoved, nullptr, nullptr, nullptr,
                                 kChildrenChangeSourceAPI };
        ChildrenChanged(change);
    }

    if (kDispatchSubtreeModifiedEvent == action)
        DispatchSubtreeModifiedEvent();
}

Node* ContainerNode::ReplaceChild(Node *newChild, Node *oldChild, ExceptionState &exceptionState)
{
    DCHECK(nullptr != newChild);
    // https://dom.spec.whatwg.org/#concept-node-replace

    if (nullptr == oldChild)
    {
        exceptionState.ThrowDOMException(DOMExceptionCode::kNotFoundError, "The node to be replaced is null.");
        return nullptr;
    }

    // Step 2 to 6.
    if (!EnsurePreInsertionValidity(*newChild, nullptr, oldChild, exceptionState))
        return oldChild;

    // 7. Let reference child be child¡¯s next sibling.
    Node *next = oldChild->nextSibling();
    // 8. If reference child is node, set it to node¡¯s next sibling.
    if (next == newChild)
        next = newChild->nextSibling();

    bool needsRecheck = false;
    // 10. Adopt node into parent¡¯s node document.
    // TODO(tkent): Actually we do only RemoveChild() as a part of 'adopt'
    // operation.
    //
    // Though the following CollectChildrenAndRemoveFromOldParent() also calls
    // RemoveChild(), we'd like to call RemoveChild() here to make a separated
    // MutationRecord.
    if (ContainerNode *newChildParent = newChild->parentNode())
    {
        DOMTreeMutationDetector detector(*newChild, *this);
        newChildParent->RemoveChild(newChild, exceptionState);
        if (exceptionState.HadException())
            return nullptr;
        if (!detector.NeedsRecheck())
            needsRecheck = true;
    }

    NodeVector targets;
    NodeVector postInsertionNotificationTargets;
    {
        // 9. Let previousSibling be child¡¯s previous sibling.
        // 11. Let removedNodes be the empty list.
        // 15. Queue a mutation record of "childList" for target parent with
        // addedNodes nodes, removedNodes removedNodes, nextSibling reference child,
        // and previousSibling previousSibling.
        ChildListMutationScope mutation(*this);

        // 12. If child¡¯s parent is not null, run these substeps:
        //    1. Set removedNodes to a list solely containing child.
        //    2. Remove child from its parent with the suppress observers flag set.
        if (ContainerNode *oldChildParent = oldChild->parentNode())
        {
            DOMTreeMutationDetector detector(*oldChild, *this);
            oldChildParent->RemoveChild(oldChild, exceptionState);
            if (exceptionState.HadException())
                return nullptr;
            if (!detector.NeedsRecheck())
                needsRecheck = true;
        }

#ifndef BLINKIT_CRAWLER_ONLY
        SlotAssignmentRecalcForbiddenScope forbidSlotRecalc(GetDocument());
#endif

        // 13. Let nodes be node¡¯s children if node is a DocumentFragment node, and
        // a list containing solely node otherwise.
        DOMTreeMutationDetector detector(*newChild, *this);
        if (!CollectChildrenAndRemoveFromOldParent(*newChild, targets, exceptionState))
            return oldChild;
        if (!detector.NeedsRecheck() || needsRecheck)
        {
            if (!RecheckNodeInsertionStructuralPrereq(targets, next, exceptionState))
                return oldChild;
        }

        // 10. Adopt node into parent¡¯s node document.
        // 14. Insert node into parent before reference child with the suppress
        // observers flag set.
        if (next)
            InsertNodeVector(targets, next, AdoptAndInsertBefore(), &postInsertionNotificationTargets);
        else
            InsertNodeVector(targets, nullptr, AdoptAndAppendChild(), &postInsertionNotificationTargets);
    }
    DidInsertNodeVector(targets, next, postInsertionNotificationTargets);

    // 16. Return child.
    return oldChild;
}

void ContainerNode::SetRestyleFlag(DynamicRestyleFlags mask)
{
    ASSERT(IsElementNode() || IsShadowRoot());
    EnsureRareData().SetRestyleFlag(mask);
}

void ContainerNode::Trace(Visitor *visitor)
{
    for (Node *n = m_firstChild; nullptr != n; n = n->nextSibling())
        visitor->Trace(n);
    Node::Trace(visitor);
}

void ContainerNode::WillRemoveChild(Node &child)
{
    ASSERT(child.parentNode() == this);
    ChildListMutationScope(*this).WillRemoveChild(child);
    child.NotifyMutationObserversNodeWillDetach();
    DispatchChildRemovalEvents(child);
    if (GetDocument() != child.GetDocument())
    {
        // |child| was moved another document by DOM mutation event handler.
        return;
    }

    // |nodeWillBeRemoved()| must be run after |ChildFrameDisconnector|, because
    // |ChildFrameDisconnector| can run script which may cause state that is to
    // be invalidated by removing the node.
    ScriptForbiddenScope scriptForbiddenScope;
    EventDispatchForbiddenScope assertNoEventDispatch;
    // e.g. mutation event listener can create a new range.
    GetDocument().NodeWillBeRemoved(child);
}

void ContainerNode::WillRemoveChildren(void)
{
    NodeVector children;
    GetChildNodes(*this, children);

    ChildListMutationScope mutation(*this);
    for (Node *node : children)
    {
        ASSERT(nullptr != node);
        mutation.WillRemoveChild(*node);
        node->NotifyMutationObserversNodeWillDetach();
        DispatchChildRemovalEvents(*node);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UI Implementations

#ifndef BLINKIT_CRAWLER_ONLY
void ContainerNode::FocusStateChanged(void)
{
    // If we're just changing the window's active state and the focused node has
    // no layoutObject we can just ignore the state change.
    LayoutObject *layoutObject = GetLayoutObject();
    if (nullptr == layoutObject)
        return;

    StyleChangeType changeType =
        GetComputedStyle()->HasPseudoStyle(kPseudoIdFirstLetter)
        ? kSubtreeStyleChange
        : kLocalStyleChange;
    StyleChangeReasonForTracing reason = StyleChangeReasonForTracing::CreateWithExtraData(
        StyleChangeReason::kPseudoClass, StyleChangeExtraData::g_focus);
    SetNeedsStyleRecalc(changeType, reason);

    if (IsElementNode())
    {
        Element *element = ToElement(this);
        if (element->ChildrenOrSiblingsAffectedByFocus())
            element->PseudoStateChanged(CSSSelector::kPseudoFocus);
    }

    layoutObject->InvalidateIfControlStateChanged(kFocusControlState);
    FocusVisibleStateChanged();
    FocusWithinStateChanged();
}

void ContainerNode::FocusVisibleStateChanged(void)
{
    if (!RuntimeEnabledFeatures::CSSFocusVisibleEnabled())
        return;

    StyleChangeType changeType =
        GetComputedStyle()->HasPseudoStyle(kPseudoIdFirstLetter)
        ? kSubtreeStyleChange
        : kLocalStyleChange;
    StyleChangeReasonForTracing reason = StyleChangeReasonForTracing::CreateWithExtraData(
        StyleChangeReason::kPseudoClass, StyleChangeExtraData::g_focus_visible);
    SetNeedsStyleRecalc(changeType, reason);

    if (IsElementNode())
    {
        Element *element = ToElement(this);
        if (element->ChildrenOrSiblingsAffectedByFocusVisible())
            element->PseudoStateChanged(CSSSelector::kPseudoFocusVisible);
    }
}

void ContainerNode::FocusWithinStateChanged(void)
{
    if (const ComputedStyle *style = GetComputedStyle())
    {
        if (style->AffectedByFocusWithin())
        {
            StyleChangeType changeType =
                style->HasPseudoStyle(kPseudoIdFirstLetter)
                ? kSubtreeStyleChange
                : kLocalStyleChange;
            StyleChangeReasonForTracing reason = StyleChangeReasonForTracing::CreateWithExtraData(StyleChangeReason::kPseudoClass,
                StyleChangeExtraData::g_focus_within);
            SetNeedsStyleRecalc(changeType, reason);
        }
    }
    if (IsElementNode())
    {
        Element *element = ToElement(this);
        if (element->ChildrenOrSiblingsAffectedByFocusWithin())
            element->PseudoStateChanged(CSSSelector::kPseudoFocusWithin);
    }
}

void ContainerNode::RebuildChildrenLayoutTrees(WhitespaceAttacher &whitespaceAttacher)
{
    ASSERT(!NeedsReattachLayoutTree());

    if (IsActiveSlotOrActiveV0InsertionPoint())
    {
        if (HTMLSlotElement *slot = ToHTMLSlotElementOrNull(this))
            slot->RebuildDistributedChildrenLayoutTrees(whitespaceAttacher);
        else
            ToV0InsertionPoint(this)->RebuildDistributedChildrenLayoutTrees(whitespaceAttacher);
        RebuildNonDistributedChildren();
        return;
    }

    // This loop is deliberately backwards because we use insertBefore in the
    // layout tree, and want to avoid a potentially n^2 loop to find the insertion
    // point while building the layout tree.  Having us start from the last child
    // and work our way back means in the common case, we'll find the insertion
    // point in O(1) time.  See crbug.com/288225
    for (Node *child = lastChild(); nullptr != child; child = child->previousSibling())
        RebuildLayoutTreeForChild(child, whitespaceAttacher);

    // This is done in ContainerNode::AttachLayoutTree but will never be cleared
    // if we don't enter ContainerNode::AttachLayoutTree so we do it here.
    ClearChildNeedsStyleRecalc();
}

void ContainerNode::RebuildLayoutTreeForChild(Node *child, WhitespaceAttacher &whitespaceAttacher)
{
    if (child->IsTextNode())
    {
        Text *textNode = ToText(child);
        if (child->NeedsReattachLayoutTree())
            textNode->RebuildTextLayoutTree(whitespaceAttacher);
        else
            whitespaceAttacher.DidVisitText(textNode);
        return;
    }

    if (!child->IsElementNode())
        return;

    Element* element = ToElement(child);
    if (element->NeedsRebuildLayoutTree(whitespaceAttacher))
        element->RebuildLayoutTree(whitespaceAttacher);
    else
        whitespaceAttacher.DidVisitElement(element);
}

void ContainerNode::RebuildNonDistributedChildren(void)
{
    // Non-distributed children are:
    // 1. Children of shadow hosts which are not slotted (v1) or distributed to an
    //    insertion point (v0).
    // 2. Children of <slot> (v1) and <content> (v0) elements which are not used
    //    as fallback content when no nodes are slotted/distributed.
    //
    // These children will not take part in the flat tree, but we need to walk
    // them in order to clear dirtiness flags during layout tree rebuild. We
    // need to use a separate WhitespaceAttacher so that DidVisitText does not
    // mess up the WhitespaceAttacher for the layout tree rebuild of the nodes
    // which take part in the flat tree.
    WhitespaceAttacher whitespaceAttacher;
    for (Node *child = lastChild(); nullptr != child; child = child->previousSibling())
        RebuildLayoutTreeForChild(child, whitespaceAttacher);
    ClearChildNeedsStyleRecalc();
    ClearChildNeedsReattachLayoutTree();
}

void ContainerNode::RecalcDescendantStyles(StyleRecalcChange change)
{
    ASSERT(GetDocument().InStyleRecalc());
    ASSERT(change >= kUpdatePseudoElements || ChildNeedsStyleRecalc());
    ASSERT(!NeedsStyleRecalc());

    for (Node *child = firstChild(); nullptr != child; child = child->nextSibling())
    {
        if (child->IsTextNode())
        {
            ToText(child)->RecalcTextStyle(change);
        }
        else if (child->IsElementNode())
        {
            Element *element = ToElement(child);
            if (element->ShouldCallRecalcStyle(change))
                element->RecalcStyle(change);
        }
    }
}

void ContainerNode::SetFocused(bool received, WebFocusType focusType)
{
    // Recurse up author shadow trees to mark shadow hosts if it matches :focus.
    // TODO(kochi): Handle UA shadows which marks multiple nodes as focused such
    // as <input type="date"> the same way as author shadow.
    if (ShadowRoot *root = ContainingShadowRoot())
    {
        if (!root->IsUserAgent())
            OwnerShadowHost()->SetFocused(received, focusType);
    }

    // If this is an author shadow host and indirectly focused (has focused
    // element within its shadow root), update focus.
    if (IsElementNode())
    {
        if (Element *focusedElement = GetDocument().FocusedElement())
        {
            if (focusedElement != this)
            {
                Element *element = ToElement(this);
                if (element->AuthorShadowRoot())
                    received = received && element->AuthorShadowRoot()->delegatesFocus();
            }
        }
    }

    if (IsFocused() == received)
        return;

    Node::SetFocused(received, focusType);

    FocusStateChanged();

    if (nullptr != GetLayoutObject() || received)
        return;

    // If :focus sets display: none, we lose focus but still need to recalc our
    // style.
    if (IsElementNode())
    {
        Element *element = ToElement(this);
        if (element->ChildrenOrSiblingsAffectedByFocus())
            element->PseudoStateChanged(CSSSelector::kPseudoFocus);
    }
    else
    {
        StyleChangeReasonForTracing reason = StyleChangeReasonForTracing::CreateWithExtraData(
            StyleChangeReason::kPseudoClass, StyleChangeExtraData::g_focus);
        SetNeedsStyleRecalc(kLocalStyleChange, reason);
    }

    if (RuntimeEnabledFeatures::CSSFocusVisibleEnabled())
    {
        if (IsElementNode())
        {
            Element *element = ToElement(this);
            if (element->ChildrenOrSiblingsAffectedByFocusVisible())
                element->PseudoStateChanged(CSSSelector::kPseudoFocusVisible);
        }
        else
        {
            StyleChangeReasonForTracing reason = StyleChangeReasonForTracing::CreateWithExtraData(
                StyleChangeReason::kPseudoClass, StyleChangeExtraData::g_focus_visible);
            SetNeedsStyleRecalc(kLocalStyleChange, reason);
        }
    }

    if (IsElementNode())
    {
        Element *element = ToElement(this);
        if (element->ChildrenOrSiblingsAffectedByFocusWithin())
            element->PseudoStateChanged(CSSSelector::kPseudoFocusWithin);
    }
    else
    {
        StyleChangeReasonForTracing reason = StyleChangeReasonForTracing::CreateWithExtraData(
            StyleChangeReason::kPseudoClass, StyleChangeExtraData::g_focus_within);
        SetNeedsStyleRecalc(kLocalStyleChange, reason);
    }
}

void ContainerNode::SetHasFocusWithinUpToAncestor(bool flag, Node *ancestor)
{
    for (ContainerNode *node = this; nullptr != node && node != ancestor; node = FlatTreeTraversal::Parent(*node))
    {
        node->SetHasFocusWithin(flag);
        node->FocusWithinStateChanged();
    }
}
#endif // BLINKIT_CRAWLER_ONLY

}  // namespace blink
