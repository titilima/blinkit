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
    duk_push_global_stash(ctx);
    duk_get_prop_lstring(ctx, -1, key.data(), key.length());

    const FunctionData *fd = reinterpret_cast<FunctionData *>(duk_get_pointer(ctx, -1));

    JSCalleeContextImpl calleeContext(ctx, argc);
    if (nullptr != fd->thisObject)
        calleeContext.SetThis(fd->thisObject->get());
    fd->impl(&calleeContext, fd->userData);
    return calleeContext.Return();
}

void FunctionManager::Flush(duk_context *ctx, const std::string &name, FunctionData &data)
{
    Duk::StackGuard sg(ctx);

    duk_push_global_stash(ctx);
    int stashIdx = duk_normalize_index(ctx, -1);

    if (nullptr != data.thisObject)
        data.thisObject->get()->PushTo(ctx);
    else
        duk_push_global_object(ctx);
    int idx = duk_normalize_index(ctx, -1);

    Register(ctx, stashIdx, idx, name, data);
}

std::string FunctionManager::GetKey(void *heapPtr)
{
    std::string ret(DUK_HIDDEN_SYMBOL("fn_"));

    char buf[128];
    sprintf(buf, "%p", heapPtr);
    ret.append(buf);

    return ret;
}

int FunctionManager::Register(duk_context *ctx, int memberContext, const char *functionName, BkFunctionImpl impl, void *userData)
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

    auto r = m_functions.insert(std::make_pair(std::string(functionName), data));
    Flush(ctx, r.first->first, r.first->second);
    return BK_ERR_SUCCESS;
}

void FunctionManager::Register(duk_context *ctx, duk_idx_t stashIdx, duk_idx_t objIdx, const std::string &name, FunctionData &data)
{
    void *heapPtr;
    duk_push_c_function(ctx, CalleeImpl, DUK_VARARGS);
    heapPtr = duk_get_heapptr(ctx, -1);
    duk_put_prop_lstring(ctx, objIdx, name.data(), name.length());

    std::string key = GetKey(heapPtr);
    duk_push_pointer(ctx, &data);
    duk_put_prop_lstring(ctx, stashIdx, key.data(), key.length());
}

void FunctionManager::RegisterTo(duk_context *ctx)
{
    Duk::StackGuard sg(ctx);

    duk_push_global_stash(ctx);
    int stashIdx = duk_normalize_index(ctx, -1);

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

        Register(ctx, stashIdx, idx, it.first, it.second);
    }
}

} // namespace BlinKit
