// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: apple_thread.mm
// Description: AppleThread Class
//      Author: Ziming Li
//     Created: 2019-08-18
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "apple_thread.h"

using namespace blink;

namespace BlinKit {

struct AppleThread::CreateData {
    AppleThread *thread;
    const char *name;
    NSCondition *cond;
};

AppleThread::AppleThread(void) {}

AppleThread::~AppleThread(void)
{
    [m_nativeThread cancel];
}

void AppleThread::Initialize(CreateData &cd)
{
    id threadProc = ^ {
        cd.thread->ApplyThreadId(ThreadImpl::CurrentThreadId());
        [cd.cond signal];

        [[NSRunLoop currentRunLoop] run];
    };
    m_nativeThread = [[NSThread alloc] initWithBlock: threadProc];
    [m_nativeThread setName: [NSString stringWithUTF8String: cd.name]];
    [m_nativeThread start];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ThreadImpl* ThreadImpl::CreateInstance(const char *name)
{
    AppleThread::CreateData cd;
    cd.thread = new AppleThread;
    cd.name = name;
    cd.cond = [[NSCondition alloc] init];

    cd.thread->Initialize(cd);

    [cd.cond wait];
    [cd.cond release];

    return cd.thread;
}

PlatformThreadId ThreadImpl::CurrentThreadId(void)
{
    return pthread_mach_thread_np(pthread_self());
}

} // namespace BlinKit
