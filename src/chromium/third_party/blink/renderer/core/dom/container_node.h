// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: container_node.h
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
 * Copyright (C) 2004, 2005, 2006, 2007, 2009, 2010, 2011, 2013 Apple Inc. All
 * rights reserved.
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

#ifndef BLINKIT_BLINK_CONTAINER_NODE_H
#define BLINKIT_BLINK_CONTAINER_NODE_H

#pragma once

#include "third_party/blink/renderer/core/dom/node.h"
#include "third_party/blink/renderer/core/html/collection_type.h"

namespace blink {

class HTMLCollection;
class NodeListsNodeData;
template <typename NodeType>
class StaticNodeTypeList;
using StaticElementList = StaticNodeTypeList<Element>;
#ifndef BLINKIT_CRAWLER_ONLY
class WhitespaceAttacher;
#endif

enum class DynamicRestyleFlags {
    kChildrenOrSiblingsAffectedByFocus = 1 << 0,
    kChildrenOrSiblingsAffectedByHover = 1 << 1,
    kChildrenOrSiblingsAffectedByActive = 1 << 2,
    kChildrenOrSiblingsAffectedByDrag = 1 << 3,
    kChildrenAffectedByFirstChildRules = 1 << 4,
    kChildrenAffectedByLastChildRules = 1 << 5,
    kChildrenAffectedByDirectAdjacentRules = 1 << 6,
    kChildrenAffectedByIndirectAdjacentRules = 1 << 7,
    kChildrenAffectedByForwardPositionalRules = 1 << 8,
    kChildrenAffectedByBackwardPositionalRules = 1 << 9,
    kAffectedByFirstChildRules = 1 << 10,
    kAffectedByLastChildRules = 1 << 11,
    kChildrenOrSiblingsAffectedByFocusWithin = 1 << 12,
    kChildrenOrSiblingsAffectedByFocusVisible = 1 << 13,

    kNumberOfDynamicRestyleFlags = 14,

    kChildrenAffectedByStructuralRules =
    kChildrenAffectedByFirstChildRules | kChildrenAffectedByLastChildRules |
    kChildrenAffectedByDirectAdjacentRules |
    kChildrenAffectedByIndirectAdjacentRules |
    kChildrenAffectedByForwardPositionalRules |
    kChildrenAffectedByBackwardPositionalRules
};

enum SubtreeModificationAction {
    kDispatchSubtreeModifiedEvent,
    kOmitSubtreeModifiedEvent
};

class ContainerNode : public Node
{
public:
    void Trace(Visitor *visitor) override;

    // Exports for JS
    HTMLCollection* getElementsByTagName(const AtomicString &qualifiedName);
    Element* querySelector(const AtomicString &selectors, ExceptionState &exceptionState);
    StaticElementList* querySelectorAll(const AtomicString &selectors, ExceptionState &exceptionState);

    Node* FirstChild(void) const { return m_firstChild; }
    Node* LastChild(void) const { return m_lastChild; }
    Node* RemoveChild(Node *oldChild, ExceptionState &exceptionState);
    Node* ReplaceChild(Node *newChild, Node *oldChild, ExceptionState &exceptionState);

    bool HasOneChild(void) const { return m_firstChild && nullptr == m_firstChild->nextSibling(); }
    bool HasOneTextChild(void) const { return HasOneChild() && m_firstChild->IsTextNode(); }
    bool HasChildren(void) const { return !!m_firstChild; }
    unsigned CountChildren(void) const;
    Element* GetElementById(const AtomicString &id) const;

    Node* AppendChild(Node *newChild, ExceptionState &exceptionState);
    Node* AppendChild(Node *newChild);
    bool EnsurePreInsertionValidity(const Node &newChild, const Node *next, const Node *oldChild, ExceptionState &exceptionState) const;
    Node* InsertBefore(Node *newChild, Node *refChild, ExceptionState &exceptionState);

    void RemoveChildren(SubtreeModificationAction action = kDispatchSubtreeModifiedEvent);
    void CloneChildNodesFrom(const ContainerNode &node);

    // Utility functions for NodeListsNodeData API.
    template <typename Collection>
    Collection* EnsureCachedCollection(CollectionType);
    template <typename Collection>
    Collection* EnsureCachedCollection(CollectionType, const AtomicString& name);

