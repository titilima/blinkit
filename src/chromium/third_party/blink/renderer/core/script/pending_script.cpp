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

using namespace BlinKit;

namespace blink {

void PendingScript::Dispose(void)
{
    ASSERT(false); // BKTODO:
}

void PendingScript::ExecuteScriptBlock(const BkURL &documentURL)
{
    ASSERT(false); // BKTODO:
}

void PendingScript::MarkParserBlockingLoadStartTime(void)
{
    ASSERT(false); // BKTODO:
}

void PendingScript::StopWatchingForLoad(void)
{
    ASSERT(false); // BKTODO:
}

void PendingScript::WatchForLoad(PendingScriptClient *client)
{
    ASSERT(false); // BKTODO:
}

}  // namespace blink
