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

#include <unordered_map>
#include "bk_js.h"
#include "duktape/duktape.h"

namespace BlinKit {

class FunctionManager
{
public:
    FunctionManager(void) = default;

    struct FunctionData {
        int memberContext;
        BkFunctionImpl impl;
        void *userData;
    };
    int Register(const std::string &name, const FunctionData &data);
    int Register(duk_context *ctx, duk_idx_t dst, const std::string &name, const FunctionData &data, void *thisObject);

    struct Indices {
        duk_idx_t globalObjectIndex = -1;
        duk_idx_t userObjectIndex   = -1;
    };
    void FlushAll(duk_context *ctx, const Indices &indices, void *userObject);
private:
    static void Flush(duk_context *ctx, duk_idx_t dst, const std::string &name, FunctionData &data, void *thisObject);
    static duk_ret_t CalleeImpl(duk_context *ctx);

    std::unordered_map<std::string, FunctionData> m_functions;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_FUNCTION_MANAGER_H
