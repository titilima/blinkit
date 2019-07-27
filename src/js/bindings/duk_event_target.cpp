// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_event_target.cpp
// Description: Bindings for EventTarget
//      Author: Ziming Li
//     Created: 2019-06-01
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_event_target.h"

#include "core/frame/LocalDOMWindow.h"

#include "bindings/duk_event_listener.h"
#include "context/duk_context.h"
#include "context/prototype_manager.h"
#include "wrappers/duk.h"

using namespace blink;

namespace BlinKit {

static EventTarget* Get(duk_context *ctx, duk_idx_t idx = -1)
{
    return DukEventTarget::GetNativeThis<EventTarget>(ctx, idx);
}

duk_ret_t DukEventTarget::Finalizer(duk_context *ctx)
{
    EventTarget *nativeThis = Get(ctx, 0);
    DukContext::From(ctx)->RemoveObjectFromPool(nativeThis);
    return 0;
}

EventTarget* DukEventTarget::Get(duk_context *ctx, duk_idx_t idx)
{
    return static_cast<EventTarget *>(Duk::GetNativeThis(ctx, idx));
}

namespace Impl {

static duk_ret_t AddEventListener(duk_context *ctx)
{
    const duk_idx_t argc = duk_get_top(ctx);

    duk_push_this(ctx);
    EventTarget *eventTarget = Get(ctx);
    if (LocalDOMWindow *window = eventTarget->toDOMWindow())
    {
        if (nullptr == window->document())
            return 0;
    }

    const AtomicString type = Duk::ToAtomicString(ctx, 0);
    RefPtr<EventListener> listener = DukEventListener::Create(ctx, duk_get_heapptr(ctx, 1), eventTarget, type);
    bool useCapture = false;

    switch (argc)
    {
        case 2:
            break;
        case 3:
            useCapture = duk_to_boolean(ctx, 2);
            break;
        default:
            assert(argc == 2 || argc == 3); // Invalid arguments!
            duk_error(ctx, DUK_ERR_TYPE_ERROR, "Invalid argument count: %d", argc);
            return 0;
    }

    eventTarget->addEventListener(type, listener, useCapture);
    return 0;
}

static duk_ret_t RemoveEventListener(duk_context *ctx)
{
    duk_idx_t argc = duk_get_top(ctx);

    duk_push_this(ctx);
    EventTarget *eventTarget = Get(ctx);
    if (LocalDOMWindow * window = eventTarget->toDOMWindow())
    {
        if (nullptr == window->document())
            return 0;
    }

    const AtomicString type = Duk::ToAtomicString(ctx, 0);
    EventListener *listener = DukEventListener::Detach(ctx, duk_get_heapptr(ctx, 1), eventTarget, type);
    bool useCapture = false;

    switch (argc)
    {
        case 2:
            break;
        case 3:
            useCapture = duk_to_boolean(ctx, 2);
            break;
        default:
            assert(argc == 2 || argc == 3); // Invalid arguments!
            duk_error(ctx, DUK_ERR_TYPE_ERROR, "Invalid argument count: %d", argc);
            return 0;
    }

    eventTarget->removeEventListener(type, listener, useCapture);
    return 0;
}

} // namespace Impl

void DukEventTarget::RegisterToPrototypeEntry(PrototypeEntry &entry)
{
    static const PrototypeEntry::Method Methods[] = {
        { "addEventListener",    Impl::AddEventListener,    DUK_VARARGS },
        { "removeEventListener", Impl::RemoveEventListener, DUK_VARARGS }
    };

    entry.SetFinalizer(Finalizer);
    entry.Add(Methods, WTF_ARRAY_LENGTH(Methods));
}

} // namespace BlinKit
