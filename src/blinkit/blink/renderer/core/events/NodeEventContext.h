// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: NodeEventContext.h
// Description: NodeEventContext Class
//      Author: Ziming Li
//     Created: 2021-07-10
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

#ifndef NodeEventContext_h
#define NodeEventContext_h

#include "core/CoreExport.h"
#include "core/events/TreeScopeEventContext.h"
#include "wtf/PassRefPtr.h"
#include "wtf/RefPtr.h"

namespace blink {

class EventTarget;
class Node;
class TouchEventContext;

class CORE_EXPORT NodeEventContext {
    DISALLOW_NEW_EXCEPT_PLACEMENT_NEW();
    DECLARE_EMPTY_DESTRUCTOR_WILL_BE_REMOVED(NodeEventContext);
public:
    NodeEventContext(void)
    {
        ASSERT(false); // BKTODO:
    }
    // FIXME: Use ContainerNode instead of Node.
    NodeEventContext(PassRefPtrWillBeRawPtr<Node>, PassRefPtrWillBeRawPtr<EventTarget> currentTarget);
    DECLARE_TRACE();

    Node* node() const { return m_node.get(); }

    void setTreeScopeEventContext(PassRefPtrWillBeRawPtr<TreeScopeEventContext> prpTreeScopeEventContext) { m_treeScopeEventContext = prpTreeScopeEventContext; }
    TreeScopeEventContext& treeScopeEventContext() { ASSERT(m_treeScopeEventContext); return *m_treeScopeEventContext; }

    EventTarget* target() const { return m_treeScopeEventContext->target(); }
    EventTarget* relatedTarget() const { return m_treeScopeEventContext->relatedTarget(); }
    // BKTODO: TouchEventContext* touchEventContext() const { return m_treeScopeEventContext->touchEventContext(); }

    bool currentTargetSameAsTarget() const { return m_currentTarget == target(); }
    void handleLocalEvents(Event&) const;

private:
    GCRefPtr<Node> m_node;
    EventTarget *m_currentTarget = nullptr;
    GCRefPtr<TreeScopeEventContext> m_treeScopeEventContext;
};

} // namespace blink

WTF_ALLOW_MOVE_INIT_AND_COMPARE_WITH_MEM_FUNCTIONS(blink::NodeEventContext);

#endif // NodeEventContext_h
