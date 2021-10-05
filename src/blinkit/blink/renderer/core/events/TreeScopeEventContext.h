// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: TreeScopeEventContext.h
// Description: TreeScopeEventContext Class
//      Author: Ziming Li
//     Created: 2021-09-09
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2014 Google Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef TreeScopeEventContext_h
#define TreeScopeEventContext_h

#include "core/CoreExport.h"
#include "core/dom/Node.h"
#include "core/dom/TreeScope.h"
#include "core/events/EventTarget.h"
#include "wtf/PassRefPtr.h"
#include "wtf/RefPtr.h"
#include "wtf/Vector.h"

namespace blink {

class EventPath;
class EventTarget;
class Node;
template <typename NodeType> class StaticNodeTypeList;
using StaticNodeList = StaticNodeTypeList<Node>;
class TouchEventContext;
class TreeScope;

class CORE_EXPORT TreeScopeEventContext final : public BlinKit::GCObject {
    DECLARE_EMPTY_DESTRUCTOR_WILL_BE_REMOVED(TreeScopeEventContext);
public:
    static PassRefPtrWillBeRawPtr<TreeScopeEventContext> create(TreeScope&);
    DECLARE_TRACE();

    TreeScope& treeScope() const { return m_treeScope; }
    Node& rootNode() const { return *m_rootNode; }

    EventTarget* target() const { return m_target; }
    void setTarget(PassRefPtrWillBeRawPtr<EventTarget>);

    EventTarget* relatedTarget() const { return m_relatedTarget; }
    void setRelatedTarget(PassRefPtrWillBeRawPtr<EventTarget>);

#if 0 // BKTODO:
    TouchEventContext* touchEventContext() const { return m_touchEventContext.get(); }
    TouchEventContext* ensureTouchEventContext();
#endif

    std::vector<EventTarget *>& ensureEventPath(EventPath&);

    bool isInclusiveAncestorOf(const TreeScopeEventContext&) const;
    bool isDescendantOf(const TreeScopeEventContext&) const;
#if ENABLE(ASSERT)
    bool isExclusivePartOf(const TreeScopeEventContext&) const;
#endif
    void addChild(TreeScopeEventContext& child) { m_children.emplace_back(&child); }

    // For ancestor-descendant relationship check in O(1).
    // Preprocessing takes O(N).
    int calculateTreeOrderAndSetNearestAncestorClosedTree(int orderNumber, TreeScopeEventContext* nearestAncestorClosedTreeScopeEventContext);

    TreeScopeEventContext* containingClosedShadowTree() const { return m_containingClosedShadowTree; }

private:
    TreeScopeEventContext(TreeScope&);

#if ENABLE(ASSERT)
    bool isUnreachableNode(EventTarget&);
#endif

    bool isUnclosedTreeOf(const TreeScopeEventContext& other);

    TreeScope &m_treeScope;
    GCRefPtr<Node> m_rootNode; // Prevents TreeScope from being freed. TreeScope itself isn't RefCounted.
    EventTarget *m_target = nullptr;
    EventTarget *m_relatedTarget = nullptr;
    std::unique_ptr<std::vector<EventTarget *>> m_eventPath;
    // BKTODO: RefPtrWillBeMember<TouchEventContext> m_touchEventContext;
    TreeScopeEventContext *m_containingClosedShadowTree = nullptr;

    std::vector<TreeScopeEventContext *> m_children;
    int m_preOrder;
    int m_postOrder;
};

#if ENABLE(ASSERT)
inline bool TreeScopeEventContext::isUnreachableNode(EventTarget& target)
{
    // FIXME: Checks also for SVG elements.
    return target.toNode() && !target.toNode()->isSVGElement() && !target.toNode()->treeScope().isInclusiveOlderSiblingShadowRootOrAncestorTreeScopeOf(treeScope());
}
#endif

inline void TreeScopeEventContext::setTarget(PassRefPtrWillBeRawPtr<EventTarget> target)
{
    ASSERT(target);
    ASSERT(!isUnreachableNode(*target));
    m_target = target;
}

inline void TreeScopeEventContext::setRelatedTarget(PassRefPtrWillBeRawPtr<EventTarget> relatedTarget)
{
    ASSERT(relatedTarget);
    ASSERT(!isUnreachableNode(*relatedTarget));
    m_relatedTarget = relatedTarget;
}

inline bool TreeScopeEventContext::isInclusiveAncestorOf(const TreeScopeEventContext& other) const
{
    ASSERT(m_preOrder != -1 && m_postOrder != -1 && other.m_preOrder != -1 && other.m_postOrder != -1);
    return m_preOrder <= other.m_preOrder && other.m_postOrder <= m_postOrder;
}

inline bool TreeScopeEventContext::isDescendantOf(const TreeScopeEventContext& other) const
{
    ASSERT(m_preOrder != -1 && m_postOrder != -1 && other.m_preOrder != -1 && other.m_postOrder != -1);
    return other.m_preOrder < m_preOrder && m_postOrder < other.m_postOrder;
}

#if ENABLE(ASSERT)
inline bool TreeScopeEventContext::isExclusivePartOf(const TreeScopeEventContext& other) const
{
    ASSERT(m_preOrder != -1 && m_postOrder != -1 && other.m_preOrder != -1 && other.m_postOrder != -1);
    return (m_preOrder < other.m_preOrder && m_postOrder < other.m_preOrder)
        || (m_preOrder > other.m_preOrder && m_preOrder > other.m_postOrder);
}
#endif

}

#endif // TreeScopeEventContext_h
