// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: navigation_scheduler.h
// Description: NavigationScheduler Class
//      Author: Ziming Li
//     Created: 2019-10-08
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
 * Copyright (C) 2008, 2009 Torch Mobile Inc. All rights reserved.
 * (http://www.torchmobile.com/)
 * Copyright (C) 2009 Adam Barth. All rights reserved.
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
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
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

#ifndef BLINKIT_BLINK_NAVIGATION_SCHEDULER_H
#define BLINKIT_BLINK_NAVIGATION_SCHEDULER_H

#pragma once

#include "base/memory/ptr_util.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/web_task_runner.h"

namespace blink {

class LocalFrame;
class ScheduledNavigation;

class NavigationScheduler final : public GarbageCollectedFinalized<NavigationScheduler>
{
public:
    static std::unique_ptr<NavigationScheduler> Create(LocalFrame *frame)
    {
        return base::WrapUnique(new NavigationScheduler(frame));
    }
    ~NavigationScheduler(void);

    bool LocationChangePending(void);

    void Cancel(void);
private:
    explicit NavigationScheduler(LocalFrame *frame);

    Member<LocalFrame> m_frame;
    TaskHandle m_navigateTaskHandle;
    std::unique_ptr<ScheduledNavigation> m_redirect;

    DISALLOW_COPY_AND_ASSIGN(NavigationScheduler);
};

}  // namespace blink

#endif  // BLINKIT_BLINK_NAVIGATION_SCHEDULER_H
