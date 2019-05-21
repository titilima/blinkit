// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: function_manager.h
// Description: FunctionManager Class
//      Author: Ziming Li
//     Created: 2019-05-20
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_FUNCTION_MANAGER_H
#define BLINKIT_JS_FUNCTION_MANAGER_H

#pragma once

#include <unordered_map>

namespace BlinKit {

class FunctionManager final
{
public:
    FunctionManager(duk_context *ctx);

    int Register(duk_context *ctx, const char *name, BkFunction *impl);
private:
    static FunctionManager* From(duk_context *ctx);
    static void* CurrentFunction(duk_context *ctx);
    duk_ret_t ProcessCall(duk_context *ctx);
    static duk_ret_t Wrapper(duk_context *ctx);

    void *m_containerPtr;
    struct FunctionEntry {
        std::string name;
        BkFunction *impl;
    };
    std::unordered_map<void *, FunctionEntry> m_functions;
};

} // namespace BlinKit

#endif // BLINKIT_JS_FUNCTION_MANAGER_H
