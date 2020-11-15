// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: script_runner.h
// Description: ScriptRunner Class
//      Author: Ziming Li
//     Created: 2020-04-03
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2010 Google, Inc. All Rights Reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef BLINKIT_BLINK_SCRIPT_RUNNER_H
#define BLINKIT_BLINK_SCRIPT_RUNNER_H

#pragma once

#include <deque>
#include <unordered_map>
#include "base/location.h"
#include "base/macros.h"
#include "base/memory/ptr_util.h"
#include "base/single_thread_task_runner.h"
#include "third_party/blink/renderer/platform/heap/handle.h"

namespace blink {

class Document;
class PendingScript;
class ScriptLoader;

class ScriptRunner final
{
public:
    static std::unique_ptr<ScriptRunner> Create(Document *document)
    {
        return base::WrapUnique(new ScriptRunner(document));
    }

    void QueueScriptForExecution(std::shared_ptr<PendingScript> &pendingScript);
    void NotifyScriptReady(PendingScript *pendingScript);
    void NotifyScriptStreamerFinished(void);
private:
    explicit ScriptRunner(Document *document);

    void PostTask(const base::Location &webTraceLocation);

    void ExecuteTask(void);
    // Execute the first task in in_order_scripts_to_execute_soon_.
    // Returns true if task was run, and false otherwise.
    bool ExecuteInOrderTask(void);
    // Execute any task in async_scripts_to_execute_soon_.
    // Returns true if task was run, and false otherwise.
    bool ExecuteAsyncTask(void);

    // Try to start streaming a specific script or any available script.
    void TryStream(PendingScript *pendingScript);
    void TryStreamAny(void);
    bool DoTryStream(PendingScript *pendingScript);  // Implementation for both Try* methods.

    Member<Document> m_document;

    std::unordered_map<PendingScript *, std::shared_ptr<PendingScript>> m_pendingAsyncScripts;

    // http://www.whatwg.org/specs/web-apps/current-work/#set-of-scripts-that-will-execute-as-soon-as-possible
    std::deque<std::shared_ptr<PendingScript>> m_asyncScriptsToExecuteSoon;
    std::deque<std::shared_ptr<PendingScript>> m_inOrderScriptsToExecuteSoon;

    std::shared_ptr<base::SingleThreadTaskRunner> m_taskRunner;

    bool m_isSuspended = false;

#ifndef NDEBUG
    // We expect to have one posted task in flight for each script in either
    // .._to_be_executed_soon_ queue. This invariant will be temporarily violated
    // when the ScriptRunner is suspended, or when we take a Script out the
    // async_scripts_to_be_executed_soon_ queue for streaming. We'll use this
    // variable to account & check this invariant for debugging.
    int m_numberOfExtraTasks = 0;
#endif
    DISALLOW_COPY_AND_ASSIGN(ScriptRunner);
};

} // namespace blink

#endif // BLINKIT_BLINK_SCRIPT_RUNNER_H
