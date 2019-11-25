// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: event_path.cpp
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

#include "event_path.h"

#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/dom/events/node_event_context.h"
#include "third_party/blink/renderer/core/dom/events/window_event_context.h"

namespace blink {

EventPath::EventPath(Node &node, Event *event) : m_node(node), m_event(event)
{
    Initialize();
}

EventPath::~EventPath(void) = default;

NodeEventContext & EventPath::at(wtf_size_t index)
{
    return m_nodeEventContexts.at(index);
}

void EventPath::CalculateAdjustedTargets(void)
{
    const TreeScope *lastTreeScope = nullptr;
    std::shared_ptr<TreeScopeEventContext> lastTreeScopeEventContext;

    for (auto &context : m_nodeEventContexts)
    {
        Node *currentNode = context.GetNode();
        TreeScope &currentTreeScope = currentNode->GetTreeScope();
        if (lastTreeScope != &currentTreeScope)
            lastTreeScopeEventContext = EnsureTreeScopeEventContext(currentNode, &currentTreeScope);
        ASSERT(lastTreeScopeEventContext);
        context.SetTreeScopeEventContext(lastTreeScopeEventContext);
        lastTreeScope = &currentTreeScope;
    }
}

void EventPath::CalculatePath(void)
{
    ASSERT(m_node);
    ASSERT(m_nodeEventContexts.empty());
    m_node->UpdateDistributionForLegacyDistributedNodes();

    // For performance and memory usage reasons we want to store the
    // path using as few bytes as possible and with as few allocations
    // as possible which is why we gather the data on the stack before
    // storing it in a perfectly sized node_event_contexts_ Vector.
    std::vector<Node *> nodesInPath;
    nodesInPath.reserve(64);

    Node *current = m_node;
    nodesInPath.push_back(current);
    while (nullptr != current)
    {
        if (m_event && current->KeepEventInNode(*m_event))
            break;

#ifdef BLINKIT_CRAWLER_ONLY
        current = current->parentNode();
        if (nullptr != current)
            nodesInPath.push_back(current);
#else
        ASSERT(false); // BKTODO:
        std::vector<V0InsertionPoint *> insertionPoints;
        HeapVector<Member<V0InsertionPoint>, 8> insertion_points;
        CollectDestinationInsertionPoints(*current, insertion_points);
        if (!insertion_points.IsEmpty()) {
            for (const auto& insertion_point : insertion_points)
                nodes_in_path.push_back(insertion_point);
            current = insertion_points.back();
            continue;
        }
        if (current->IsChildOfV1ShadowHost()) {
            if (HTMLSlotElement* slot = current->AssignedSlot()) {
                current = slot;
                nodes_in_path.push_back(current);
                continue;
            }
        }
        if (current->IsShadowRoot()) {
            if (event_ &&
                ShouldStopAtShadowRoot(*event_, *ToShadowRoot(current), *node_))
                break;
            current = current->OwnerShadowHost();
            nodes_in_path.push_back(current);
        }
        else {
            current = current->parentNode();
            if (current)
                nodes_in_path.push_back(current);
        }
#endif
    }

    m_nodeEventContexts.reserve(nodesInPath.size());
    for (Node *nodeInPath : nodesInPath)
    {
        EventTarget *target = EventTargetRespectingTargetRules(*nodeInPath);
        m_nodeEventContexts.push_back(NodeEventContext(nodeInPath, target));
    }
}

void EventPath::CalculateTreeOrderAndSetNearestAncestorClosedTree(void)
{
    // Precondition:
    //   - TreeScopes in tree_scope_event_contexts_ must be *connected* in the
    //     same composed tree.
    //   - The root tree must be included.
    std::shared_ptr<TreeScopeEventContext> rootTree;
    for (const auto &treeScopeEventContext : m_treeScopeEventContexts)
    {
        TreeScope *parent = treeScopeEventContext->GetTreeScope().ParentTreeScope();
        if (nullptr == parent)
        {
            ASSERT(!rootTree);
            rootTree = treeScopeEventContext;
            continue;
        }
        std::shared_ptr<TreeScopeEventContext> parentTreeScopeEventContext = GetTreeScopeEventContext(parent);
        DCHECK(parentTreeScopeEventContext);
        parentTreeScopeEventContext->AddChild(*treeScopeEventContext);
    }
    ASSERT(rootTree);
    rootTree->CalculateTreeOrderAndSetNearestAncestorClosedTree(0, nullptr);
}

std::shared_ptr<TreeScopeEventContext> EventPath::EnsureTreeScopeEventContext(Node *currentTarget, TreeScope *treeScope)
{
    if (nullptr == treeScope)
        return nullptr;
    std::shared_ptr<TreeScopeEventContext> treeScopeEventContext = GetTreeScopeEventContext(treeScope);
    if (!treeScopeEventContext)
    {
        treeScopeEventContext = TreeScopeEventContext::Create(*treeScope);
        m_treeScopeEventContexts.push_back(treeScopeEventContext);

        std::shared_ptr<TreeScopeEventContext> parentTreeScopeEventContext = EnsureTreeScopeEventContext(nullptr, treeScope->ParentTreeScope());
        if (parentTreeScopeEventContext && nullptr != parentTreeScopeEventContext->Target())
            treeScopeEventContext->SetTarget(parentTreeScopeEventContext->Target());
        else if (nullptr != currentTarget)
            treeScopeEventContext->SetTarget(EventTargetRespectingTargetRules(*currentTarget));
    }
    else if (nullptr == treeScopeEventContext->Target() && nullptr != currentTarget)
    {
        treeScopeEventContext->SetTarget(EventTargetRespectingTargetRules(*currentTarget));
    }
    return treeScopeEventContext;
}

void EventPath::EnsureWindowEventContext(void)
{
    ASSERT(m_event);
    if (!m_windowEventContext)
        m_windowEventContext = std::make_unique<WindowEventContext>(*m_event, TopNodeEventContext());
}

EventTarget* EventPath::EventTargetRespectingTargetRules(Node &referenceNode)
{
    if (referenceNode.IsPseudoElement())
    {
        ASSERT(nullptr != referenceNode.parentNode());
        return referenceNode.parentNode();
    }

    return &referenceNode;
}

std::shared_ptr<TreeScopeEventContext> EventPath::GetTreeScopeEventContext(TreeScope *treeScope)
{
    ASSERT(nullptr != treeScope);
    for (auto &treeScopeEventContext : m_treeScopeEventContexts)
    {
        if (treeScopeEventContext->GetTreeScope() == treeScope)
            return treeScopeEventContext;
    }
    return nullptr;
}

static inline bool EventPathShouldBeEmptyFor(Node &node)
{
    return node.IsPseudoElement() && nullptr == node.parentElement();
}

void EventPath::Initialize(void)
{
    if (EventPathShouldBeEmptyFor(*m_node))
        return;

    CalculatePath();
    CalculateAdjustedTargets();
    CalculateTreeOrderAndSetNearestAncestorClosedTree();
}

void EventPath::InitializeWith(Node &node, Event *event)
{
    ASSERT(false); // BKTODO:
}

NodeEventContext& EventPath::Last(void)
{
    return m_nodeEventContexts.back();
}

wtf_size_t EventPath::size(void) const
{
    return m_nodeEventContexts.size();
}

NodeEventContext& EventPath::TopNodeEventContext(void)
{
    ASSERT(!IsEmpty());
    return Last();
}

}  // namespace blink
