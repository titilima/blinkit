// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: duk_event_listener.h
// Description: DukEventListener Class
//      Author: Ziming Li
//     Created: 2020-03-19
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_DUK_EVENT_LISTENER_H
#define BLINKIT_BLINKIT_DUK_EVENT_LISTENER_H

#pragma once

#include "duktape/duktape.h"
#include "blinkit/blink/renderer/core/events/EventListener.h"
#include "blinkit/blink/renderer/wtf/text/AtomicString.h"

namespace blink {
class EventTarget;
class Node;
class QualifiedName;
}

namespace BlinKit {

class DukEventListener : public blink::EventListener
{
public:
    static std::shared_ptr<EventListener> Get(duk_context *ctx, duk_idx_t idx, blink::EventTarget *target, const AtomicString &type, bool createIfNotExists);
    static std::shared_ptr<EventListener> CreateAttributeEventListener(blink::Node *node, const blink::QualifiedName &name, const AtomicString &value);
    ~DukEventListener(void) override;

private:
    DukEventListener(duk_context *ctx, void *heapPtr, const std::string &key);

    static std::string GenerateKey(blink::EventTarget *target, const AtomicString &type, void *heapPtr);

    // blink::EventListener
    bool operator==(const blink::EventListener &other) const final { return this == &other; }
    void handleEvent(blink::ExecutionContext *executionContext, blink::Event *event) final;
    bool belongsToTheCurrentWorld(void) const final;

    duk_context *m_ctx;
    void *m_heapPtr;
    const std::string m_key;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_DUK_EVENT_LISTENER_H
