// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: web_thread_scheduler.h
// Description: WebThreadScheduler Class
//      Author: Ziming Li
//     Created: 2019-09-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_WEB_THREAD_SCHEDULER_H
#define BLINKIT_BLINK_WEB_THREAD_SCHEDULER_H

#pragma once

#include "base/macros.h"

namespace blink {
namespace scheduler {

class WebThreadScheduler
{
public:
protected:
    WebThreadScheduler(void) = default;
    DISALLOW_COPY_AND_ASSIGN(WebThreadScheduler);
};

} // namespace scheduler
} // namespace blink

#endif // BLINKIT_BLINK_WEB_THREAD_SCHEDULER_H
