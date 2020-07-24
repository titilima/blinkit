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
    FunctionManager(std::unique_ptr<JSObjectImpl> &userObject);

    int Register(int memberContext, const char *functionName, BkFunctionImpl impl, void *userData);
    void RegisterTo(duk_context *ctx);
private:
    static std::string GetKey(void *heapPtr);
    static duk_ret_t CalleeImpl(duk_context *ctx);

    std::unique_ptr<JSObjectImpl> &m_userObject;

    struct FunctionData {
        std::unique_ptr<JSObjectImpl> *thisObject;
        BkFunctionImpl impl;
        void *userData;
    };
    std::unordered_map<std::string, FunctionData> m_functions;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_FUNCTION_MANAGER_H
