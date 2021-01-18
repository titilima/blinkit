// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: live_node_list_base.h
// Description: LiveNodeListBase Class
//      Author: Ziming Li
//     Created: 2019-12-31
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2004, 2006, 2007 Apple Inc. All rights reserved.
 * Copyright (C) 2014 Samsung Electronics. All rights reserved.
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

#ifndef BLINKIT_BLINK_LIVE_NODE_LIST_BASE_H
#define BLINKIT_BLINK_LIVE_NODE_LIST_BASE_H

#pragma once

#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/dom/element_traversal.h"
#include "third_party/blink/renderer/platform/heap/handle.h"

namespace blink {

enum class NodeListSearchRoot {
    kOwnerNode,
    kTreeScope,
};

class LiveNodeListBase : public GarbageCollectedMixin
{
public:
    LiveNodeListBase(ContainerNode &ownerNode,
        NodeListSearchRoot searchRoot,
        NodeListInvalidationType invalidationType,
        CollectionType collectionType)
        : m_ownerNode(ownerNode)
        , m_searchRoot(static_cast<unsigned>(searchRoot))
        , m_invalidationType(invalidationType)
        , m_collectionType(collectionType)
    {
        ASSERT(static_cast<unsigned>(searchRoot) == m_searchRoot);
        ASSERT(static_cast<unsigned>(invalidationType) == m_invalidationType);
        ASSERT(static_cast<unsigned>(collectionType) == m_collectionType);
    }
    virtual ~LiveNodeListBase(void) = default;

    ContainerNode& RootNode(void) const;
    ContainerNode& ownerNode(void) const { return *m_ownerNode; }

    bool IsRootedAtTreeScope(void) const { return m_searchRoot == static_cast<unsigned>(NodeListSearchRoot::kTreeScope); }
    NodeListInvalidationType InvalidationType(void) const { return static_cast<NodeListInvalidationType>(m_invalidationType); }
    CollectionType GetType(void) const { return static_cast<CollectionType>(m_collectionType); }

    virtual void InvalidateCache(Document *oldDocument = nullptr) const = 0;
    void InvalidateCacheForAttribute(const QualifiedName *attrName) const;

    static bool ShouldInvalidateTypeOnAttributeChange(NodeListInvalidationType type, const QualifiedName &attrName);
protected:
    Document& GetDocument(void) const { return m_ownerNode->GetDocument(); }

    template <typename MatchFunc>
    static Element* TraverseMatchingElementsForwardToOffset(
        Element &currentElement,
        const ContainerNode *stayWithin,
        unsigned offset, unsigned &currentOffset,
        MatchFunc isMatch)
    {
        ASSERT(currentOffset < offset);
        for (Element *next = ElementTraversal::Next(currentElement, stayWithin, isMatch); nullptr != next; next = ElementTraversal::Next(*next, stayWithin, isMatch))
        {
            if (++currentOffset == offset)
                return next;
        }
        return nullptr;
    }
private:
    Member<ContainerNode> m_ownerNode;  // Cannot be null.
    const unsigned m_searchRoot : 1;
    const unsigned m_invalidationType : 4;
    const unsigned m_collectionType : 5;
};

} // namespace blink

#endif // BLINKIT_BLINK_LIVE_NODE_LIST_BASE_H
