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
using namespace BlinKit;

@interface NativeTaskRunner : NSObject
{
@private
    TaskRunnerImpl *m_taskRunner;
    WebTaskRunner::Task *m_task;
}

+ (id)taskRunnerWith: (TaskRunnerImpl *)taskRunner andTask: (WebTaskRunner::Task *)task;
- (void)fireBy: (NSDate *)date;

@end

@implementation NativeTaskRunner

+ (id)taskRunnerWith: (TaskRunnerImpl *)taskRunner andTask: (WebTaskRunner::Task *)task
{
    NativeTaskRunner *ret = [[NativeTaskRunner alloc] init];
    if (nil == ret)
        return nil;

    ret->m_taskRunner = taskRunner;
    ret->m_task = task;
    return [ret autorelease];
}

- (void)fireBy: (NSDate *)date
{
    NSTimeInterval interval = 0;
    if (nil != date)
        interval = date.timeIntervalSinceNow;

    if (interval <= 0)
    {
        m_taskRunner->Run(m_task);
        return;
    }

    id timerProc = ^(NSTimer *) {
        m_taskRunner->Run(m_task);
    };
    NSTimer *timer = [NSTimer timerWithTimeInterval: interval repeats: NO block: timerProc];
    [[NSRunLoop currentRunLoop] addTimer: timer forMode: NSRunLoopCommonModes];
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
    NativeTaskRunner *taskRunner = [NativeTaskRunner taskRunnerWith: this andTask: task];
    [taskRunner performSelector: @selector(fireBy:)
                       onThread: m_nativeThread
                     withObject: [NSDate dateWithTimeIntervalSinceNow: delayMs / 1000]
                  waitUntilDone: NO];
}

void AppleTaskRunner::postTask(const WebTraceLocation &location, Task *task)
{
    NativeTaskRunner *taskRunner = [NativeTaskRunner taskRunnerWith: this andTask: task];
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
