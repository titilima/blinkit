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
#include <tuple>
#ifdef _DEBUG
#include <unordered_set>
#endif

namespace blink {
class LocalFrame;
class ScriptWrappable;
}

namespace BlinKit {

class CrawlerImpl;
class FunctionManager;
class PrototypeManager;

class DukContext
{
public:
    DukContext(blink::LocalFrame &frame);
    ~DukContext(void);

    static DukContext* From(duk_context *ctx);
    CrawlerImpl* GetCrawler(void);

    std::tuple<int, std::string> CreateCrawlerObject(const char *script, size_t length);
    int Eval(const char *code, size_t length, BkCallback *callback, const char *fileName = nullptr);
    int CallFunction(const char *name, BkCallback *callback);
    int CallCrawler(const char *method, BkCallback *callback = nullptr);
    int RegisterFunction(const char *name, BkCallback &functionImpl);
    int AccessCrawlerMember(const char *name, BkCallback &callback);
    void GetCrawlerProperty(const char *name, const std::function<void(const BkValue &)> &callback);

    template <class T>
    void CreateObject(void) {
        CreateObject(T::ProtoName, nullptr, nullptr, nullptr);
    }
    template <class T>
    void CreateObject(blink::ScriptWrappable *nativeThis) {
        assert(nullptr != nativeThis);
        CreateObject(T::ProtoName, nativeThis, T::OnCreate, T::OnGC);
    }
    template <class T>
    void PushObject(blink::ScriptWrappable *nativeThis) {
        if (nullptr == nativeThis)
        {
            duk_push_undefined(m_context);
            return;
        }

        auto it = m_objectPool.find(nativeThis);
        if (std::end(m_objectPool) != it)
            duk_push_heapptr(m_context, it->second.HeapPtr);
        else
            CreateObject(T::ProtoName, nativeThis, T::OnCreate, T::OnGC);
    }
    void RemoveObjectFromPool(blink::ScriptWrappable *nativeThis);

    void Reset(void);
private:
    void Attach(void);
    void CreateObject(const char *protoName, blink::ScriptWrappable *nativeThis,
        void(*createCallback)(duk_context *, blink::ScriptWrappable *),
        void(*gcCallback)(blink::ScriptWrappable *));
    void Initialize(void);
    void GC(void);
    static void AdjustGlobalsForCrawler(duk_context *ctx);
    void PrepareGlobalsToTop(void);
#ifndef BLINKIT_CRAWLER_ONLY
    void RegisterPrototypes(void);
#endif
    void RegisterPrototypesForCrawler(void);

    blink::LocalFrame &m_frame;
    duk_context *m_context;
    std::unique_ptr<PrototypeManager> m_prototypeManager;
    std::unique_ptr<FunctionManager> m_functionManager;
    void *m_crawlerObjectPtr = nullptr;
    void *m_globalsPtr = nullptr;

    struct ObjectEntry {
        void *HeapPtr;
        void (*GC)(blink::ScriptWrappable *);
    };
    std::unordered_map<blink::ScriptWrappable *, ObjectEntry> m_objectPool;
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_CONTEXT_H
