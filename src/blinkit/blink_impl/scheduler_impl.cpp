// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: scheduler_impl.cpp
// Description: SchedulerImpl Class
//      Author: Ziming Li
//     Created: 2019-03-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "scheduler_impl.h"

#include "blink_impl/thread_impl.h"
#include "blink_impl/view_scheduler_impl.h"

using namespace blink;

namespace BlinKit {

SchedulerImpl::SchedulerImpl(ThreadImpl &thread) : m_thread(thread)
{
    // Nothing
}

SchedulerImpl::~SchedulerImpl(void)
{
    assert(!m_pending);
}

void SchedulerImpl::addPendingNavigation(void)
{
#ifdef _DEBUG
    m_pending = true;
#endif
}

bool SchedulerImpl::canExceedIdleDeadlineIfRequired(void)
{
    assert(false); // BKTODO:
    return false;
}

WebPassOwnPtr<WebViewScheduler> SchedulerImpl::createWebViewScheduler(WebView *webView)
{
    return adoptWebPtr(ViewSchedulerImpl::CreateInstance(m_thread));
}

WebTaskRunner* SchedulerImpl::loadingTaskRunner(void)
{
    return m_thread.taskRunner();
}

void SchedulerImpl::onNavigationStarted(void)
{
    assert(false); // BKTODO:
}

void SchedulerImpl::postIdleTask(const WebTraceLocation &location, WebThread::IdleTask *task)
{
    assert(false); // BKTODO:
}

void SchedulerImpl::postIdleTaskAfterWakeup(const WebTraceLocation &location, WebThread::IdleTask *task)
{
    assert(false); // BKTODO:
}

void SchedulerImpl::postNonNestableIdleTask(const WebTraceLocation &location, WebThread::IdleTask *task)
{
    assert(false); // BKTODO:
}

void SchedulerImpl::removePendingNavigation(void)
{
#ifdef _DEBUG
    m_pending = false;
#endif
}

void SchedulerImpl::resumeTimerQueue(void)
{
    assert(false); // BKTODO:
}

void SchedulerImpl::suspendTimerQueue(void)
{
    assert(false); // BKTODO:
}

WebTaskRunner* SchedulerImpl::timerTaskRunner(void)
{
    return m_thread.taskRunner();
}

} // namespace BlinKit
