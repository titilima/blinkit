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

namespace blink {
class LocalFrame;
}

namespace BlinKit {

class PrototypeManager;

class DukContext
{
public:
    DukContext(blink::LocalFrame &frame);
    ~DukContext(void);

    static DukContext* From(duk_context *ctx);

    int CreateCrawlerObject(const char *script, size_t length);
    int CallFunction(const char *name, BkCallerContext::Callback callback, void *userData);
    int CallCrawler(const char *method, BkCallerContext::Callback callback, void *userData);

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
    void *m_crawlerObjectPtr = nullptr;
    void *m_globalsPtr = nullptr;
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_CONTEXT_H
