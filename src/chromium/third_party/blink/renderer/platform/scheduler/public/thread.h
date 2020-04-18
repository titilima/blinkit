// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: thread.h
// Description: Thread Class
//      Author: Ziming Li
//     Created: 2019-11-01
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2011 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef BLINKIT_BLINK_THREAD_H
#define BLINKIT_BLINK_THREAD_H

#pragma once

#include <cstdint>
#include <memory>

namespace base {
class SingleThreadTaskRunner;
}

namespace blink {

class ThreadScheduler;

// Always an integer value.
typedef uintptr_t PlatformThreadId;

class Thread
{
public:
    virtual ~Thread(void) = default;

    class TaskObserver {
    public:
        virtual ~TaskObserver(void) = default;
        virtual void WillProcessTask(void) = 0;
        virtual void DidProcessTask(void) = 0;
    };

    virtual std::shared_ptr<base::SingleThreadTaskRunner> GetTaskRunner(void) const
    {
        NOTREACHED();
        return nullptr;
    }

    virtual bool IsCurrentThread(void) const = 0;
    virtual PlatformThreadId ThreadId(void) const { return 0; }
};

} // namespace blink

#endif // BLINKIT_BLINK_THREAD_H
