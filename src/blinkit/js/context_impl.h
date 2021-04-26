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
class ModuleManager;
}

class CrawlerImpl;

class ContextImpl
{
public:
    virtual ~ContextImpl(void);

    virtual bool IsScriptController(void) const = 0;

    int EnableModules(BkModuleLoader loader, void *userData);

    typedef std::function<void(duk_context *)> Callback;
    void Eval(const std::string_view &code, const Callback &callback, const char *fileName = "eval");

    BkJSCallerContext PrepareFunctionCall(int callContext, const char *functionName);
    BkJSCallerContext PrepareScriptFunction(const char *code);

    int RegisterFunction(int memberContext, const char *functionName, BkFunctionImpl impl, void *userData);

    bool IsSameSession(duk_context *ctx) const { return m_ctx == ctx; }

    static void DefaultConsoleOutput(int type, const char *msg);
protected:
    ContextImpl(void);

    duk_context* EnsureDukSession(void);
    void DestroyDukSession(void);
    virtual bool IsDukSessionDirty(void) const { return false; }

    virtual void Attach(duk_context *ctx, duk_idx_t globalStashIndex);
    virtual void Detach(duk_context *ctx) {}

    std::unique_ptr<BlinKit::ModuleManager> m_moduleManager;
private:
    virtual void* GetUserObject(void) { return nullptr; }

    duk_context *m_ctx = nullptr;
    std::unique_ptr<BlinKit::FunctionManager> m_functionManager;
};

#endif // BLINKIT_BLINKIT_CONTEXT_IMPL_H
