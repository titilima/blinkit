// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: pausable_object.cc
// Description: PausableObject Class
//      Author: Ziming Li
//     Created: 2020-09-25
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
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

#include "third_party/blink/renderer/core/dom/pausable_object.h"

#include "third_party/blink/renderer/core/execution_context/execution_context.h"

namespace blink {

PausableObject::PausableObject(ExecutionContext* execution_context)
    : ContextLifecycleObserver(execution_context, kPausableObjectType)
#if DCHECK_IS_ON()
      ,
      pause_if_needed_called_(false)
#endif
{
  ASSERT(false); // BKTODO: DCHECK(!execution_context || execution_context->IsContextThread());
}

PausableObject::~PausableObject() {
#if DCHECK_IS_ON()
  DCHECK(pause_if_needed_called_);
#endif
}

void PausableObject::PauseIfNeeded() {
#if DCHECK_IS_ON()
  DCHECK(!pause_if_needed_called_);
  pause_if_needed_called_ = true;
#endif
  if (ExecutionContext* context = GetExecutionContext())
    ASSERT(false); // BKTODO: context->PausePausableObjectIfNeeded(this);
}

void PausableObject::Pause() {}

void PausableObject::Unpause() {}

void PausableObject::DidMoveToNewExecutionContext(ExecutionContext* context) {
  SetContext(context);

  if (context->IsContextDestroyed()) {
    ContextDestroyed(context);
    return;
  }

  ASSERT(false); // BKTODO:
#if 0
  if (context->IsContextPaused()) {
    Pause();
    return;
  }
#endif

  Unpause();
}

}  // namespace blink
