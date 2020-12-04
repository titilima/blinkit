// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: document_lifecycle.h
// Description: DocumentLifecycle Class
//      Author: Ziming Li
//     Created: 2019-10-10
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
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
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

#ifndef BLINKIT_BLINK_DOCUMENT_LIFECYCLE_H
#define BLINKIT_BLINK_DOCUMENT_LIFECYCLE_H

#pragma once

#include "base/macros.h"
#include "third_party/blink/renderer/platform/wtf/allocator.h"

namespace blink {

class DocumentLifecycle
{
    DISALLOW_NEW();
public:
    DocumentLifecycle(void) = default;

    enum LifecycleState {
        kUninitialized,
        kInactive,

        // When the document is active, it traverses these states.

        kVisualUpdatePending,

        kInStyleRecalc,
        kStyleClean,

        kInLayoutSubtreeChange,
        kLayoutSubtreeChangeClean,

        kInPreLayout,
        kInPerformLayout,
        kAfterPerformLayout,
        kLayoutClean,

        kInCompositingUpdate,
        kCompositingInputsClean,
        kCompositingClean,

        // In InPrePaint step, any data needed by painting are prepared.
        // Paint property trees are built and paint invalidations are issued.
        kInPrePaint,
        kPrePaintClean,

        // In InPaint step, paint artifacts are generated and raster invalidations
        // are issued.
        // In SPv2, composited layers are generated/updated.
        kInPaint,
        kPaintClean,

        // Once the document starts shutting down, we cannot return
        // to the style/layout/compositing states.
        kStopping,
        kStopped,
    };
    LifecycleState GetState(void) const { return m_state; }

    bool IsActive(void) const { return kInactive < m_state && m_state < kStopping; }

    void AdvanceTo(LifecycleState nextState);
    void EnsureStateAtMost(LifecycleState);

#ifndef BLINKIT_CRAWLER_ONLY
    bool InDetach(void) const { return 0 != m_detachCount; }
    void IncrementDetachCount(void) { ++m_detachCount; }
    void DecrementDetachCount(void)
    {
        ASSERT(m_detachCount > 0);
        --m_detachCount;
    }

    bool StateAllowsDetach(void) const;
    bool StateAllowsLayoutTreeMutations(void) const
    {
        return m_detachCount > 0 || kInStyleRecalc == m_state || kInLayoutSubtreeChange == m_state;
    }
    bool StateAllowsLayoutTreeNotifications(void) const { return kInLayoutSubtreeChange == m_state; }
#endif
    bool StateAllowsTreeMutations(void) const;
    bool StateTransitionDisallowed(void) const { return m_disallowTransitionCount > 0; }
    void IncrementNoTransitionCount(void) { ++m_disallowTransitionCount; }
    void DecrementNoTransitionCount(void)
    {
        ASSERT(m_disallowTransitionCount > 0);
        --m_disallowTransitionCount;
    }

#ifndef BLINKIT_CRAWLER_ONLY
    bool ThrottlingAllowed(void) const;
    bool LifecyclePostponed(void) const { return m_lifeCyclePostponed; }

    class Scope
    {
        STACK_ALLOCATED();
    public:
        Scope(DocumentLifecycle &lifecycle, LifecycleState finalState) : m_lifecycle(lifecycle), m_finalState(finalState) {}
        ~Scope(void) { m_lifecycle.AdvanceTo(m_finalState); }
    private:
        DocumentLifecycle &m_lifecycle;
        LifecycleState m_finalState;

        DISALLOW_COPY_AND_ASSIGN(Scope);
    };
    // Within this scope, state transitions are not allowed.
    // Any attempts to advance or rewind will result in a DCHECK.
    class DisallowTransitionScope
    {
        STACK_ALLOCATED();
    public:
        explicit DisallowTransitionScope(DocumentLifecycle &documentLifecycle) : m_documentLifecycle(documentLifecycle)
        {
            m_documentLifecycle.IncrementNoTransitionCount();
        }
        ~DisallowTransitionScope(void)
        {
            m_documentLifecycle.DecrementNoTransitionCount();
        }
    private:
        DocumentLifecycle &m_documentLifecycle;
        DISALLOW_COPY_AND_ASSIGN(DisallowTransitionScope);
    };
    class DetachScope
    {
        STACK_ALLOCATED();
    public:
        explicit DetachScope(DocumentLifecycle &documentLifecycle) : m_documentLifecycle(documentLifecycle)
        {
            m_documentLifecycle.IncrementDetachCount();
        }
        ~DetachScope(void)
        {
            m_documentLifecycle.DecrementDetachCount();
        }
    private:
        DocumentLifecycle &m_documentLifecycle;
        DISALLOW_COPY_AND_ASSIGN(DetachScope);
    };
    // Throttling is disabled by default. Instantiating this class allows
    // throttling (e.g., during BeginMainFrame). If a script needs to run inside
    // this scope, DisallowThrottlingScope should be used to let the script
    // perform a synchronous layout if necessary.
    class AllowThrottlingScope
    {
        STACK_ALLOCATED();
    public:
        AllowThrottlingScope(DocumentLifecycle &lifecycle);
        ~AllowThrottlingScope(void);
        DISALLOW_COPY_AND_ASSIGN(AllowThrottlingScope);
    };
#endif

private:
#ifndef NDEBUG
    bool CanAdvanceTo(LifecycleState nextState) const;
    bool CanRewindTo(LifecycleState nextState) const;
#endif

    LifecycleState m_state = kUninitialized;
#ifndef BLINKIT_CRAWLER_ONLY
    int m_detachCount = 0;
    bool m_lifeCyclePostponed = false;
#endif
    int m_disallowTransitionCount = 0;
    bool m_checkNoTransition = false;

    DISALLOW_COPY_AND_ASSIGN(DocumentLifecycle);
};

}  // namespace blink

#endif
