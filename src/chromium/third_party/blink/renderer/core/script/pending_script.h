// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: pending_script.h
// Description: PendingScript Class
//      Author: Ziming Li
//     Created: 2019-10-27
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
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

#ifndef BLINKIT_BLINK_PENDING_SCRIPT_H
#define BLINKIT_BLINK_PENDING_SCRIPT_H

#pragma once

#include <functional>
#include "base/macros.h"
#include "third_party/blink/renderer/core/script/script.h"
#include "third_party/blink/renderer/core/script/script_element_base.h"
#include "third_party/blink/renderer/core/script/script_scheduling_type.h"
#include "third_party/blink/renderer/platform/heap/handle.h"

namespace blink {

class PendingScript;

class PendingScriptClient : public GarbageCollectedMixin
{
public:
    virtual ~PendingScriptClient(void) = default;

    virtual void PendingScriptFinished(PendingScript *pendingScript) = 0;
};

class PendingScript : public GarbageCollectedFinalized<PendingScript>
{
public:
    virtual ~PendingScript(void) = default;

    TextPosition StartingPosition(void) const { return m_startingPosition; }
    void MarkParserBlockingLoadStartTime(void);

    void WatchForLoad(PendingScriptClient *client);
    void StopWatchingForLoad(void);
    void PendingScriptFinished(void);

    ScriptElementBase* GetElement(void) const;

    ScriptSchedulingType GetSchedulingType(void) const
    {
        ASSERT(ScriptSchedulingType::kNotSet != m_schedulingType);
        return m_schedulingType;
    }
    bool IsControlledByScriptRunner(void) const;
    void SetSchedulingType(ScriptSchedulingType schedulingType)
    {
        ASSERT(m_schedulingType == ScriptSchedulingType::kNotSet);
        m_schedulingType = schedulingType;
    }

    // Returns nullptr when "script's script is null", i.e. an error occurred.
    virtual std::unique_ptr<Script> GetSource(const GURL &documentURL) const = 0;

    virtual ScriptType GetScriptType(void) const = 0;
    virtual bool IsReady(void) const = 0;
    virtual bool IsExternal(void) const = 0;
    virtual bool WasCanceled(void) const = 0;

    // Support for script streaming.
    virtual bool StartStreamingIfPossible(const std::function<void()> &done) = 0;
    virtual bool IsCurrentlyStreaming(void) const = 0;

    bool IsExternalOrModule(void) const { return IsExternal() || GetScriptType() == ScriptType::kModule; }
    void Dispose(void);

    bool WasCreatedDuringDocumentWrite(void) const { return m_createdDuringDocumentWrite; }

    virtual void ExecuteScriptBlock(const GURL &documentURL);
protected:
    PendingScript(ScriptElementBase *element, const TextPosition &startingPosition);

    PendingScriptClient* Client(void) { return m_client; }
    bool IsWatchingForLoad(void) const { return nullptr != m_client; }

    virtual void DisposeInternal(void) = 0;
    virtual void CheckState(void) const = 0;
private:
    static void ExecuteScriptBlockInternal(Script *script, ScriptElementBase *element, bool wasCanceled,
        bool isExternal, bool createdDuringDocumentWrite, bool isControlledByScriptRunner);

    // |m_element| must points to the corresponding ScriptLoader's
    // ScriptElementBase and thus must be non-null before dispose() is called
    // (except for unit tests).
    Member<ScriptElementBase> m_element;

    TextPosition m_startingPosition;  // Only used for inline script tags.

    ScriptSchedulingType m_schedulingType = ScriptSchedulingType::kNotSet;

    Member<PendingScriptClient> m_client;

    const bool m_createdDuringDocumentWrite;

    DISALLOW_COPY_AND_ASSIGN(PendingScript);
};

} // namespace blink

#endif // BLINKIT_BLINK_PENDING_SCRIPT_H
