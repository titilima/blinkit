// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: script_runner.cpp
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

#include "script_runner.h"

#include <algorithm>
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/script/script_loader.h"
#include "third_party/blink/renderer/platform/runtime_enabled_features.h"

namespace blink {

ScriptRunner::ScriptRunner(Document *document)
    : m_document(document)
    , m_taskRunner(document->GetTaskRunner(TaskType::kNetworking))
{
    ASSERT(nullptr != document);
}

bool ScriptRunner::DoTryStream(PendingScript *pendingScript)
{
    // Checks that all callers should have already done.
    ASSERT(!m_isSuspended);
    ASSERT(nullptr != pendingScript);

    // Currently, we stream only async scripts in this function.
    // Note: HTMLParserScriptRunner kicks streaming for deferred or blocking
    // scripts.
    ASSERT(std::end(m_pendingAsyncScripts) != m_pendingAsyncScripts.find(pendingScript)); // BKTODO: async_scripts_to_execute_soon_

    if (nullptr == pendingScript)
        return false;

    ASSERT(pendingScript->GetSchedulingType() == ScriptSchedulingType::kAsync);

#ifndef NDEBUG
    bool wasAlreadyStreaming = pendingScript->IsCurrentlyStreaming();
#endif

    const std::function<void()> done = std::bind(&ScriptRunner::NotifyScriptStreamerFinished, this);
    bool success = pendingScript->StartStreamingIfPossible(done);
#ifndef NDEBUG
    if (wasAlreadyStreaming)
        ASSERT(!success);
    else
        ASSERT(success == pendingScript->IsCurrentlyStreaming());
    if (success)
        ++m_numberOfExtraTasks;
#endif
    return success;
}

bool ScriptRunner::ExecuteAsyncTask(void)
{
    // Find an async script loader which is not currently streaming.
    auto it = std::find_if(m_asyncScriptsToExecuteSoon.begin(), m_asyncScriptsToExecuteSoon.end(),
        [](std::shared_ptr<PendingScript> &pendingScript)
        {
            ASSERT(pendingScript);
            return !pendingScript->IsCurrentlyStreaming();
        }
    );
    if (std::end(m_asyncScriptsToExecuteSoon) == it)
        return false;

    // Remove the async script loader from the ready-to-exec set and execute.
    std::shared_ptr<PendingScript> pendingScript(*it);
    m_asyncScriptsToExecuteSoon.erase(it);

    // Async scripts queue should not contain any in-order script.
    ASSERT(pendingScript->GetSchedulingType() == ScriptSchedulingType::kAsync);

    pendingScript->ExecuteScriptBlock(GURL());

    m_document->DecrementLoadEventDelayCount();
    return true;
}

bool ScriptRunner::ExecuteInOrderTask(void)
{
    if (m_inOrderScriptsToExecuteSoon.empty())
        return false;

    std::shared_ptr<PendingScript> pendingScript(m_inOrderScriptsToExecuteSoon.front());
    m_inOrderScriptsToExecuteSoon.pop_front();
    ASSERT(pendingScript);
    // In-order scripts queue should not contain any async script.
    ASSERT(pendingScript->GetSchedulingType() == ScriptSchedulingType::kInOrder);

    pendingScript->ExecuteScriptBlock(GURL());

    m_document->DecrementLoadEventDelayCount();
    return true;
}

void ScriptRunner::ExecuteTask(void)
{
    if (m_isSuspended)
        return;

    if (ExecuteAsyncTask())
        return;

    if (ExecuteInOrderTask())
        return;

#ifndef NDEBUG
    // Extra tasks should be posted only when we resume after suspending,
    // or when we stream a script. These should all be accounted for in
    // number_of_extra_tasks_.
    ASSERT(m_numberOfExtraTasks-- > 0);
#endif
}

void ScriptRunner::NotifyScriptReady(PendingScript *pendingScript)
{
    ASSERT(nullptr != pendingScript);
    switch (pendingScript->GetSchedulingType())
    {
        case ScriptSchedulingType::kAsync:
        {
            auto it = m_pendingAsyncScripts.find(pendingScript);
            ASSERT(std::end(m_pendingAsyncScripts) != it);

            m_asyncScriptsToExecuteSoon.push_back(it->second);
            m_pendingAsyncScripts.erase(it);

            PostTask(FROM_HERE);
            TryStreamAny();
            break;
        }
        case ScriptSchedulingType::kInOrder:
            ASSERT(false); // BKTODO:
#if 0
            SECURITY_CHECK(number_of_in_order_scripts_with_pending_notification_ > 0);
            number_of_in_order_scripts_with_pending_notification_--;

            ScheduleReadyInOrderScripts();
#endif

            break;
        default:
            NOTREACHED();
            break;
    }
}

void ScriptRunner::NotifyScriptStreamerFinished(void)
{
    PostTask(FROM_HERE);
    TryStreamAny();
}

void ScriptRunner::PostTask(const base::Location &webTraceLocation)
{
    const auto task = std::bind(&ScriptRunner::ExecuteTask, this);
    m_taskRunner->PostTask(webTraceLocation, task);
}

void ScriptRunner::QueueScriptForExecution(std::shared_ptr<PendingScript> &pendingScript)
{
    ASSERT(nullptr != pendingScript);
    m_document->IncrementLoadEventDelayCount();
    switch (pendingScript->GetSchedulingType())
    {
        case ScriptSchedulingType::kAsync:
            m_pendingAsyncScripts[pendingScript.get()] = pendingScript;
            TryStream(pendingScript.get());
            break;

        case ScriptSchedulingType::kInOrder:
            ASSERT(false); // BKTODO:
#if 0
            pending_in_order_scripts_.push_back(pending_script);
            number_of_in_order_scripts_with_pending_notification_++;
#endif
            break;

        default:
            NOTREACHED();
            break;
    }
}

void ScriptRunner::TryStream(PendingScript *pendingScript)
{
    if (!m_isSuspended)
        DoTryStream(pendingScript);
}

void ScriptRunner::TryStreamAny(void)
{
    if (m_isSuspended)
        return;

    if (!RuntimeEnabledFeatures::WorkStealingInScriptRunnerEnabled())
        return;

    // Look through async_scripts_to_execute_soon_, and stream any one of them.
    for (auto pendingScript : m_asyncScriptsToExecuteSoon)
    {
        if (DoTryStream(pendingScript.get()))
            return;
    }
}

} // namespace blink