    void SetChildrenOrSiblingsAffectedByFocus(void) { SetRestyleFlag(DynamicRestyleFlags::kChildrenOrSiblingsAffectedByFocus); }
    bool ChildrenAffectedByFirstChildRules(void) const { return HasRestyleFlag(DynamicRestyleFlags::kChildrenAffectedByFirstChildRules); }
    void SetChildrenAffectedByFirstChildRules(void) { SetRestyleFlag(DynamicRestyleFlags::kChildrenAffectedByFirstChildRules); }
    void SetChildrenAffectedByLastChildRules(void) { SetRestyleFlag(DynamicRestyleFlags::kChildrenAffectedByLastChildRules); }
    void SetChildrenAffectedByDirectAdjacentRules(void) { SetRestyleFlag(DynamicRestyleFlags::kChildrenAffectedByDirectAdjacentRules); }
    void SetChildrenAffectedByIndirectAdjacentRules(void) { SetRestyleFlag(DynamicRestyleFlags::kChildrenAffectedByIndirectAdjacentRules); }
    bool ChildrenAffectedByForwardPositionalRules(void) const { return HasRestyleFlag(DynamicRestyleFlags::kChildrenAffectedByForwardPositionalRules); }
    void SetChildrenAffectedByForwardPositionalRules(void) { SetRestyleFlag(DynamicRestyleFlags::kChildrenAffectedByForwardPositionalRules); }
    bool ChildrenAffectedByBackwardPositionalRules(void) const { return HasRestyleFlag(DynamicRestyleFlags::kChildrenAffectedByBackwardPositionalRules); }
    void SetChildrenAffectedByBackwardPositionalRules(void) { SetRestyleFlag(DynamicRestyleFlags::kChildrenAffectedByBackwardPositionalRules); }
    void SetAffectedByFirstChildRules(void) { SetRestyleFlag(DynamicRestyleFlags::kAffectedByFirstChildRules); }
    void SetAffectedByLastChildRules(void) { SetRestyleFlag(DynamicRestyleFlags::kAffectedByLastChildRules); }
    void SetChildrenOrSiblingsAffectedByFocusVisible(void) { SetRestyleFlag(DynamicRestyleFlags::kChildrenOrSiblingsAffectedByFocusVisible); }

    void ParserAppendChild(Node *newChild);
    void ParserRemoveChild(Node &oldChild);
    void ParserInsertBefore(Node *newChild, Node &refChild);
    void ParserTakeAllChildrenFrom(ContainerNode &oldParent);

    enum ChildrenChangeType {
        kElementInserted,
        kNonElementInserted,
        kElementRemoved,
        kNonElementRemoved,
        kAllChildrenRemoved,
        kTextChanged
    };
    enum ChildrenChangeSource {
        kChildrenChangeSourceAPI,
        kChildrenChangeSourceParser
    };
    struct ChildrenChange;
    virtual void ChildrenChanged(const ChildrenChange &change);

#ifndef BLINKIT_CRAWLER_ONLY
    // FIXME: These methods should all be renamed to something better than
    // "check", since it's not clear that they alter the style bits of siblings
    // and children.
    enum SiblingCheckType {
        kFinishedParsingChildren,
        kSiblingElementInserted,
        kSiblingElementRemoved
    };
    void CheckForSiblingStyleChanges(SiblingCheckType changeType, Element *changedElement,
        Node *nodeBeforeChange, Node *nodeAfterChange);
    void RecalcDescendantStyles(StyleRecalcChange);

    void RebuildChildrenLayoutTrees(WhitespaceAttacher &whitespaceAttacher);
    void RebuildLayoutTreeForChild(Node *child, WhitespaceAttacher &whitespaceAttacher);
    void RebuildNonDistributedChildren(void);

    void SetFocused(bool received, WebFocusType focusType) override;

    void AttachLayoutTree(AttachContext &context) override;
    void DetachLayoutTree(const AttachContext &context = AttachContext()) override;
#endif
protected:
    ContainerNode(TreeScope *treeScope, ConstructionType type = kCreateContainer);

    void SetFirstChild(Node *child) { m_firstChild = child; }
    void SetLastChild(Node *child) { m_lastChild = child; }

    void InvalidateNodeListCachesInAncestors(const QualifiedName *attrName, Element *attributeOwnerElement,
        const ChildrenChange *change);
private:
    class AdoptAndAppendChild;
    class AdoptAndInsertBefore;
    friend class AdoptAndAppendChild;
    friend class AdoptAndInsertBefore;

