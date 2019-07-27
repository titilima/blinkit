// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_mouse_event.cpp
// Description: Bindings for MouseEvent
//      Author: Ziming Li
//     Created: 2019-07-27
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_mouse_event.h"

#include "context/duk_context.h"
#include "context/prototype_manager.h"
#include "wrappers/duk.h"

using namespace blink;

namespace BlinKit {

const char DukMouseEvent::ProtoName[] = "MouseEvent";

namespace Impl {

static duk_ret_t ButtonGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    MouseEvent *e = DukEvent::GetNativeThis<MouseEvent>(ctx);

    duk_push_int(ctx, e->button());
    return 1;
}

static duk_ret_t ButtonsGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    MouseEvent *e = DukEvent::GetNativeThis<MouseEvent>(ctx);

    duk_push_uint(ctx, e->buttons());
    return 1;
}

static duk_ret_t ClientXGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    MouseEvent *e = DukEvent::GetNativeThis<MouseEvent>(ctx);

    duk_push_int(ctx, e->clientX());
    return 1;
}

static duk_ret_t ClientYGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    MouseEvent *e = DukEvent::GetNativeThis<MouseEvent>(ctx);

    duk_push_int(ctx, e->clientY());
    return 1;
}

static duk_ret_t MovementXGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    MouseEvent *e = DukEvent::GetNativeThis<MouseEvent>(ctx);

    duk_push_int(ctx, e->movementX());
    return 1;
}

static duk_ret_t MovementYGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    MouseEvent *e = DukEvent::GetNativeThis<MouseEvent>(ctx);

    duk_push_int(ctx, e->movementY());
    return 1;
}

static duk_ret_t RelatedTargetGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    MouseEvent *e = DukEvent::GetNativeThis<MouseEvent>(ctx);

    return DukContext::From(ctx)->PushEventTarget(ctx, e->relatedTarget());
}

static duk_ret_t ScreenXGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    MouseEvent *e = DukEvent::GetNativeThis<MouseEvent>(ctx);

    duk_push_int(ctx, e->screenX());
    return 1;
}

static duk_ret_t ScreenYGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    MouseEvent *e = DukEvent::GetNativeThis<MouseEvent>(ctx);

    duk_push_int(ctx, e->screenY());
    return 1;
}

static duk_ret_t XGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    MouseEvent *e = DukEvent::GetNativeThis<MouseEvent>(ctx);

    duk_push_int(ctx, e->x());
    return 1;
}

static duk_ret_t YGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    MouseEvent *e = DukEvent::GetNativeThis<MouseEvent>(ctx);

    duk_push_int(ctx, e->y());
    return 1;
}

} // namespace Impl

void DukMouseEvent::RegisterPrototype(duk_context *ctx, PrototypeManager &protos)
{
    static const PrototypeEntry::Property Properties[] = {
        { "button",        Impl::ButtonGetter,        nullptr },
        { "buttons",       Impl::ButtonsGetter,       nullptr },
        { "clientX",       Impl::ClientXGetter,       nullptr },
        { "clientY",       Impl::ClientYGetter,       nullptr },
        { "movementX",     Impl::MovementXGetter,     nullptr },
        { "movementY",     Impl::MovementYGetter,     nullptr },
        { "relatedTarget", Impl::RelatedTargetGetter, nullptr },
        { "screenX",       Impl::ScreenXGetter,       nullptr },
        { "screenY",       Impl::ScreenYGetter,       nullptr },
        { "x",             Impl::XGetter,             nullptr },
        { "y",             Impl::YGetter,             nullptr },
    };

    const auto worker = [](PrototypeEntry &entry)
    {
        DukUIEventWithKeyState::RegisterToPrototypeEntry(entry);
        entry.Add(Properties, WTF_ARRAY_LENGTH(Properties));
    };
    protos.Register(ctx, ProtoName, worker);
}

} // namespace BlinKit
