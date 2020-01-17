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

#include <vector>
#include "third_party/blink/renderer/core/dom/node.h"

namespace blink {

enum SubtreeModificationAction {
    kDispatchSubtreeModifiedEvent,
    kOmitSubtreeModifiedEvent
};

using NodeVector = std::vector<Node *>;

class ContainerNode : public Node
{
public:
    Node* firstChild(void) const { return m_firstChild; }
    Node* lastChild(void) const { return m_lastChild; }
    bool HasChildren(void) const { return !!m_firstChild; }
    unsigned CountChildren(void) const;

    Node* AppendChild(Node *newChild);

    void RemoveChildren(SubtreeModificationAction action = kDispatchSubtreeModifiedEvent);
    void CloneChildNodesFrom(const ContainerNode &node);

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
protected:
    ContainerNode(TreeScope *treeScope, ConstructionType type = kCreateContainer);

    void SetFirstChild(Node *child) { m_firstChild = child; }
    void SetLastChild(Node *child) { m_lastChild = child; }
    void FastCleanupChildren(void);

    void InvalidateNodeListCachesInAncestors(const QualifiedName *attrName, Element *attributeOwnerElement,
        const ChildrenChange *change);
private:
    class AdoptAndAppendChild;
    friend class AdoptAndAppendChild;

    void AppendChildCommon(Node &child);
    bool CheckParserAcceptChild(const Node &newChild) const;
    void NotifyNodeInserted(Node &root, ChildrenChangeSource source = kChildrenChangeSourceAPI);
    void NotifyNodeInsertedInternal(Node &root, NodeVector &postInsertionNotificationTargets);

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
        change.type = node.IsElementNode() ? kElementInserted : kNonElementInserted;
        change.siblingChanged = &node;
        change.siblingBeforeChange = unchangedPrevious;
        change.siblingAfterChange = unchangedNext;
        change.byParser = byParser;
        return change;
    }

    bool IsChildInsertion(void) const { return kElementInserted == type || kNonElementInserted == type; }

    ChildrenChangeType type;
    Member<Node> siblingChanged;
    // |siblingBeforeChange| is
    //  - siblingChanged.previousSibling before node removal
    //  - siblingChanged.previousSibling after single node insertion
    //  - previousSibling of the first inserted node after multiple node
    //    insertion
    Member<Node> siblingBeforeChange;
    // |siblingAfterChange| is
    //  - siblingChanged.nextSibling before node removal
    //  - siblingChanged.nextSibling after single node insertion
    //  - nextSibling of the last inserted node after multiple node insertion.
    Member<Node> siblingAfterChange;
    ChildrenChangeSource byParser;
};

}  // namespace blink

#endif  // BLINKIT_BLINK_CONTAINER_NODE_H