    NodeListsNodeData& EnsureNodeLists(void);

    bool HasRestyleFlag(DynamicRestyleFlags mask) const { return HasRareData() && HasRestyleFlagInternal(mask); }
    bool HasRestyleFlagInternal(DynamicRestyleFlags mask) const;
    void SetRestyleFlag(DynamicRestyleFlags mask);

    void AppendChildCommon(Node &child);
    bool CheckParserAcceptChild(const Node &newChild) const;
    void DidInsertNodeVector(const NodeVector &targets, Node *next, const NodeVector& postInsertionNotificationTargets);
    void InsertBeforeCommon(Node &nextChild, Node &newChild);
    // Inserts the specified nodes before |next|.
    // |next| may be nullptr.
    // |post_insertion_notification_targets| must not be nullptr.
    template <typename Functor>
    void InsertNodeVector(const NodeVector &targets, Node *next, const Functor &mutator, NodeVector *postInsertionNotificationTargets);
    void NotifyNodeInserted(Node &root, ChildrenChangeSource source = kChildrenChangeSourceAPI);
    void NotifyNodeInsertedInternal(Node &root, NodeVector &postInsertionNotificationTargets);
    void NotifyNodeRemoved(Node &root);
    void RemoveBetween(Node *previousChild, Node* nextChild, Node &oldChild);
    void WillRemoveChild(Node &child);
    void WillRemoveChildren(void);

    bool RecheckNodeInsertionStructuralPrereq(const NodeVector &newChildren, const Node *next, ExceptionState &exceptionState);
    inline bool IsHostIncludingInclusiveAncestorOfThis(const Node &newChild, ExceptionState &exceptionState) const;
    inline bool IsChildTypeAllowed(const Node &child) const;

    Member<Node> m_firstChild;
    Member<Node> m_lastChild;
};

DEFINE_NODE_TYPE_CASTS(ContainerNode, IsContainerNode());

struct ContainerNode::ChildrenChange
{
    STACK_ALLOCATED();
public:
    static ChildrenChange ForInsertion(Node &node, Node *unchangedPrevious, Node *unchangedNext, ChildrenChangeSource byParser)
    {
        ChildrenChange change;
        change.type                  = node.IsElementNode() ? kElementInserted : kNonElementInserted;
        change.sibling_changed       = &node;
        change.sibling_before_change = unchangedPrevious;
        change.sibling_after_change  = unchangedNext;
        change.by_parser             = byParser;
        return change;
    }
    static ChildrenChange ForRemoval(Node &node, Node *previousSibling, Node *nextSibling, ChildrenChangeSource byParser)
    {
        ChildrenChange change;
        change.type                  = node.IsElementNode() ? kElementRemoved : kNonElementRemoved;
        change.sibling_changed       = &node;
        change.sibling_before_change = previousSibling;
        change.sibling_after_change  = nextSibling;
        change.by_parser             = byParser;
        return change;
    }

    bool IsChildElementChange(void) const { return kElementInserted == type || kElementRemoved == type; }
    bool IsChildInsertion(void) const { return kElementInserted == type || kNonElementInserted == type; }
    bool IsChildRemoval(void) const { return kElementRemoved == type || kNonElementRemoved == type; }

    ChildrenChangeType type;
    Member<Node> sibling_changed;
    // |siblingBeforeChange| is
    //  - siblingChanged.previousSibling before node removal
    //  - siblingChanged.previousSibling after single node insertion
    //  - previousSibling of the first inserted node after multiple node
    //    insertion
    Member<Node> sibling_before_change;
    // |siblingAfterChange| is
    //  - siblingChanged.nextSibling before node removal
    //  - siblingChanged.nextSibling after single node insertion
    //  - nextSibling of the last inserted node after multiple node insertion.
    Member<Node> sibling_after_change;
    ChildrenChangeSource by_parser;
};

inline void GetChildNodes(ContainerNode &node, NodeVector &nodes)
{
    ASSERT(nodes.empty());
    for (Node *child = node.FirstChild(); nullptr != child; child = child->nextSibling())
        nodes.push_back(child);
}

} // namespace blink

#endif // BLINKIT_BLINK_CONTAINER_NODE_H
