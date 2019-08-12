// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_event_listener.cpp
// Description: DukEventListener Class
//      Author: Ziming Li
//     Created: 2019-07-22
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_event_listener.h"

#include "base/strings/stringprintf.h"
#include "core/dom/Document.h"
#include "core/events/BeforeUnloadEvent.h"
#include "wtf/text/AtomicString.h"

#include "context/duk_context.h"
#include "context/value_impl.h"
#include "public/script_controller.h"
#include "wrappers/duk.h"

using namespace blink;

namespace BlinKit {

static const char ListenerPoolKey[] = DUK_HIDDEN_SYMBOL("listenerPool");

static std::string GenerateListenerKey(EventTarget *target, const AtomicString &type, void *heapPtr)
{
    const std::string utf8 = type.to_string();
    return base::StringPrintf("%p_%s_%p", target, utf8.c_str(), heapPtr);
}

static std::string GenerateNativeListenerKey(DukEventListener *listener)
{
    return base::StringPrintf("listener_%p", listener);
}

static DukContext* ToDukContext(ExecutionContext *executionContext)
{
    if (executionContext->isDocument())
    {
        LocalFrame *frame = toDocument(executionContext)->frame();
        return frame->script().Context();
    }

    assert(false); // Not reached!
    return nullptr;
}

DukEventListener::DukEventListener(void) : EventListener(JSEventListenerType)
{
    // Nothing
}

DukEventListener::DukEventListener(duk_context *ctx, void *heapPtr)
    : EventListener(JSEventListenerType), m_ctx(ctx), m_heapPtr(heapPtr)
{
    Duk::StackKeeper sk(ctx);
    duk_push_global_object(ctx);
    duk_get_prop_string(ctx, -1, ListenerPoolKey);

    std::string key = GenerateNativeListenerKey(this);
    duk_push_heapptr(ctx, heapPtr);
    duk_put_prop_lstring(ctx, -2, key.data(), key.length());
}

DukEventListener::~DukEventListener(void)
{
    if (nullptr == m_ctx)
        return;

    Duk::StackKeeper sk(m_ctx);
    duk_push_global_object(m_ctx);
    duk_get_prop_string(m_ctx, -1, ListenerPoolKey);

    std::string key = GenerateNativeListenerKey(this);
    duk_del_prop_lstring(m_ctx, -2, key.data(), key.length());
}

PassRefPtr<DukEventListener> DukEventListener::Create(duk_context *ctx, void *heapPtr, EventTarget *target, const AtomicString &type)
{
    DukEventListener *ret = new DukEventListener(ctx, heapPtr);

    Duk::StackKeeper sk(ctx);
    std::string listenerKey = GenerateListenerKey(target, type, heapPtr);
    duk_push_global_object(ctx);
    duk_get_prop_string(ctx, -1, ListenerPoolKey);
    duk_push_pointer(ctx, ret);
    duk_put_prop_lstring(ctx, -2, listenerKey.data(), listenerKey.length());

    return adoptRef(ret);
}

DukEventListener* DukEventListener::Detach(duk_context *ctx, void *heapPtr, blink::EventTarget *target, const AtomicString &type)
{
    Duk::StackKeeper sk(ctx);
    std::string listenerKey = GenerateListenerKey(target, type, heapPtr);
    duk_push_global_object(ctx);
    duk_get_prop_string(ctx, -1, ListenerPoolKey);
    duk_get_prop_lstring(ctx, -1, listenerKey.data(), listenerKey.length());

    return reinterpret_cast<DukEventListener *>(duk_get_pointer(ctx, -1));
}

void DukEventListener::handleEvent(ExecutionContext *executionContext, Event *event)
{
    DukContext *context = ToDukContext(executionContext);

    // The raw context SHOULD BE retained here,
    // because the event listener (`this`) may be removed & destroyed after the event being handled,
    // and the member `m_ctx` will become `nullptr` in that scenario.
    duk_context *ctx = context->RawContext(); 
    if (nullptr == m_ctx)
    {
        m_ctx = ctx;
        assert(false); // BKTODO: Prepare lazy event listener.
    }
    assert(ctx == m_ctx);

    Duk::StackKeeper sk(ctx);

    assert(nullptr != m_heapPtr);
    duk_push_heapptr(ctx, m_heapPtr);
    context->PushEvent(ctx, event);
    int r = duk_pcall(ctx, 1);
    if (DUK_EXEC_SUCCESS == r)
    {
        if (event->isBeforeUnloadEvent() && !duk_is_null(ctx, -1) && !duk_is_undefined(ctx, -1))
        {
            size_t len = 0;
            const char *s = duk_to_lstring(ctx, -1, &len);
            toBeforeUnloadEvent(event)->setReturnValue(String::fromUTF8(s, len));
        }

        if (duk_is_boolean(ctx, -1))
        {
            if (!duk_to_boolean(ctx, -1))
                event->preventDefault();
        }
    }
    else
    {
#ifdef _DEBUG
        ValueImpl retVal(ctx);
        retVal.SetAsErrorType();
#endif
        assert(DUK_EXEC_SUCCESS == r);
    }
}

void DukEventListener::InitializeListenerPool(duk_context *ctx)
{
    duk_push_object(ctx);
    duk_put_prop_string(ctx, -2, ListenerPoolKey);
}

} // namespace BlinKit
