// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: context_lifecycle_observer.h
// Description: ContextLifecycleObserver Class
//      Author: Ziming Li
//     Created: 2019-12-24
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2008 Apple Inc. All Rights Reserved.
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

#ifndef BLINKIT_BLINK_CONTEXT_LIFECYCLE_OBSERVER_H
#define BLINKIT_BLINK_CONTEXT_LIFECYCLE_OBSERVER_H

#pragma once

#include "third_party/blink/renderer/core/execution_context/execution_context.h"
#include "third_party/blink/renderer/platform/lifecycle_observer.h"

namespace blink {

class ExecutionContext;

// ContextLifecycleObserver provides an additional ContextDestroyed() hook
// to execute cleanup code when a context is shut down (e.g. for a document,
// at navigation or frame detach -- not when its destructor runs).
//
// Execution context associated objects which have ongoing activity,
// registration with objects which outlive the context, or resources which
// should be promptly released, should consider deriving from
// ContextLifecycleObserver. As a rule of thumb: if the destructor contains
// non-trivial logic, that logic may belong in ContextDestroyed() instead.
//
// If there is ongoing activity associated with the object, consider whether it
// needs to be paused when execution is suspended (see PausableObject).
//
// If none of the above applies, prefer the simpler ContextClient.
class ContextLifecycleObserver : public LifecycleObserver<ExecutionContext, ContextLifecycleObserver>
{
public:
    virtual void ContextDestroyed(ExecutionContext *executionContext) {}

    // Returns the execution context until it is detached.
    // From then on, returns null instead.
    ExecutionContext* GetExecutionContext(void) const { return LifecycleContext(); }

    enum Type {
        kGenericType,
        kPausableObjectType,
    };
    Type ObserverType(void) const { return m_observerType; }

protected:
    explicit ContextLifecycleObserver(ExecutionContext *executionContext, Type type = kGenericType)
        : LifecycleObserver(executionContext), m_observerType(type) {}

private:
    Type m_observerType;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_DOM_CONTEXT_LIFECYCLE_OBSERVER_H_
