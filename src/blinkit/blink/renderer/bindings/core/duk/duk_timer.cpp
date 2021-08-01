// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: duk_timer.cpp
// Description: DukTimer Class
//      Author: Ziming Li
//     Created: 2020-07-05
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "./duk_timer.h"

#include "blinkit/blink/renderer/bindings/core/duk/duk.h"
#include "blinkit/blink/renderer/bindings/core/duk/script_controller.h"

using namespace blink;

namespace BlinKit {

/**
 * Timer data layout in array:
 * ---------------------------------------
 * | Callback | Arg0 | Arg1 | ... | ArgN |
 * ---------------------------------------
 */

DukTimer::DukTimer(duk_context *ctx, duk_idx_t idx, int argc)
    : HeapRetainedValue(ctx, HeapRetainedValue::Type::ARRAY, DUK_HIDDEN_SYMBOL("timer"))
{
    idx = duk_normalize_index(ctx, idx);

    HeapRetainedValue::PushTo(ctx);

    // Callback
    void *heapPtr = duk_get_heapptr(m_ctx, idx);
    duk_push_heapptr(m_ctx, heapPtr);
    duk_put_prop_index(m_ctx, -2, 0);
    // Args
    for (int i = 0; i < argc; ++i)
    {
        heapPtr = duk_get_heapptr(m_ctx, idx + 2 + i);
        duk_push_heapptr(m_ctx, heapPtr);
        duk_put_prop_index(m_ctx, -2, i + 1);
    }

    duk_pop(m_ctx);
}

static void OnError(duk_context *ctx)
{
#ifndef NDEBUG
    duk_get_prop_string(ctx, -1, "stack");
#endif
    std::string str = Duk::To<std::string>(ctx, -1);
    ScriptController::From(ctx)->ConsoleOutput(BK_CONSOLE_ERROR, str.c_str());
}

void DukTimer::DoCall(duk_context *ctx, duk_idx_t idx)
{
    duk_size_t n = duk_get_length(ctx, idx);
    for (duk_uarridx_t i = 1; i < n; ++i)
        duk_get_prop_index(ctx, idx, i);

    int r = duk_pcall(ctx, n - 1);
    if (DUK_EXEC_SUCCESS != r)
        OnError(ctx);
}

void DukTimer::DoEval(duk_context *ctx)
{
    int r = duk_peval(ctx);
    if (DUK_EXEC_SUCCESS != r)
        OnError(ctx);
}

bool DukTimer::Fire(void)
{
    const auto worker = [](duk_context *ctx, duk_idx_t idx)
    {
        duk_get_prop_index(ctx, -1, 0);
        if (duk_is_string(ctx, -1))
            DoEval(ctx);
        else if (duk_is_callable(ctx, -1))
            DoCall(ctx, duk_normalize_index(ctx, -2));
        else
            NOTREACHED();
    };
    return HeapRetainedValue::SafeAccess(m_ctx, worker);
}

} // namespace BlinKit
