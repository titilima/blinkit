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
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "url/bk_url.h"

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

    void MarkParserBlockingLoadStartTime(void);

    void WatchForLoad(PendingScriptClient *client);
    void StopWatchingForLoad(void);

    virtual bool IsReady(void) const = 0;
    virtual bool IsExternal(void) const = 0;
    virtual bool WasCanceled(void) const = 0;

    virtual bool StartStreamingIfPossible(const std::function<void()> &done) = 0;

    bool IsExternalOrModule(void) const {
        ASSERT(false); // BKTODO:
        return false;
    }
    void Dispose(void);

    virtual void ExecuteScriptBlock(const BlinKit::BkURL &documentURL);
private:

    DISALLOW_COPY_AND_ASSIGN(PendingScript);
};

} // namespace blink

#endif // BLINKIT_BLINK_PENDING_SCRIPT_H
