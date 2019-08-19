// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: apple_task_runner.mm
// Description: AppleTaskRunner Class
//      Author: Ziming Li
//     Created: 2019-08-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "apple_task_runner.h"

using namespace blink;
using namespace BlinKit;

@implementation TaskWrapper

- (id)init
{
    if (self = [super init])
    {
        m_taskRunner = nullptr;
        m_task = nullptr;
    }
    return self;
}

+ (id)taskWrapperWith: (TaskRunnerImpl *)taskRunner andTask: (WebTaskRunner::Task *)task
{
    TaskWrapper *ret = [[TaskWrapper alloc] init];
    if (nil == ret)
        return nil;

    ret->m_taskRunner = taskRunner;
    ret->m_task = task;
    return [ret autorelease];
}

- (void)exitRunLoopWithPort: (NSPort *)port
{
    [[NSRunLoop currentRunLoop] removePort: port forMode: NSDefaultRunLoopMode];
}

- (void)fireBy: (NSDate *)date
{
    if (nil != date)
    {
        NSTimeInterval interval = date.timeIntervalSinceNow;
        if (interval > 0)
        {
            [self performSelector: @selector(fireBy:)
                       withObject: nil
                       afterDelay: interval];
            return;
        }
    }

    m_taskRunner->Run(m_task);
}

@end

namespace BlinKit {

AppleTaskRunner::AppleTaskRunner(ThreadImpl &thread) : TaskRunnerImpl(thread), m_nativeThread([NSThread currentThread])
{
    // Nothing
}

WebTaskRunner* AppleTaskRunner::clone(void)
{
    return new AppleTaskRunner(m_thread);
}

void AppleTaskRunner::postDelayedTask(const WebTraceLocation &location, Task *task, double delayMs)
{
    TaskWrapper *taskWrapper = [TaskWrapper taskWrapperWith: this andTask: task];
    [taskWrapper performSelector: @selector(fireBy:)
                        onThread: m_nativeThread
                      withObject: [NSDate dateWithTimeIntervalSinceNow: delayMs / 1000]
                   waitUntilDone: NO];
}

void AppleTaskRunner::postTask(const WebTraceLocation &location, Task *task)
{
    TaskWrapper *taskWrapper = [TaskWrapper taskWrapperWith: this andTask: task];
    [taskWrapper performSelector: @selector(fireBy:)
                        onThread: m_nativeThread
                      withObject: nil
                   waitUntilDone: NO];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<WebTaskRunner> TaskRunnerImpl::CreateInstance(ThreadImpl &thread)
{
    return std::make_unique<AppleTaskRunner>(thread);
}

} // namespace BlinKit
