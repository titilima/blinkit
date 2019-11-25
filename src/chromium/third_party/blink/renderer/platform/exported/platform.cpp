// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: platform.cpp
// Description: Platform Class
//      Author: Ziming Li
//     Created: 2019-09-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2012 Google Inc. All rights reserved.
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

#include "third_party/blink/public/platform/platform.h"

#include "blinkit/blink_impl/thread_impl.h"
#include "third_party/blink/renderer/platform/language.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"

using namespace BlinKit;

namespace blink {

static Platform *g_platform = nullptr;

Platform* Platform::Current(void)
{
    return g_platform;
}

void Platform::AttachMainThread(Thread *thread)
{
    m_threads[thread->ThreadId()] = thread;
}

Thread* Platform::CurrentThread(void)
{
    std::lock_guard<std::mutex> lock(m_lock);
    auto it = m_threads.find(ThreadImpl::CurrentThreadId());
    if (std::end(m_threads) != it)
        return it->second;

    ASSERT(std::end(m_threads) != it);
    return nullptr;
}

void Platform::Initialize(Platform *platform, scheduler::WebThreadScheduler *mainThreadScheduler)
{
    DCHECK(!g_platform);
    DCHECK(platform);
    g_platform = platform;
    InitializeCommon(platform);
}

static void CallOnMainThreadFunction(WTF::MainThreadFunction function, void *context)
{
    assert(false); // BKTODO:
}

void Platform::InitializeCommon(Platform *platform)
{
    WTF::Initialize(CallOnMainThreadFunction);
    InitializePlatformLanguage();
    // BKTODO:
}

} // namespace blink
