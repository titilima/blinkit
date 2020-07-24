// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: function_manager.cpp
// Description: FunctionManager Class
//      Author: Ziming Li
//     Created: 2020-07-23
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "function_manager.h"

#include "blinkit/js/js_callee_context_impl.h"
#include "blinkit/js/js_value_impl.h"
#include "third_party/blink/renderer/bindings/core/duk/duk.h"

namespace BlinKit {

FunctionManager::FunctionManager(std::unique_ptr<JSObjectImpl> &userObject) : m_userObject(userObject)
{
}

duk_ret_t FunctionManager::CalleeImpl(duk_context *ctx)
{
    int argc = duk_get_top(ctx);

    duk_push_current_function(ctx);

    std::string key = GetKey(duk_get_heapptr(ctx, -1));
    duk_push_global_object(ctx);
    duk_get_prop_lstring(ctx, -1, key.data(), key.length());

    const FunctionData *fd = reinterpret_cast<FunctionData *>(duk_get_pointer(ctx, -1));

    JSCalleeContextImpl calleeContext(ctx, argc);
    if (nullptr != fd->thisObject)
        calleeContext.SetThis(fd->thisObject->get());
    fd->impl(&calleeContext, fd->userData);
    return calleeContext.Return();
}

std::string FunctionManager::GetKey(void *heapPtr)
{
    std::string ret(DUK_HIDDEN_SYMBOL("fn_"));

    char buf[128];
    sprintf(buf, "%p", heapPtr);
    ret.append(buf);

    return ret;
}

int FunctionManager::Register(int memberContext, const char *functionName, BkFunctionImpl impl, void *userData)
{
    FunctionData data = { 0 };
    switch (memberContext)
    {
        case BK_CTX_GLOBAL:
            break;
        case BK_CTX_USER_OBJECT:
            if (!m_userObject)
                return BK_ERR_FORBIDDEN;
            data.thisObject = &m_userObject;
            break;
        default:
            NOTREACHED();
            return BK_ERR_FORBIDDEN;
    }
    data.impl = impl;
    data.userData = userData;

    m_functions[functionName] = data;
    return BK_ERR_SUCCESS;
}

void FunctionManager::RegisterTo(duk_context *ctx)
{
    Duk::StackGuard sg(ctx);

    int globalIdx, userIdx;
    duk_push_global_object(ctx);
    globalIdx = duk_normalize_index(ctx, -1);
    if (m_userObject)
    {
        m_userObject->PushTo(ctx);
        userIdx = duk_normalize_index(ctx, -1);
    }
    else
    {
        userIdx = -1;
    }

    for (auto &it : m_functions)
    {
        int idx = -1;
        if (nullptr == it.second.thisObject)
            idx = globalIdx;
        else if (it.second.thisObject == &m_userObject)
            idx = userIdx;
        else
            NOTREACHED();
        if (-1 == idx)
            continue;

        duk_push_c_function(ctx, CalleeImpl, DUK_VARARGS);
        void *heapPtr = duk_get_heapptr(ctx, -1);
        duk_put_prop_lstring(ctx, idx, it.first.data(), it.first.length());

        std::string key = GetKey(heapPtr);
        FunctionData &fd = it.second;
        duk_push_pointer(ctx, &fd);
        duk_put_prop_lstring(ctx, globalIdx, key.data(), key.length());
    }
}

} // namespace BlinKit
