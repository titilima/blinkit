#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: NodeIterator.h
// Description: NodeIterator Class
//      Author: Ziming Li
//     Created: 2022-03-02
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2000 Frederik Holljen (frederik.holljen@hig.no)
 * Copyright (C) 2001 Peter Kelly (pmk@post.com)
 * Copyright (C) 2006 Samuel Weinig (sam.weinig@gmail.com)
 * Copyright (C) 2004, 2008 Apple Inc. All rights reserved.
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

#ifndef NodeIterator_h
#define NodeIterator_h

#include "blinkit/blink/renderer/core/dom/NodeFilter.h"
#include "blinkit/blink/renderer/core/dom/NodeIteratorBase.h"
#include "blinkit/blink/renderer/wtf/PassRefPtr.h"
#include "blinkit/blink/renderer/wtf/RefCounted.h"

namespace blink {

class ExceptionState;

class NodeIterator final : public RefCountedWillBeGarbageCollected<NodeIterator>, public ScriptWrappable, public NodeIteratorBase
{
    DEFINE_WRAPPERTYPEINFO();
    WILL_BE_USING_GARBAGE_COLLECTED_MIXIN(NodeIterator);
public:
    static PassRefPtrWillBeRawPtr<NodeIterator> create(PassRefPtrWillBeRawPtr<Node> rootNode, unsigned whatToShow, PassRefPtrWillBeRawPtr<NodeFilter> filter)
    {
        return adoptRefWillBeNoop(new NodeIterator(rootNode, whatToShow, filter));
    }

#if !ENABLE(OILPAN)
    ~NodeIterator();
#endif

    PassRefPtrWillBeRawPtr<Node> nextNode(ExceptionState&);
    PassRefPtrWillBeRawPtr<Node> previousNode(ExceptionState&);
    void detach();

    Node* referenceNode() const { return m_referenceNode.node.get(); }
    bool pointerBeforeReferenceNode() const { return m_referenceNode.isPointerBeforeNode; }

    // This function is called before any node is removed from the document tree.
    void nodeWillBeRemoved(Node&);

    DECLARE_VIRTUAL_TRACE();

private:
    NodeIterator(PassRefPtrWillBeRawPtr<Node>, unsigned whatToShow, PassRefPtrWillBeRawPtr<NodeFilter>);

    class NodePointer {
        DISALLOW_NEW();
    public:
        NodePointer();
        NodePointer(PassRefPtrWillBeRawPtr<Node>, bool);

        void clear();
        bool moveToNext(Node* root);
        bool moveToPrevious(Node* root);

        RefPtrWillBeMember<Node> node;
        bool isPointerBeforeNode;

        DEFINE_INLINE_TRACE()
        {
            visitor->trace(node);
        }
    };

    void updateForNodeRemoval(Node& nodeToBeRemoved, NodePointer&) const;

    NodePointer m_referenceNode;
    NodePointer m_candidateNode;
};

} // namespace blink

#endif // NodeIterator_h
