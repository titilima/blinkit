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

static const char CallData[]   = DUK_HIDDEN_SYMBOL("callData");
static const char ThisObject[] = DUK_HIDDEN_SYMBOL("thisObject");

duk_ret_t FunctionManager::CalleeImpl(duk_context *ctx)
{
    int argc = duk_get_top(ctx);

    duk_idx_t funcIdx;
    duk_push_current_function(ctx);
    funcIdx = duk_normalize_index(ctx, -1);

    JSCalleeContextImpl calleeContext(ctx, argc);
    {
        const FunctionData *fd = nullptr;
        duk_get_prop_string(ctx, funcIdx, CallData);
        fd = reinterpret_cast<FunctionData *>(duk_get_pointer(ctx, -1));

        std::unique_ptr<JSObjectImpl> thisObject;
        if (duk_get_prop_string(ctx, funcIdx, ThisObject))
        {
            thisObject = std::make_unique<JSObjectImpl>(ctx, duk_normalize_index(ctx, -1));
            calleeContext.SetThis(thisObject.get());
        }

        fd->impl(&calleeContext, fd->userData);
    }
    return calleeContext.Return();
}

void FunctionManager::Flush(duk_context *ctx, duk_idx_t dst, const std::string &name, FunctionData &data, void *thisObject)
{
    ASSERT(nullptr != ctx);

    duk_push_c_function(ctx, CalleeImpl, DUK_VARARGS);

    duk_push_pointer(ctx, &data);
    duk_put_prop_string(ctx, -2, CallData);
    if (nullptr != thisObject)
    {
        duk_push_heapptr(ctx, thisObject);
        duk_put_prop_string(ctx, -2, ThisObject);
    }

    duk_put_prop_lstring(ctx, dst, name.data(), name.length());
}

void FunctionManager::FlushAll(duk_context *ctx, const Indices &indices, void *userObject)
{
    ASSERT(nullptr != ctx);
    for (auto &it : m_functions)
    {
        switch (it.second.memberContext)
        {
            case BK_CTX_GLOBAL:
                Flush(ctx, indices.globalObjectIndex, it.first, it.second, nullptr);
                break;
            case BK_CTX_USER_OBJECT:
                if (0 != indices.userObjectIndex)
                {
                    Flush(ctx, indices.userObjectIndex, it.first, it.second, userObject);
                    break;
                }
                [[fallthrough]];
            default:
                NOTREACHED();
        }
    }
}


int FunctionManager::Register(duk_context *ctx, duk_idx_t dst, const std::string &name, const FunctionData &data, void *thisObject)
{
    auto it = m_functions.find(name);
    if (m_functions.end() != it)
        return BK_ERR_FORBIDDEN;

    auto r = m_functions.emplace(name, data);
    if (nullptr != ctx)
        Flush(ctx, dst, name, r.first->second, thisObject);

    return BK_ERR_SUCCESS;
}

} // namespace BlinKit
