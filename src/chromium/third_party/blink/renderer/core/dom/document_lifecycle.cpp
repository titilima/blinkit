// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: document_lifecycle.cpp
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

#include "document_lifecycle.h"

#include "third_party/blink/renderer/platform/runtime_enabled_features.h"

namespace blink {

#ifndef BLINKIT_CRAWLER_ONLY
static DocumentLifecycle::DeprecatedTransition *g_deprecatedTransitionStack = nullptr;

DocumentLifecycle::DeprecatedTransition::DeprecatedTransition(LifecycleState from, LifecycleState to)
    : m_previous(g_deprecatedTransitionStack), m_from(from), m_to(to)
{
    g_deprecatedTransitionStack = this;
}

DocumentLifecycle::DeprecatedTransition::~DeprecatedTransition(void)
{
    g_deprecatedTransitionStack = m_previous;
}

// TODO(skyostil): Come up with a better way to store cross-frame lifecycle
// related data to avoid this being a global setting.
static unsigned g_allowThrottlingCount = 0;

DocumentLifecycle::AllowThrottlingScope::AllowThrottlingScope(DocumentLifecycle &lifecycle)
{
    ++g_allowThrottlingCount;
}

DocumentLifecycle::AllowThrottlingScope::~AllowThrottlingScope(void)
{
    ASSERT(g_allowThrottlingCount > 0);
    --g_allowThrottlingCount;
}
#endif

void DocumentLifecycle::AdvanceTo(LifecycleState nextState)
{
    ASSERT(CanAdvanceTo(nextState));
    ASSERT(m_state == nextState || !m_checkNoTransition);
    m_state = nextState;
}

#ifndef NDEBUG
bool DocumentLifecycle::CanAdvanceTo(LifecycleState nextState) const
{
    if (StateTransitionDisallowed())
        return false;

    // We can stop from anywhere.
    if (kStopping == nextState)
        return true;

    switch (m_state)
    {
        case kUninitialized:
            return kInactive == nextState;
        case kInactive:
            if (kStyleClean == nextState)
                return true;
            break;
        case kVisualUpdatePending:
            if (kInPreLayout == nextState)
                return true;
            if (kInStyleRecalc == nextState)
                return true;
            if (kInPerformLayout == nextState)
                return true;
            break;
        case kInStyleRecalc:
            return kStyleClean == nextState;
        case kStyleClean:
            // We can synchronously recalc style.
            if (kInStyleRecalc == nextState)
                return true;
            // We can notify layout objects that subtrees changed.
            if (kInLayoutSubtreeChange == nextState)
                return true;
            // We can synchronously perform layout.
            if (kInPreLayout == nextState)
                return true;
            if (kInPerformLayout == nextState)
                return true;
            // We can redundant arrive in the style clean state.
            if (kStyleClean == nextState)
                return true;
            if (kLayoutClean == nextState)
                return true;
            if (RuntimeEnabledFeatures::SlimmingPaintV2Enabled())
            {
                if (kInPrePaint == nextState)
                    return true;
            }
            else
            {
                if (kInCompositingUpdate == nextState)
                    return true;
            }
            break;
        case kInLayoutSubtreeChange:
            return kLayoutSubtreeChangeClean == nextState;
        case kLayoutSubtreeChangeClean:
            // We can synchronously recalc style.
            if (kInStyleRecalc == nextState)
                return true;
            // We can synchronously perform layout.
            if (kInPreLayout == nextState)
                return true;
            if (kInPerformLayout == nextState)
                return true;
            // Can move back to style clean.
            if (kStyleClean == nextState)
                return true;
            if (kLayoutClean == nextState)
                return true;
            if (RuntimeEnabledFeatures::SlimmingPaintV2Enabled())
            {
                if (kInPrePaint == nextState)
                    return true;
            }
            else
            {
                if (kInCompositingUpdate == nextState)
                    return true;
            }
            break;
        case kInPreLayout:
            if (kInStyleRecalc == nextState)
                return true;
            if (kStyleClean == nextState)
                return true;
            if (kInPreLayout == nextState)
                return true;
            break;
        case kInPerformLayout:
            return kAfterPerformLayout == nextState;
        case kAfterPerformLayout:
            // We can synchronously recompute layout in AfterPerformLayout.
            // FIXME: Ideally, we would unnest this recursion into a loop.
            if (kInPreLayout == nextState)
                return true;
            if (kLayoutClean == nextState)
                return true;
            break;
        case kLayoutClean:
            // We can synchronously recalc style.
            if (kInStyleRecalc == nextState)
                return true;
            // We can synchronously perform layout.
            if (kInPreLayout == nextState)
                return true;
            if (kInPerformLayout == nextState)
                return true;
            // We can redundantly arrive in the layout clean state. This situation
            // can happen when we call layout recursively and we unwind the stack.
            if (kLayoutClean == nextState)
                return true;
            if (kStyleClean == nextState)
                return true;
            if (RuntimeEnabledFeatures::SlimmingPaintV2Enabled())
            {
                if (kInPrePaint == nextState)
                    return true;
            }
            else
            {
                if (kInCompositingUpdate == nextState)
                    return true;
            }
            break;
        case kInCompositingUpdate:
            assert(!RuntimeEnabledFeatures::SlimmingPaintV2Enabled());
            // Once we are in the compositing update, we can either just clean the
            // inputs or do the whole of compositing.
            return kCompositingInputsClean == nextState || kCompositingClean == nextState;
        case kCompositingInputsClean:
            assert(!RuntimeEnabledFeatures::SlimmingPaintV2Enabled());
            // We can return to style re-calc, layout, or the start of compositing.
            if (kInStyleRecalc == nextState)
                return true;
            if (kInPreLayout == nextState)
                return true;
            if (kInCompositingUpdate == nextState)
                return true;
            // Otherwise, we can continue onwards.
            if (kCompositingClean == nextState)
                return true;
            break;
        case kCompositingClean:
            assert(!RuntimeEnabledFeatures::SlimmingPaintV2Enabled());
            if (kInStyleRecalc == nextState)
                return true;
            if (kInPreLayout == nextState)
                return true;
            if (kInCompositingUpdate == nextState)
                return true;
            if (kInPrePaint == nextState)
                return true;
            break;
        case kInPrePaint:
            if (kPrePaintClean == nextState)
                return true;
            break;
        case kPrePaintClean:
            if (kInPaint == nextState)
                return true;
            if (kInStyleRecalc == nextState)
                return true;
            if (kInPreLayout == nextState)
                return true;
            if (!RuntimeEnabledFeatures::SlimmingPaintV2Enabled() && kInCompositingUpdate == nextState)
                return true;
            if (kInPrePaint == nextState)
                return true;
            break;
        case kInPaint:
            if (kPaintClean == nextState)
                return true;
            break;
        case kPaintClean:
            if (kInStyleRecalc == nextState)
                return true;
            if (kInPreLayout == nextState)
                return true;
            if (!RuntimeEnabledFeatures::SlimmingPaintV2Enabled() && kInCompositingUpdate == nextState)
                return true;
            if (kInPrePaint == nextState)
                return true;
            if (kInPaint == nextState)
                return true;
            break;
        case kStopping:
            return kStopped == nextState;
        case kStopped:
            return false;
    }
    return false;
}

bool DocumentLifecycle::CanRewindTo(LifecycleState nextState) const
{
    if (StateTransitionDisallowed())
        return false;

#if 0 // BKTODO: Check if necessary
    // This transition is bogus, but we've whitelisted it anyway.
    if (g_deprecated_transition_stack &&
        state_ == g_deprecated_transition_stack->From() &&
        next_state == g_deprecated_transition_stack->To())
        return true;
#endif
    switch (m_state)
    {
        case kStyleClean:
        case kLayoutSubtreeChangeClean:
        case kAfterPerformLayout:
        case kLayoutClean:
        case kCompositingInputsClean:
        case kCompositingClean:
        case kPrePaintClean:
        case kPaintClean:
            return true;
    }
    return false;
}
#endif // NDEBUG

void DocumentLifecycle::EnsureStateAtMost(LifecycleState state)
{
    ASSERT(kVisualUpdatePending == state || kStyleClean == state || kLayoutClean == state);
    if (m_state <= state)
        return;
    ASSERT(CanRewindTo(state));
    ASSERT(m_state == state || !m_checkNoTransition);
    m_state = state;
}

#ifndef BLINKIT_CRAWLER_ONLY
bool DocumentLifecycle::StateAllowsDetach(void) const
{
    switch (m_state)
    {
        case kVisualUpdatePending:
        case kInStyleRecalc:
        case kStyleClean:
        case kLayoutSubtreeChangeClean:
        case kInPreLayout:
        case kLayoutClean:
        case kCompositingInputsClean:
        case kCompositingClean:
        case kPrePaintClean:
        case kPaintClean:
        case kStopping:
            return true;
    }
    return false;
}
#endif

bool DocumentLifecycle::StateAllowsTreeMutations(void) const
{
    switch (m_state)
    {
        case kInStyleRecalc:
        case kInPerformLayout:
        case kInCompositingUpdate:
        case kInPrePaint:
        case kInPaint:
            return false;
        default:
            return true;
    }
}

#ifndef BLINKIT_CRAWLER_ONLY
bool DocumentLifecycle::ThrottlingAllowed(void) const
{
    return g_allowThrottlingCount > 0;
}
#endif

}  // namespace blink
