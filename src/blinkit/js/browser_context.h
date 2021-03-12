// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: browser_context.h
// Description: BrowserContext Class
//      Author: Ziming Li
//     Created: 2020-08-09
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_BROWSER_CONTEXT_H
#define BLINKIT_BLINKIT_BROWSER_CONTEXT_H

#pragma once

#include <unordered_map>
#include "blinkit/js/context_impl.h"

namespace blink {
class ExecutionContext;
class LocalFrame;
}

namespace BlinKit {

class BrowserContext : public ContextImpl
{
public:
    ~BrowserContext(void) override;

    static BrowserContext* From(duk_context *ctx);
    static BrowserContext* From(blink::ExecutionContext *executionContext);

    const blink::LocalFrame& GetFrame(void) const { return m_frame; }

    void Clear(void);
    void WillStartNavigation(void) { m_sessionFlags = 0; }
    void ResetSessionIfNecessary(void);
    virtual void UpdateDocument(void);
    void ConsoleOutput(int type, const char *msg) { m_consoleMessager(type, msg); }

    const char* LookupPrototypeName(const std::string &tagName) const;
protected:
    typedef std::unordered_map<std::string, const char *> PrototypeMap;
    BrowserContext(const blink::LocalFrame &frame, const PrototypeMap &prototypeMap);

    void InitializeHeapStash(DukWorker worker);
    void NewGlobalObject(void);
    virtual void InitializeSession(void);

    const PrototypeMap &m_prototypeMap;
    std::function<void(int, const char *)> m_consoleMessager;
private:
    static void ExposeGlobals(duk_context* ctx, duk_idx_t dst);

    bool QueryDestroy(void) const final { return false; }

    const blink::LocalFrame &m_frame;

    enum SessionFlags {
        GLOBAL_OBJECT_INITIALIZED = 0x1,
        SESSION_INITIALIZED = 0x2
    };
    unsigned m_sessionFlags = 0;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_BROWSER_CONTEXT_H
