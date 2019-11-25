// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: tree_scope_event_context.h
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

#ifndef BLINKIT_BLINK_TREE_SCOPE_EVENT_CONTEXT_H
#define BLINKIT_BLINK_TREE_SCOPE_EVENT_CONTEXT_H

#pragma once

#include <vector>
#include "third_party/blink/renderer/platform/heap/handle.h"

namespace blink {

class Event;
class EventTarget;
class TreeScope;

class TreeScopeEventContext final : public GarbageCollected<TreeScopeEventContext>
{
public:
    static std::shared_ptr<TreeScopeEventContext> Create(TreeScope &treeScope);

    TreeScope& GetTreeScope(void) const { return *m_treeScope; }

    EventTarget* Target(void) const { return m_target.Get(); }
    void SetTarget(EventTarget *target);

    EventTarget* RelatedTarget(void) const { return m_relatedTarget.Get(); }
    void SetRelatedTarget(EventTarget *relatedTarget);

    void AddChild(TreeScopeEventContext &child) { m_children.push_back(&child); }

    // For ancestor-descendant relationship check in O(1).
    // Preprocessing takes O(N).
    int CalculateTreeOrderAndSetNearestAncestorClosedTree(int orderNumber, TreeScopeEventContext *nearestAncestorClosedTreeScopeEventContext);

    TreeScopeEventContext* ContainingClosedShadowTree(void) const { return m_containingClosedShadowTree.Get(); }
private:
    TreeScopeEventContext(TreeScope &treeScope);

    void CheckReachableNode(EventTarget &target);

    Member<TreeScope> m_treeScope;
    Member<EventTarget> m_target, m_relatedTarget;
    Member<TreeScopeEventContext> m_containingClosedShadowTree;

    std::vector<TreeScopeEventContext *> m_children;
    int m_preOrder = -1;
    int m_postOrder = -1;
};

} // namespace blink

#endif // BLINKIT_BLINK_TREE_SCOPE_EVENT_CONTEXT_H
