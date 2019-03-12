// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: scheduler_impl.h
// Description: SchedulerImpl Class
//      Author: Ziming Li
//     Created: 2019-03-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_SCHEDULER_IMPL_H
#define BLINKIT_BLINKIT_SCHEDULER_IMPL_H

#pragma once

#include "public/platform/WebScheduler.h"

namespace BlinKit {

class ThreadImpl;

class SchedulerImpl final : public blink::WebScheduler
{
public:
    SchedulerImpl(ThreadImpl &thread);
    ~SchedulerImpl(void);
private:
    // blink::WebScheduler
    void shutdown(void) override { /* Nothing to do. */ }
    bool shouldYieldForHighPriorityWork(void) override { return false; }
    bool canExceedIdleDeadlineIfRequired(void) override;
    void postIdleTask(const blink::WebTraceLocation &location, blink::WebThread::IdleTask *task) override;
    void postNonNestableIdleTask(const blink::WebTraceLocation &location, blink::WebThread::IdleTask *task) override;
    void postIdleTaskAfterWakeup(const blink::WebTraceLocation &location, blink::WebThread::IdleTask *task) override;
    blink::WebTaskRunner* loadingTaskRunner(void) override;
    blink::WebTaskRunner* timerTaskRunner(void) override;
    blink::WebPassOwnPtr<blink::WebViewScheduler> createWebViewScheduler(blink::WebView *webView) override;
    void suspendTimerQueue(void) override;
    void resumeTimerQueue(void) override;
    void addPendingNavigation(void) override;
    void removePendingNavigation(void) override;
    void onNavigationStarted(void) override;

    ThreadImpl &m_thread;
#ifdef _DEBUG
    bool m_pending = false;
#endif
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_SCHEDULER_IMPL_H
