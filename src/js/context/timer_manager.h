// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: timer_manager.h
// Description: TimerManager Class
//      Author: Ziming Li
//     Created: 2019-08-24
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_TIMER_MANAGER_H
#define BLINKIT_JS_TIMER_MANAGER_H

#pragma once

namespace BlinKit {

class TimerManager final
{
public:
    static void Attach(duk_context *ctx);

    static duk_ret_t SetInterval(duk_context *ctx);
    static duk_ret_t SetTimeout(duk_context *ctx);
private:
    TimerManager(void) = delete;

    static int SetTimer(duk_context *ctx, int argc, bool repeat);
};

} // namespace BlinKit

#endif // BLINKIT_JS_TIMER_MANAGER_H
