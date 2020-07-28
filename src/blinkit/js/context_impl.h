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
#include <unordered_map>
#include "bk_js.h"
#include "duktape/duktape.h"

namespace blink {
class ExecutionContext;
class LocalFrame;
}

namespace BlinKit {
class FunctionManager;
}

class CrawlerImpl;

class ContextImpl final
{
public:
    ContextImpl(const blink::LocalFrame &frame);
    ~ContextImpl(void);

    static ContextImpl* From(duk_context *ctx);
    static ContextImpl* From(blink::ExecutionContext *executionContext);

    void Clear(void);
    void NewGlobalObject(void);
    void UpdateDocument(void);

    const char* LookupPrototypeName(const std::string &tagName) const;

    JSObjectImpl* UserObject(void) { return m_userObject.get(); }
    typedef std::function<void(duk_context *)> Callback;
    void Eval(const std::string_view code, const Callback &callback, const char *fileName = "eval");
    void ConsoleOutput(int type, const char *msg) { m_consoleMessager(type, msg); }
    BkJSCallerContext PrepareFunctionCall(int callContext, const char *functionName);
    int RegisterFunction(int memberContext, const char *functionName, BkFunctionImpl impl, void *userData);

    duk_context* GetRawContext(void) const { return m_ctx; }
private:
    void InitializeHeapStash(void);
    static void RegisterPrototypesForCrawler(duk_context *ctx);
    void CreateUserObject(const CrawlerImpl &crawler);
    static void ExposeGlobals(duk_context *ctx, duk_idx_t dst);

    const blink::LocalFrame &m_frame;
    duk_context *m_ctx;
    std::unique_ptr<BlinKit::FunctionManager> m_functionManager;
    std::function<void(int, const char *)> m_consoleMessager;
    const std::unordered_map<std::string, const char *> &m_prototypeMap;
    std::optional<std::string> m_objectScript;
    std::unique_ptr<JSObjectImpl> m_userObject;
};

#endif // BLINKIT_BLINKIT_CONTEXT_IMPL_H
