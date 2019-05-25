// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: caller_context_impl.cpp
// Description: CallerContextImpl Class
//      Author: Ziming Li
//     Created: 2019-05-18
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "caller_context_impl.h"

#include "context/value_impl.h"

namespace BlinKit {

using namespace Duk;

CallerContextImpl::CallerContextImpl(duk_context *ctx) : StackKeeper(ctx, 1)
{
    // Nothing
}

int CallerContextImpl::Call(const char *name, BkCallback *callback)
{
    assert(DUK_TYPE_OBJECT == duk_get_type(m_ctx, -1));

    if (!duk_get_prop_string(m_ctx, -1, name))
        return BkError::NotFound;
    if (!duk_is_callable(m_ctx, -1))
        return BkError::TypeError;

    if (m_thisCall)
    {
        void *heapPtr = duk_get_heapptr(m_ctx, -2);
        duk_push_heapptr(m_ctx, heapPtr);
    }

    if (nullptr != callback)
        callback->OnPushArgs(*this);

    int r;
    if (m_thisCall)
        r = duk_pcall_method(m_ctx, m_argc);
    else
        r = duk_pcall(m_ctx, m_argc);

    ValueImpl retVal(m_ctx);
    if (DUK_EXEC_SUCCESS != r)
        retVal.SetAsErrorType();
    if (nullptr != callback)
        callback->OnReturn(retVal);

    return retVal.ErrorCode();
}

int BKAPI CallerContextImpl::PushInt(int arg)
{
    duk_push_int(m_ctx, arg);
    return ++m_argc;
}

int BKAPI CallerContextImpl::PushString(const char *arg, size_t length)
{
    if (0 == length)
        duk_push_string(m_ctx, arg);
    else
        duk_push_lstring(m_ctx, arg, length);
    return ++m_argc;
}

} // namespace BlinKit
