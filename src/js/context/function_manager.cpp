// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: function_manager.cpp
// Description: FunctionManager Class
//      Author: Ziming Li
//     Created: 2019-05-20
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "function_manager.h"

#include "context/function_context_impl.h"
#include "wrappers/duk.h"

namespace BlinKit {

namespace StashFields {
static const char FunctionManager[] = "functionManager";
}

FunctionManager::FunctionManager(duk_context *ctx) : m_containerPtr(duk_get_heapptr(ctx, -1))
{
    Duk::StackKeeper sk(ctx);
    duk_push_heap_stash(ctx);
    duk_push_pointer(ctx, this);
    duk_put_prop_string(ctx, -2, StashFields::FunctionManager);
}

void* FunctionManager::CurrentFunction(duk_context *ctx)
{
    Duk::StackKeeper sk(ctx);
    duk_push_current_function(ctx);
    return duk_get_heapptr(ctx, -1);
}

FunctionManager* FunctionManager::From(duk_context *ctx)
{
    Duk::StackKeeper sk(ctx);
    duk_push_heap_stash(ctx);
    if (!duk_get_prop_string(ctx, -1, StashFields::FunctionManager))
        return nullptr;
    return reinterpret_cast<FunctionManager *>(duk_get_pointer(ctx, -1));
}

duk_ret_t FunctionManager::ProcessCall(duk_context *ctx)
{
    auto it = m_functions.find(CurrentFunction(ctx));

    FunctionContextImpl context(ctx, it->second.name);
    it->second.impl->OnCall(context);
    return context.HasReturnValue() ? 1 : 0;
}

int FunctionManager::Register(duk_context *ctx, const char *name, BkFunction *impl)
{
    Duk::StackKeeper sk(ctx);

    duk_push_heapptr(ctx, m_containerPtr);

    duk_push_c_function(ctx, Wrapper, DUK_VARARGS);
    void *heapPtr = duk_get_heapptr(ctx, -1);

    duk_put_prop_string(ctx, -2, name);

    FunctionEntry entry;
    entry.name = name;
    entry.impl = impl;
    m_functions[heapPtr] = entry;
    return BkError::Success;
}

duk_ret_t FunctionManager::Wrapper(duk_context *ctx)
{
    return From(ctx)->ProcessCall(ctx);
}

} // namespace BlinKit
