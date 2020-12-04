// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: platform.h
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

#ifndef BLINKIT_BLINK_PLATFORM_H
#define BLINKIT_BLINK_PLATFORM_H

#pragma once

#include <mutex>
#include <unordered_map>
#include "third_party/blink/renderer/platform/scheduler/public/thread.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

namespace base {
class SingleThreadTaskRunner;
}

namespace blink {

class WebThemeEngine;
class WebURLLoader;

namespace scheduler {
class WebThreadScheduler;
}

class Platform
{
public:
    static void Initialize(Platform *platform, scheduler::WebThreadScheduler *mainThreadScheduler);
    static Platform* Current(void);

    Thread* CurrentThread(void);
    constexpr bool IsLowEndDevice(void) const { return false; }

    virtual WTF::String DefaultLocale(void) { return String("en-US"); }
    virtual std::unique_ptr<WebURLLoader> CreateURLLoader(const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner);

#ifndef BLINKIT_CRAWLER_ONLY
    // Returns a blob of data corresponding to the named resource.
    virtual std::string GetDataResource(const char *name) { return std::string(); }
    // May return null on some platforms.
    virtual WebThemeEngine* ThemeEngine(void) { return nullptr; }
#endif
protected:
    void AttachMainThread(Thread *thread);

    std::mutex m_lock;
private:
    static void InitializeCommon(Platform *platform);

    std::unordered_map<PlatformThreadId, Thread *> m_threads;
};

} // namespace blink

#endif // BLINKIT_BLINK_PLATFORM_H
