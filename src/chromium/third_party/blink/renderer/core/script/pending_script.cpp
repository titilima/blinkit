// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: pending_script.cpp
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

#include "pending_script.h"

#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/script/ignore_destructive_write_count_incrementer.h"

namespace blink {

PendingScript::PendingScript(ScriptElementBase *element, const TextPosition &startingPosition)
    : m_element(element)
    , m_startingPosition(startingPosition)
    , m_createdDuringDocumentWrite(element->GetDocument().IsInDocumentWrite())
{
}

void PendingScript::Dispose(void)
{
    StopWatchingForLoad();
    ASSERT(nullptr == Client());
    ASSERT(!IsWatchingForLoad());

    m_startingPosition = TextPosition::BelowRangePosition();

    DisposeInternal();
    m_element = nullptr;
}

void PendingScript::ExecuteScriptBlock(const GURL &documentURL)
{
    Document *contextDocument = m_element->GetDocument().ContextDocument();
    if (nullptr == contextDocument)
    {
        Dispose();
        return;
    }

    LocalFrame *frame = contextDocument->GetFrame();
    if (nullptr == frame)
    {
        Dispose();
        return;
    }

    std::unique_ptr<Script> script = GetSource(documentURL);
    const bool wasCanceled = WasCanceled();
    const bool isExternal = IsExternal();
    const bool createdDuringDocumentWrite = WasCreatedDuringDocumentWrite();
    const bool isControlledByScriptRunner = IsControlledByScriptRunner();
    ScriptElementBase *element = m_element;
    Dispose();

    // ExecuteScriptBlockInternal() is split just in order to prevent accidential
    // access to |this| after Dispose().
    ExecuteScriptBlockInternal(script.get(), element, wasCanceled, isExternal, createdDuringDocumentWrite,
        isControlledByScriptRunner);
}

void PendingScript::ExecuteScriptBlockInternal(
    Script *script,
    ScriptElementBase *element,
    bool wasCanceled,
    bool isExternal,
    bool createdDuringDocumentWrite,
    bool isControlledByScriptRunner)
{
    Document &elementDocument = element->GetDocument();
    Document *contextDocument = elementDocument.ContextDocument();

    // <spec step="2">If the script's script is null, fire an event named error at
    // the element, and return.</spec>
    if (nullptr == script)
    {
        element->DispatchErrorEvent();
        return;
    }

    if (wasCanceled)
        return;

    {
        const bool isImportedScript = contextDocument != &elementDocument;

        // <spec step="3">If the script is from an external file, or the script's
        // type is "module", ...</spec>
        const bool needsIncrement = isExternal || script->GetScriptType() == ScriptType::kModule || isImportedScript;
        // <spec step="3">... then increment the ignore-destructive-writes counter
        // of the script element's node document. Let neutralized doc be that
        // Document.</spec>
        IgnoreDestructiveWriteCountIncrementer incrementer(needsIncrement ? contextDocument : nullptr);

        // <spec step="4">Let old script element be the value to which the script
        // element's node document's currentScript object was most recently
        // set.</spec>
        //
        // This is implemented as push/popCurrentScript().

        // <spec step="5">Switch on the script's type:</spec>
        //
        // Step 5.A. "classic" [spec text]
        //
        // <spec step="5.A.1">If the script element's root is not a shadow root,
        // then set the script element's node document's currentScript attribute to
        // the script element. Otherwise, set it to null.</spec>
        //
        // Step 5.B. "module" [spec text]
        //
        // <spec step="5.B.1">Set the script element's node document's currentScript
        // attribute to null.</spec>
        ScriptElementBase *currentScript = nullptr;
        if (script->GetScriptType() == ScriptType::kClassic)
            currentScript = element;
        contextDocument->PushCurrentScript(currentScript);

        // Step 5.A. "classic" [spec text]
        //
        // <spec step="5.A.2">Run the classic script given by the script's
        // script.</spec>
        //
        // Note: This is where the script is compiled and actually executed.
        //
        // Step 5.B. "module" [spec text]
        //
        // <spec step="5.B.2">Run the module script given by the script's
        // script.</spec>
        script->RunScript(contextDocument->GetFrame());

        // <spec step="6">Set the script element's node document's currentScript
        // attribute to old script element.</spec>
        contextDocument->PopCurrentScript(currentScript);

        // <spec step="7">Decrement the ignore-destructive-writes counter of
        // neutralized doc, if it was incremented in the earlier step.</spec>
        //
        // Implemented as the scope out of IgnoreDestructiveWriteCountIncrementer.
    }

    // <spec step="8">If the script is from an external file, then fire an event
    // named load at the script element.</spec>
    if (isExternal)
        element->DispatchLoadEvent();
}

ScriptElementBase* PendingScript::GetElement(void) const
{
    // As mentioned in the comment at |m_element| declaration,
    // |m_element|  must point to the corresponding ScriptLoader's
    // client.
    ASSERT(m_element);
    return m_element.Get();
}

bool PendingScript::IsControlledByScriptRunner(void) const
{
    switch (m_schedulingType)
    {
        case ScriptSchedulingType::kNotSet:
            NOTREACHED();
            return false;

        case ScriptSchedulingType::kDefer:
        case ScriptSchedulingType::kParserBlocking:
        case ScriptSchedulingType::kParserBlockingInline:
        case ScriptSchedulingType::kImmediate:
            return false;

        case ScriptSchedulingType::kInOrder:
        case ScriptSchedulingType::kAsync:
            return true;
    }
    NOTREACHED();
    return false;
}

void PendingScript::MarkParserBlockingLoadStartTime(void)
{
    // BKTODO: Check if necessary.
}

void PendingScript::PendingScriptFinished(void)
{
    if (nullptr != m_client)
        m_client->PendingScriptFinished(this);
}

void PendingScript::StopWatchingForLoad(void)
{
    if (!IsWatchingForLoad())
        return;
    CheckState();
    ASSERT(IsExternalOrModule());
    m_client = nullptr;
}

void PendingScript::WatchForLoad(PendingScriptClient *client)
{
    CheckState();

    ASSERT(!IsWatchingForLoad());
    ASSERT(nullptr != client);
    // addClient() will call streamingFinished() if the load is complete. Callers
    // who do not expect to be re-entered from this call should not call
    // watchForLoad for a PendingScript which isReady. We also need to set
    // m_watchingForLoad early, since addClient() can result in calling
    // notifyFinished and further stopWatchingForLoad().
    m_client = client;
    if (IsReady())
        PendingScriptFinished();
}

}  // namespace blink
