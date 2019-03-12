// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: view_scheduler_impl.h
// Description: ViewSchedulerImpl Class
//      Author: Ziming Li
//     Created: 2019-03-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_VIEW_SCHEDULER_IMPL_H
#define BLINKIT_BLINKIT_VIEW_SCHEDULER_IMPL_H

#pragma once

#include "public/platform/WebViewScheduler.h"

namespace BlinKit {

class ThreadImpl;

class ViewSchedulerImpl final : public blink::WebViewScheduler
{
public:
    static ViewSchedulerImpl* CreateInstance(ThreadImpl &thread);
private:
    ViewSchedulerImpl(ThreadImpl &thread);

    // blink::WebViewScheduler
    void setPageInBackground(bool pageInBackground) override { /* Nothing to do. */ }
    blink::WebPassOwnPtr<blink::WebFrameScheduler> createFrameScheduler(void) override;

    ThreadImpl &m_thread;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_VIEW_SCHEDULER_IMPL_H
