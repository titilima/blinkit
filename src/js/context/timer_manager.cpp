// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: timer_manager.cpp
// Description: TimerManager Class
//      Author: Ziming Li
//     Created: 2019-08-24
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "timer_manager.h"

#include "public/platform/Platform.h"
#include "public/platform/WebTaskRunner.h"
#include "public/platform/WebTraceLocation.h"

#include "context/value_impl.h"
#include "wrappers/duk.h"

using namespace blink;

namespace BlinKit {

namespace Keys {
static const char TimerManager[] = DUK_HIDDEN_SYMBOL("Timers");
static duk_uarridx_t TimerNextIdIndex = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TimerTask final : public WebTaskRunner::Task
{
public:
    static void Fire(duk_context *ctx, int timerId, unsigned delay);
private:
    TimerTask(duk_context *ctx, int timerId);

    // WebTaskRunner::Task
    void run(void) override;

    duk_context *m_ctx;
    const int m_timerId;
};

TimerTask::TimerTask(duk_context *ctx, int timerId) : m_ctx(ctx), m_timerId(timerId)
{
    // Nothing
}

void TimerTask::Fire(duk_context *ctx, int timerId, unsigned delay)
{
    TimerTask *task = new TimerTask(ctx, timerId);

    WebTaskRunner *taskRunner = Platform::current()->currentThread()->taskRunner();
    if (0 == delay)
        taskRunner->postTask(BLINK_FROM_HERE, task);
    else
        taskRunner->postDelayedTask(BLINK_FROM_HERE, task, delay);
}

void TimerTask::run(void)
{
    Duk::StackKeeper sk(m_ctx);

    duk_push_global_object(m_ctx);
    duk_get_prop_string(m_ctx, -1, Keys::TimerManager);
    int mgrIdx = duk_get_top_index(m_ctx);

    if (!duk_get_prop_index(m_ctx, mgrIdx, m_timerId))
        return; // May be cleared before.

    int timerIdx = duk_get_top_index(m_ctx);

    duk_get_prop_index(m_ctx, timerIdx, 0); // worker
    int r = DUK_EXEC_SUCCESS;
    if (duk_is_string(m_ctx, -1))
    {
        r = duk_peval(m_ctx);
    }
    else if (duk_is_callable(m_ctx, -1))
    {
        duk_size_t n = duk_get_length(m_ctx, timerIdx);
        for (duk_size_t i = 2; i < n; ++i)
            duk_get_prop_index(m_ctx, timerIdx, i);
        r = duk_pcall(m_ctx, n - 2);
    }
#ifdef _DEBUG
    if (DUK_EXEC_SUCCESS != r)
    {
        ValueImpl retVal(m_ctx);
        retVal.SetAsErrorType();
    }
#endif
    assert(DUK_EXEC_SUCCESS == r);

    // Repeatable?
    duk_get_prop_index(m_ctx, timerIdx, 1);
    if (duk_is_number(m_ctx, -1))
    {
        unsigned delay = duk_to_uint(m_ctx, -1);
        Fire(m_ctx, m_timerId, delay);
    }
    else
    {
        duk_del_prop_index(m_ctx, mgrIdx, m_timerId);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const char TypeError[] = "Not enough arguments";

void TimerManager::Attach(duk_context *ctx)
{
    duk_push_object(ctx);

    duk_push_int(ctx, 1);
    duk_put_prop_index(ctx, -2, Keys::TimerNextIdIndex);

    duk_put_prop_string(ctx, -2, Keys::TimerManager);
}

duk_ret_t TimerManager::SetInterval(duk_context *ctx)
{
    int argc = duk_get_top(ctx);
    if (argc < 1)
        return duk_type_error(ctx, TypeError);

    int timerId = SetTimer(ctx, argc, true);
    TimerTask::Fire(ctx, timerId, duk_opt_uint(ctx, 1, 0));

    duk_push_int(ctx, timerId);
    return 1;
}

duk_ret_t TimerManager::SetTimeout(duk_context *ctx)
{
    int argc = duk_get_top(ctx);
    if (argc < 1)
        return duk_type_error(ctx, TypeError);

    int timerId = SetTimer(ctx, argc, false);
    TimerTask::Fire(ctx, timerId, duk_opt_uint(ctx, 1, 0));

    duk_push_int(ctx, timerId);
    return 1;
}

int TimerManager::SetTimer(duk_context *ctx, int argc, bool repeat)
{
    int timerId;

    duk_push_global_object(ctx);
    duk_get_prop_string(ctx, -1, Keys::TimerManager);

    duk_get_prop_index(ctx, -1, Keys::TimerNextIdIndex);
    timerId = duk_to_int(ctx, -1);
    duk_pop(ctx);

    duk_push_array(ctx);
    // worker
    duk_dup(ctx, 0);
    duk_put_prop_index(ctx, -2, 0);
    if (argc > 1)
    {
        // interval
        if (repeat)
            duk_dup(ctx, 1);
        else
            duk_push_undefined(ctx);
        duk_put_prop_index(ctx, -2, 1);
        // params
        for (int i = 2; i < argc; ++i)
        {
            duk_dup(ctx, i);
            duk_put_prop_index(ctx, -2, i);
        }
    }
    duk_put_prop_index(ctx, -2, timerId);

    duk_push_int(ctx, timerId + 1);
    duk_put_prop_index(ctx, -2, Keys::TimerNextIdIndex);
    return timerId;
}

} // namespace BlinKit
