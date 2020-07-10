// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_timer.cpp
// Description: DukTimer Class
//      Author: Ziming Li
//     Created: 2020-07-05
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "duk_timer.h"

#include "blinkit/js/context_impl.h"
#include "third_party/blink/renderer/bindings/core/duk/duk.h"

namespace BlinKit {

/**
 * Timer data layout in array:
 * ---------------------------------------
 * | Callback | Arg0 | Arg1 | ... | ArgN |
 * ---------------------------------------
 */

DukTimer::DukTimer(duk_context *ctx, duk_idx_t idx, int argc) : m_ctx(ctx)
{
    char buf[128];
    sprintf(buf, "%p", this);
    m_key.assign(DUK_HIDDEN_SYMBOL("timer"));
    m_key.append(buf);

    duk_push_global_object(m_ctx);
    duk_push_bare_array(m_ctx);

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

    duk_put_prop_lstring(m_ctx, -2, m_key.data(), m_key.length());
    duk_pop(m_ctx);
}

DukTimer::~DukTimer(void)
{
    duk_push_global_object(m_ctx);
    duk_del_prop_lstring(m_ctx, -1, m_key.data(), m_key.length());
    duk_pop(m_ctx);
}

static void OnError(duk_context *ctx)
{
#ifndef NDEBUG
    duk_get_prop_string(ctx, -1, "stack");
#endif
    std::string str = Duk::To<std::string>(ctx, -1);
    ContextImpl::From(ctx)->ConsoleOutput(BK_CONSOLE_ERROR, str.c_str());
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
    bool ret = true;

    int top = duk_get_top(m_ctx);

    duk_push_global_object(m_ctx);
    if (duk_get_prop_lstring(m_ctx, -1, m_key.data(), m_key.length()))
    {
        duk_get_prop_index(m_ctx, -1, 0);
        if (duk_is_string(m_ctx, -1))
            DoEval(m_ctx);
        else if (duk_is_callable(m_ctx, -1))
            DoCall(m_ctx, duk_normalize_index(m_ctx, -2));
        else
            NOTREACHED();
    }
    else
    {
        ret = false; // Already unavailable, destroy it after `Fire`.
    }

    duk_set_top(m_ctx, top);
    return ret;
}

} // namespace BlinKit
