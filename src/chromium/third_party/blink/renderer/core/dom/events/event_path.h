// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: event_path.h
// Description: EventPath Class
//      Author: Ziming Li
//     Created: 2019-11-20
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2013 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef BLINKIT_BLINK_EVENT_PATH_H
#define BLINKIT_BLINK_EVENT_PATH_H

#pragma once

#include <vector>
#include "base/macros.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/wtf_size_t.h"

namespace blink {

class Event;
class EventTarget;
class Node;
class NodeEventContext;
class TreeScope;
class TreeScopeEventContext;
class WindowEventContext;

class EventPath final
{
public:
    explicit EventPath(Node &node, Event *event = nullptr);
    ~EventPath(void);

    void InitializeWith(Node &node, Event *event);

    NodeEventContext& at(wtf_size_t index);
    NodeEventContext& Last(void);

    bool IsEmpty(void) const { return m_nodeEventContexts.empty(); }
    wtf_size_t size(void) const;

    WindowEventContext& GetWindowEventContext(void)
    {
        ASSERT(m_windowEventContext);
        return *m_windowEventContext;
    }
    void EnsureWindowEventContext(void);

    NodeEventContext& TopNodeEventContext(void);

    static EventTarget* EventTargetRespectingTargetRules(Node &referenceNode);
private:
    EventPath(void) = delete;

    void Initialize(void);
    void CalculatePath(void);
    void CalculateAdjustedTargets(void);
    void CalculateTreeOrderAndSetNearestAncestorClosedTree(void);

    std::shared_ptr<TreeScopeEventContext> GetTreeScopeEventContext(TreeScope *treeScope);
    std::shared_ptr<TreeScopeEventContext> EnsureTreeScopeEventContext(Node *currentTarget, TreeScope *treeScope);

    std::vector<NodeEventContext> m_nodeEventContexts;
    Member<Node> m_node;
    Member<Event> m_event;
    std::vector<std::shared_ptr<TreeScopeEventContext>> m_treeScopeEventContexts;
    std::unique_ptr<WindowEventContext> m_windowEventContext;

    DISALLOW_COPY_AND_ASSIGN(EventPath);
};

} // namespace blink

#endif // BLINKIT_BLINK_EVENT_PATH_H
