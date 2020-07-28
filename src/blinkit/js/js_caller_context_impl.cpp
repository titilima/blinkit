// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: js_caller_context_impl.cpp
// Description: JSCallerContextImpl Class
//      Author: Ziming Li
//     Created: 2020-07-14
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "js_caller_context_impl.h"

#include "blinkit/js/js_value_impl.h"
#include "third_party/blink/renderer/bindings/core/duk/duk.h"
#include "third_party/blink/renderer/platform/bindings/gc_pool.h"

using namespace BlinKit;

JSCallerContextImpl::JSCallerContextImpl(duk_context *ctx, duk_idx_t idx)
    : HeapRetainedValue(ctx, HeapRetainedValue::Type::ARRAY, DUK_HIDDEN_SYMBOL("caller_ctx"))
    , m_gcPool(std::make_unique<GCPool>(ctx))
{
    void *heapPtr = duk_get_heapptr(m_ctx, idx);

    HeapRetainedValue::PushTo(m_ctx);
    duk_push_heapptr(m_ctx, heapPtr);
    duk_put_prop_index(m_ctx, -2, 0);
    duk_pop(m_ctx);
}

JSCallerContextImpl::~JSCallerContextImpl(void) = default;

int JSCallerContextImpl::Call(BkJSValue *retVal)
{
    Duk::StackGuard sg(m_ctx);

    int err = BK_ERR_SUCCESS;

    duk_idx_t idx = HeapRetainedValue::PushTo(m_ctx);
    size_t n = duk_get_length(m_ctx, idx);

    // function
    duk_get_prop_index(m_ctx, idx, 0);
    // this
    if (nullptr != m_thisPtr)
        duk_push_heapptr(m_ctx, m_thisPtr);
    // args
    for (size_t i = 1; i < n; ++i)
        duk_get_prop_index(m_ctx, idx, i);

    --n;
    if (nullptr != m_thisPtr)
        duk_pcall_method(m_ctx, n);
    else
        duk_pcall(m_ctx, n);

    std::unique_ptr<JSValueImpl> ret(JSValueImpl::Create(m_ctx, -1));
    if (ret->GetType() == BK_VT_ERROR)
        err = static_cast<JSErrorImpl *>(ret.get())->GetCode();
    if (nullptr != retVal)
        *retVal = ret.release();
    delete this;
    return err;
}

int JSCallerContextImpl::DoPush(const std::function<void(duk_context *)> &worker)
{
    Duk::StackGuard sg(m_ctx);

    duk_idx_t idx = HeapRetainedValue::PushTo(m_ctx);

    worker(m_ctx);

    ASSERT(sg.IsChangedBy(2));
    size_t i = duk_get_length(m_ctx, idx);
    duk_put_prop_index(m_ctx, idx, i);
    return BK_ERR_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT int BKAPI BkCallFunction(BkJSCallerContext callerContext, BkJSValue *retVal)
{
    return callerContext->Call(retVal);
}

BKEXPORT int BKAPI BkPushInteger(BkJSCallerContext callerContext, int n)
{
    const auto worker = [n](duk_context *ctx)
    {
        duk_push_int(ctx, n);
    };
    return callerContext->DoPush(worker);
}

BKEXPORT int BKAPI BkPushString(BkJSCallerContext callerContext, const char *s)
{
    const auto worker = [s](duk_context *ctx)
    {
        duk_push_string(ctx, s);
    };
    return callerContext->DoPush(worker);
}

BKEXPORT int BKAPI BkPushStringPiece(BkJSCallerContext callerContext, const char *s, size_t l)
{
    const auto worker = [s, l](duk_context *ctx)
    {
        duk_push_lstring(ctx, s, l);
    };
    return callerContext->DoPush(worker);
}

} // extern "C"
