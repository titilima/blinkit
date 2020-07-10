// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_event_target.cpp
// Description: DukEventTarget Class
//      Author: Ziming Li
//     Created: 2020-01-31
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "duk_event_target.h"

#include "third_party/blink/renderer/bindings/core/duk/duk.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_event_listener.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"

using namespace blink;

namespace BlinKit {

namespace Impl {

static duk_ret_t AddEventListener(duk_context *ctx)
{
    const duk_idx_t argc = duk_get_top(ctx);

    duk_push_this(ctx);
    EventTarget *eventTarget = DukScriptObject::To<EventTarget>(ctx, -1);
    if (const LocalDOMWindow *window = eventTarget->ToLocalDOMWindow())
    {
        if (nullptr == window->document())
            return 0;
    }

    const AtomicString type = Duk::To<AtomicString>(ctx, 0);
    bool useCapture = false;
    switch (argc)
    {
        case 2:
            break;
        case 3:
            useCapture = duk_to_boolean(ctx, 2);
            break;
        default:
            ASSERT(argc == 2 || argc == 3); // Invalid arguments!
            duk_error(ctx, DUK_ERR_TYPE_ERROR, "Invalid argument count: %d", argc);
            return 0;
    }

    std::shared_ptr<EventListener> listener = DukEventListener::Get(ctx, 1, eventTarget, type, true);
    eventTarget->addEventListener(type, listener.get(), useCapture);
    return 0;
}

static duk_ret_t RemoveEventListener(duk_context *ctx)
{
    duk_idx_t argc = duk_get_top(ctx);

    duk_push_this(ctx);
    EventTarget *eventTarget = DukScriptObject::To<EventTarget>(ctx, -1);
    if (const LocalDOMWindow *window = eventTarget->ToLocalDOMWindow())
    {
        if (nullptr == window->document())
            return 0;
    }

    const AtomicString type = Duk::To<AtomicString>(ctx, 0);
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

    std::shared_ptr<EventListener> listener = DukEventListener::Get(ctx, 1, eventTarget, type, false);
    if (listener)
        eventTarget->removeEventListener(type, listener.get(), useCapture);
    return 0;
}

} // namespace Impl

void DukEventTarget::FillPrototypeEntry(PrototypeEntry &entry)
{
    static const PrototypeEntry::Method Methods[] = {
        { "addEventListener",    Impl::AddEventListener,    DUK_VARARGS },
        { "removeEventListener", Impl::RemoveEventListener, DUK_VARARGS }
    };

    DukScriptObject::FillPrototypeEntry(entry);
    entry.Add(Methods, std::size(Methods));
}

} // namespace BlinKit
