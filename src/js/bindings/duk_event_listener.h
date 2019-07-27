// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_event_listener.h
// Description: DukEventListener Class
//      Author: Ziming Li
//     Created: 2019-07-22
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_EVENT_LISTENER_H
#define BLINKIT_JS_DUK_EVENT_LISTENER_H

#pragma once

#include "core/events/EventListener.h"
#include "platform/bindings/script_wrappable.h"

namespace blink {
class EventTarget;
}

namespace BlinKit {

class DukEventListener : public blink::EventListener, public blink::ScriptWrappable
{
public:
    static PassRefPtr<DukEventListener> Create(duk_context *ctx, void *heapPtr, blink::EventTarget *target, const AtomicString &type);
    static DukEventListener* Detach(duk_context *ctx, void *heapPtr, blink::EventTarget *target, const AtomicString &type);
    ~DukEventListener(void);

    static void InitializeListenerPool(duk_context *ctx);
protected:
    DukEventListener(void);

    duk_context *m_ctx = nullptr;
    void *m_heapPtr = nullptr;
private:
    DukEventListener(duk_context *ctx, void *heapPtr);

    // blink::EventListener
    bool operator==(const blink::EventListener &other) const override final { return this == &other; }
    void handleEvent(blink::ExecutionContext *executionContext, blink::Event *event) override final;
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_EVENT_LISTENER_H
