// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: function_manager.h
// Description: FunctionManager Class
//      Author: Ziming Li
//     Created: 2020-07-23
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_FUNCTION_MANAGER_H
#define BLINKIT_BLINKIT_FUNCTION_MANAGER_H

#pragma once

#include <functional>
#include <unordered_map>
#include "bk_js.h"
#include "duktape/duktape.h"

namespace BlinKit {

class FunctionManager
{
public:
    FunctionManager(ContextImpl &ctx);

    int Register(int memberContext, const char *functionName, BkFunctionImpl impl, void *userData);
    void FlushAll(void);
private:
    struct FunctionData {
        int memberContext;
        JSObjectImpl *thisObject;
        BkFunctionImpl impl;
        void *userData;
    };

    static std::string GetKey(void *heapPtr);
    static void Flush(duk_context *ctx, const std::string &name, FunctionData &data);
    static void Register(duk_context *ctx, duk_idx_t stashIdx, duk_idx_t objIdx, const std::string &name, FunctionData &data);
    static duk_ret_t CalleeImpl(duk_context *ctx);

    ContextImpl &m_ctx;
    std::unordered_map<std::string, FunctionData> m_functions;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_FUNCTION_MANAGER_H
