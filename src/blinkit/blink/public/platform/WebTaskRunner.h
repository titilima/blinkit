// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: WebTaskRunner.h
// Description: WebTaskRunner Class
//      Author: Ziming Li
//     Created: 2021-06-26
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WebTaskRunner_h
#define WebTaskRunner_h

#include <functional>
#include "WebCommon.h"

namespace blink {

class WebTraceLocation;

// The blink representation of a chromium SingleThreadTaskRunner.
class BLINK_PLATFORM_EXPORT WebTaskRunner {
public:
    virtual ~WebTaskRunner() {}

    class BLINK_PLATFORM_EXPORT Task {
    public:
        virtual ~Task() { }
        virtual void run() = 0;
    };

    // Schedule a task to be run on the the associated WebThread.
    // Takes ownership of |Task|. Can be called from any thread.
    virtual void postTask(const WebTraceLocation&, Task*) = 0;

    // Schedule a task to be run after |delayMs| on the the associated WebThread.
    // Takes ownership of |Task|. Can be called from any thread.
    virtual void postDelayedTask(const WebTraceLocation&, Task*, double delayMs) = 0;

    // Returns a clone of the WebTaskRunner.
    // BKTODO: virtual WebTaskRunner* clone() = 0;

    // Helpers for posting bound functions as tasks.
    typedef std::function<void()> ClosureTask;

    void postTask(const WebTraceLocation&, ClosureTask &&task);
#if 0 // BKTODO:
    // TODO(alexclarke): Remove this when possible.
    void postDelayedTask(const WebTraceLocation&, PassOwnPtr<ClosureTask>, long long delayMs);
    void postDelayedTask(const WebTraceLocation&, PassOwnPtr<ClosureTask>, double delayMs);

    PassOwnPtr<WebTaskRunner> adoptClone()
    {
        return adoptPtr(clone());
    }
#endif
};

} // namespace blink

#endif // WebTaskRunner_h
