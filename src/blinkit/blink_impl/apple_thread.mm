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

#include "blink_impl/apple_task_runner.h"

using namespace blink;

namespace BlinKit {

struct AppleThread::CreateData {
    AppleThread *thread;
    const char *name;
    base::scoped_nsobject<NSCondition> cond;
};

AppleThread::AppleThread(void)
    : m_nativeThread([NSThread currentThread], base::scoped_policy::RETAIN)
    , m_portForRunloop(nil)
{
    // Nothing
}

AppleThread::~AppleThread(void)
{
    m_keepRunning = false;

    base::scoped_nsobject<TaskWrapper> taskWrapper = [[TaskWrapper alloc] init];
    [taskWrapper performSelector: @selector(exitRunLoopWithPort:)
                        onThread: m_nativeThread
                      withObject: m_portForRunloop
                   waitUntilDone: YES];

    m_portForRunloop = nil;
}

void AppleThread::Initialize(CreateData &cd)
{
#ifdef _DEBUG
    m_name = cd.name;
#endif
    m_portForRunloop = [NSPort port];
    [[NSRunLoop currentRunLoop] addPort: m_portForRunloop
                                forMode: NSDefaultRunLoopMode];
}

void AppleThread::Run(void)
{
    do {
        [[NSRunLoop currentRunLoop] runMode: NSDefaultRunLoopMode
                                 beforeDate: [NSDate distantFuture]];
    } while (m_keepRunning);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ThreadImpl* ThreadImpl::CreateInstance(const char *name)
{
    __block AppleThread::CreateData cd;
    cd.thread = nullptr;
    cd.name = name;
    cd.cond = [[NSCondition alloc] init];

    id threadProc = ^ {
        cd.thread = new AppleThread; // Create instance in the new thread
                                     // to initialize the task runner
                                     // in the corresponding thread context.
        cd.thread->Initialize(cd);
        [cd.cond signal];
        
        cd.thread->Run();
    };
    base::scoped_nsobject<NSThread> nativeThread = [[NSThread alloc] initWithBlock: threadProc];
    [nativeThread setName: [NSString stringWithUTF8String: cd.name]];
    [nativeThread start];

    [cd.cond wait];
    return cd.thread;
}

PlatformThreadId ThreadImpl::CurrentThreadId(void)
{
    return pthread_mach_thread_np(pthread_self());
}

} // namespace BlinKit
