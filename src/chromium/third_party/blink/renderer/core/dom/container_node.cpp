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
#include "third_party/blink/renderer/core/dom/child_list_mutation_scope.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/dom/element_traversal.h"
#include "third_party/blink/renderer/core/dom/events/event_dispatch_forbidden_scope.h"
#include "third_party/blink/renderer/core/dom/node_traversal.h"
#include "third_party/blink/renderer/core/html_element_type_helpers.h"
#include "third_party/blink/renderer/platform/bindings/exception_state.h"
#include "third_party/blink/renderer/platform/bindings/script_forbidden_scope.h"

namespace blink {

class ContainerNode::AdoptAndAppendChild
{
public:
    inline void operator()(ContainerNode &container, Node &child, Node *) const
    {
        container.GetTreeScope().AdoptIfNeeded(child);
        container.AppendChildCommon(child);
    }
};

ContainerNode::ContainerNode(TreeScope *treeScope, ConstructionType type) : Node(treeScope, type)
{
}

Node* ContainerNode::AppendChild(Node *newChild)
{
    ASSERT(false); // BKTODO:
    return nullptr;
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
    // BKTODO: Check overrides:
    //   * HTML Elements
    //   * ShadowRoot
    //   * V0InsertionPoint
    ASSERT(false); // BKTODO:
    if (change.IsChildInsertion()) {
        if (change.sibling_changed->NeedsStyleRecalc())
            MarkAncestorsWithChildNeedsStyleRecalc(change.sibling_changed);
    }
    else if (change.IsChildRemoval() || change.type == kAllChildrenRemoved) {
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

void ContainerNode::FastCleanupChildren(void)
{
    std::stack<Node *> children;

    Node *n = m_firstChild;
    while (nullptr != n)
    {
        ASSERT(!n->OwnedByContext());
        children.push(n);
        n = NodeTraversal::Next(*n);
    }

    m_firstChild = m_lastChild = nullptr;

    while (!children.empty())
    {
        delete children.top();
        children.pop();
    }
}

Element* ContainerNode::getElementById(const AtomicString &id) const
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
        ASSERT(false); // BKTODO:
#if 0
        if (NodeListsNodeData* lists = RareData()->NodeLists()) {
            if (ChildNodeList* child_node_list = lists->GetChildNodeList(*this)) {
                if (change) {
                    child_node_list->ChildrenChanged(*change);
                }
                else {
                    child_node_list->InvalidateCache();
                }
            }
        }
#endif
    }

    // Modifications to attributes that are not associated with an Element can't
    // invalidate NodeList caches.
    if (nullptr != attrName && nullptr == attributeOwnerElement)
        return;

    Document &document = GetDocument();
    if (!document.ShouldInvalidateNodeListCaches(attrName))
        return;

    ASSERT(false); // BKTODO:
#if 0
    document.InvalidateNodeListCaches(attrName);
#endif

    for (ContainerNode *node = this; nullptr != node; node = node->parentNode())
    {
        ASSERT(false); // BKTODO:
#if 0
        if (NodeListsNodeData* lists = node->NodeLists())
            lists->InvalidateCaches(attr_name);
#endif
    }
}

void ContainerNode::NotifyNodeInserted(Node &root, ChildrenChangeSource source)
{
#if DCHECK_IS_ON()
    DCHECK(!EventDispatchForbiddenScope::IsEventDispatchForbidden());
#endif
    DCHECK(!root.IsShadowRoot());

    if (GetDocument().ContainsV1ShadowTree())
        root.CheckSlotChangeAfterInserted();

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
        ASSERT(false); // BKTODO:
        if (ShadowRoot *shadowRoot = node.GetShadowRoot())
            NotifyNodeInsertedInternal(*shadow_root,
                post_insertion_notification_targets);
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
    while (Node *child = oldParent.firstChild())
    {
        // Explicitly remove since appending can fail, but this loop shouldn't be
        // infinite.
        oldParent.ParserRemoveChild(*child);
        ParserAppendChild(child);
    }
}

void ContainerNode::RemoveChildren(SubtreeModificationAction action)
{
    if (!m_firstChild)
    {
        ASSERT(!m_lastChild);
        return;
    }

    ASSERT(false); // BKTODO:
}

}  // namespace blink
