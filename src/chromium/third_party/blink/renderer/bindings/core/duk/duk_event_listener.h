// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_event_listener.h
// Description: DukEventListener Class
//      Author: Ziming Li
//     Created: 2020-03-19
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DUK_EVENT_LISTENER_H
#define BLINKIT_BLINK_DUK_EVENT_LISTENER_H

#pragma once

#include "duktape/duktape.h"
#include "third_party/blink/renderer/core/dom/events/event_listener.h"

namespace blink {
class EventTarget;
}

namespace BlinKit {

class DukEventListener : public blink::EventListener
{
public:
    static std::shared_ptr<DukEventListener> Create(duk_context *ctx, duk_idx_t idx, blink::EventTarget *target, const AtomicString &type);
    ~DukEventListener(void) override;

    static EventListener* From(duk_context *ctx, duk_idx_t idx, blink::EventTarget *target, const AtomicString &type);
private:
    DukEventListener(duk_context *ctx, void *heapPtr, const std::string &key);

    static std::string GenerateKey(blink::EventTarget *target, const AtomicString &type, void *heapPtr);

    // blink::EventListener
    bool operator==(const blink::EventListener &other) const final { return this == &other; }
    void handleEvent(blink::ExecutionContext *executionContext, blink::Event *event) final;

    duk_context *m_ctx = nullptr;
    void *m_heapPtr = nullptr;
    const std::string m_key;
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_EVENT_LISTENER_H
