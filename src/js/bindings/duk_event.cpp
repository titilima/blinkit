// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_event.cpp
// Description: Bindings for Event
//      Author: Ziming Li
//     Created: 2019-07-26
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_event.h"

#include "context/duk_context.h"
#include "context/prototype_manager.h"
#include "wrappers/duk.h"

using namespace blink;

namespace BlinKit {

const char DukEvent::ProtoName[] = "Event";

duk_ret_t DukEvent::Finalizer(duk_context *ctx)
{
    Event *nativeThis = Get(ctx, 0);
    DukContext::From(ctx)->RemoveObjectFromPool(nativeThis);
    return 0;
}

Event* DukEvent::Get(duk_context *ctx, duk_idx_t idx)
{
    return static_cast<Event *>(Duk::GetNativeThis(ctx, idx));
}

namespace Impl {

static duk_ret_t BubblesGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Event *e = DukEvent::GetNativeThis<Event>(ctx);

    duk_push_boolean(ctx, e->bubbles());
    return 1;
}

static duk_ret_t CancelableGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Event *e = DukEvent::GetNativeThis<Event>(ctx);

    duk_push_boolean(ctx, e->cancelable());
    return 1;
}

static duk_ret_t CancelBubbleGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Event *e = DukEvent::GetNativeThis<Event>(ctx);

    duk_push_boolean(ctx, e->cancelBubble());
    return 1;
}

static duk_ret_t CancelBubbleSetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Event *e = DukEvent::GetNativeThis<Event>(ctx);

    e->setCancelBubble(duk_to_boolean(ctx, 0));
    return 0;
}

static duk_ret_t CurrentTargetGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Event *e = DukEvent::GetNativeThis<Event>(ctx);

    return DukContext::From(ctx)->PushEventTarget(ctx, e->currentTarget());
}

static duk_ret_t DefaultPreventedGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Event *e = DukEvent::GetNativeThis<Event>(ctx);

    duk_push_boolean(ctx, e->defaultPrevented());
    return 1;
}

static duk_ret_t EventPhaseGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Event *e = DukEvent::GetNativeThis<Event>(ctx);

    duk_push_uint(ctx, e->eventPhase());
    return 1;
}

static duk_ret_t PreventDefault(duk_context *ctx)
{
    duk_push_this(ctx);
    Event *e = DukEvent::GetNativeThis<Event>(ctx);

    e->preventDefault();
    return 0;
}

static duk_ret_t ReturnValueGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Event *e = DukEvent::GetNativeThis<Event>(ctx);

    duk_push_boolean(ctx, !e->defaultPrevented());
    return 1;
}

static duk_ret_t ReturnValueSetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Event *e = DukEvent::GetNativeThis<Event>(ctx);

    e->setDefaultPrevented(!duk_to_boolean(ctx, 0));
    return 0;
}

static duk_ret_t StopImmediatePropagation(duk_context *ctx)
{
    duk_push_this(ctx);
    Event *e = DukEvent::GetNativeThis<Event>(ctx);

    e->stopImmediatePropagation();
    return 0;
}

static duk_ret_t StopPropagation(duk_context *ctx)
{
    duk_push_this(ctx);
    Event *e = DukEvent::GetNativeThis<Event>(ctx);

    e->stopPropagation();
    return 0;
}

static duk_ret_t TargetGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Event *e = DukEvent::GetNativeThis<Event>(ctx);

    return DukContext::From(ctx)->PushEventTarget(ctx, e->target());
}

static duk_ret_t TimeStampGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Event *e = DukEvent::GetNativeThis<Event>(ctx);

    duk_push_number(ctx, e->platformTimeStamp());
    return 1;
}

static duk_ret_t TypeGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Event *e = DukEvent::GetNativeThis<Event>(ctx);

    return Duk::PushString(ctx, e->type());
}

} // namespace Impl

void DukEvent::RegisterPrototype(duk_context *ctx, PrototypeManager &protos)
{
    const auto worker = [](PrototypeEntry &entry)
    {
        RegisterToPrototypeEntry(entry);
    };
    protos.Register(ctx, ProtoName, worker);
}

void DukEvent::RegisterToPrototypeEntry(PrototypeEntry &entry)
{
    static const PrototypeEntry::Property Properties[] = {
        { "bubbles",          Impl::BubblesGetter,          nullptr                  },
        { "cancelable",       Impl::CancelableGetter,       nullptr                  },
        { "cancelBubble",     Impl::CancelBubbleGetter,     Impl::CancelBubbleSetter },
        { "currentTarget",    Impl::CurrentTargetGetter,    nullptr                  },
        { "defaultPrevented", Impl::DefaultPreventedGetter, nullptr                  },
        { "eventPhase",       Impl::EventPhaseGetter,       nullptr                  },
        { "returnValue",      Impl::ReturnValueGetter,      Impl::ReturnValueSetter  },
        { "target",           Impl::TargetGetter,           nullptr                  },
        { "timeStamp",        Impl::TimeStampGetter,        nullptr                  },
        { "type",             Impl::TypeGetter,             nullptr                  },
    };
    static const PrototypeEntry::Method Methods[] = {
        { "preventDefault",           Impl::PreventDefault,           0 },
        { "stopImmediatePropagation", Impl::StopImmediatePropagation, 0 },
        { "stopPropagation",          Impl::StopPropagation,          0 },
    };

    entry.SetFinalizer(Finalizer);
    entry.Add(Properties, WTF_ARRAY_LENGTH(Properties));
    entry.Add(Methods, WTF_ARRAY_LENGTH(Methods));
}

} // namespace BlinKit
