// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_context.h
// Description: DukContext Class
//      Author: Ziming Li
//     Created: 2019-03-07
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_CONTEXT_H
#define BLINKIT_JS_DUK_CONTEXT_H

#pragma once

#include <functional>

namespace blink {
class LocalFrame;
}

namespace BlinKit {

class FunctionManager;
class PrototypeManager;

class DukContext
{
public:
    DukContext(blink::LocalFrame &frame);
    ~DukContext(void);

    static DukContext* From(duk_context *ctx);

    int CreateCrawlerObject(const char *script, size_t length);
    int Eval(const char *code, size_t length, BkCallback *callback, const char *fileName = nullptr);
    int CallFunction(const char *name, BkCallback *callback);
    int CallCrawler(const char *method, BkCallback *callback);
    int RegisterFunction(const char *name, BkCallback &functionImpl);
    int AccessCrawlerMember(const char *name, BkCallback &callback);
    void GetCrawlerProperty(const char *name, const std::function<void(const BkValue &)> &callback);

    void CreateObject(const char *protoName);

    void Reset(void);
private:
    void Attach(blink::LocalFrame &frame);
    void Initialize(void);
    static void AdjustGlobalsForCrawler(duk_context *ctx);
    void PrepareGlobalsToTop(void);
#ifndef BLINKIT_CRAWLER_ONLY
    void RegisterPrototypes(void);
#endif
    void RegisterPrototypesForCrawler(void);

    duk_context *m_context;
    std::unique_ptr<PrototypeManager> m_prototypeManager;
    std::unique_ptr<FunctionManager> m_functionManager;
    void *m_crawlerObjectPtr = nullptr;
    void *m_globalsPtr = nullptr;
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_CONTEXT_H
