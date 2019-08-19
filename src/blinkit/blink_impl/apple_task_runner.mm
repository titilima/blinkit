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

#include "blink_impl/thread_impl.h"

using namespace blink;

@interface NativeTaskRunner : NSObject
{
@private
    BlinKit::TaskRunnerImpl *m_taskRunner;
    blink::WebTaskRunner::Task *m_task;
}

- (id)initWith: (BlinKit::TaskRunnerImpl *)taskRunner andTask: (WebTaskRunner::Task *)task;

- (void)fireBy: (NSDate *)date;

@end

@implementation NativeTaskRunner

- (id)initWith: (BlinKit::TaskRunnerImpl *)taskRunner andTask: (WebTaskRunner::Task *)task
{
    if (self = [super init])
    {
        m_taskRunner = taskRunner;
        m_task = task;
    }
    return self;
}

- (void)fireBy: (NSDate *)date
{
    NSTimeInterval interval = 0;
    if (nil != date)
        interval = date.timeIntervalSinceNow;

    if (interval <= 0)
    {
        m_taskRunner->Run(m_task);
    }
    else
    {
        id timerProc = ^(NSTimer *) {
            m_taskRunner->Run(m_task);
        };
        NSTimer *timer = [NSTimer timerWithTimeInterval: interval repeats: NO block: timerProc];
        [[NSRunLoop currentRunLoop] addTimer: timer forMode: NSRunLoopCommonModes];
    }

    [self release];
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
    NativeTaskRunner *taskRunner = [[NativeTaskRunner alloc] initWith: this andTask: task];

    NSDate *timeToFire = [[NSDate alloc] initWithTimeIntervalSinceNow: delayMs / 1000];
    [taskRunner performSelector: @selector(fireBy:)
                       onThread: m_nativeThread
                     withObject: timeToFire
                  waitUntilDone: NO];
    [timeToFire release];
}

void AppleTaskRunner::postTask(const WebTraceLocation &location, Task *task)
{
    NativeTaskRunner *taskRunner = [[NativeTaskRunner alloc] initWith: this andTask: task];
    [taskRunner performSelector: @selector(fireBy:)
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
