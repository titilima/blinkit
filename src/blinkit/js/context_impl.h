// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: context_impl.h
// Description: ContextImpl Class
//      Author: Ziming Li
//     Created: 2020-01-21
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_CONTEXT_IMPL_H
#define BLINKIT_BLINKIT_CONTEXT_IMPL_H

#pragma once

#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include "bk_js.h"
#include "duktape/duktape.h"

namespace BlinKit {
class FunctionManager;
}

class CrawlerImpl;

class ContextImpl
{
public:
    virtual ~ContextImpl(void);

    virtual bool QueryDestroy(void) const = 0;

    typedef std::function<void(duk_context *)> Callback;
    void Eval(const std::string_view code, const Callback &callback, const char *fileName = "eval");
    virtual JSObjectImpl* GetContextObject(int callContext) { return nullptr; }
    BkJSCallerContext PrepareFunctionCall(int callContext, const char *functionName);
    BkJSCallerContext PrepareScriptFunction(const char *code);
    int RegisterFunction(int memberContext, const char *functionName, BkFunctionImpl impl, void *userData);

    duk_context* GetRawContext(void) const { return m_ctx; }
protected:
    ContextImpl(void);

    void RegisterFunctions(void);

    typedef void (*DukWorker)(duk_context*);
    std::unique_ptr<BlinKit::FunctionManager> m_functionManager;
private:
    duk_context *m_ctx;
};

#endif // BLINKIT_BLINKIT_CONTEXT_IMPL_H
