// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: tree_scope_event_context.cpp
// Description: TreeScopeEventContext Class
//      Author: Ziming Li
//     Created: 2019-11-20
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
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

#include "tree_scope_event_context.h"

#include "base/memory/ptr_util.h"
#include "third_party/blink/renderer/core/dom/node.h"

namespace blink {
TreeScopeEventContext::TreeScopeEventContext(TreeScope &treeScope) : m_treeScope(treeScope)
{
}

int TreeScopeEventContext::CalculateTreeOrderAndSetNearestAncestorClosedTree(int orderNumber, TreeScopeEventContext *nearestAncestorClosedTreeScopeEventContext)
{
    m_preOrder = orderNumber;
#ifdef BLINKIT_CRAWLER_ONLY
    m_containingClosedShadowTree = nearestAncestorClosedTreeScopeEventContext;
#else
    ASSERT(false); // BKTODO:
#if 0
    m_containingClosedShadowTree =
        (RootNode().IsShadowRoot() && !ToShadowRoot(RootNode()).IsOpenOrV0())
        ? this
        : nearestAncestorClosedTreeScopeEventContext;
#endif
#endif
    for (TreeScopeEventContext *context : m_children)
    {
        orderNumber = context->CalculateTreeOrderAndSetNearestAncestorClosedTree(orderNumber + 1,
            ContainingClosedShadowTree());
    }
    m_postOrder = orderNumber + 1;

    return orderNumber + 1;
}

inline void TreeScopeEventContext::CheckReachableNode(EventTarget &target)
{
#if DCHECK_IS_ON()
    Node *node = target.ToNode();
    if (nullptr == node)
        return;
    ASSERT(node->GetTreeScope().IsInclusiveOlderSiblingShadowRootOrAncestorTreeScopeOf(GetTreeScope()));
#endif
}

std::shared_ptr<TreeScopeEventContext> TreeScopeEventContext::Create(TreeScope &treeScope)
{
    return base::WrapShared(new TreeScopeEventContext(treeScope));
}

void TreeScopeEventContext::SetRelatedTarget(EventTarget *relatedTarget)
{
    ASSERT(nullptr != relatedTarget);
    CheckReachableNode(*relatedTarget);
    m_relatedTarget = relatedTarget;
}

void TreeScopeEventContext::SetTarget(EventTarget *target)
{
    ASSERT(nullptr != target);
    CheckReachableNode(*target);
    m_target = target;
}

}  // namespace blink
