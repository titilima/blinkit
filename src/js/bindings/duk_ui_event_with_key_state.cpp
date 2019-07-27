// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_ui_event_with_key_state.cpp
// Description: Bindings for UIEvent
//      Author: Ziming Li
//     Created: 2019-07-27
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_ui_event_with_key_state.h"

#include "context/prototype_manager.h"
#include "wrappers/duk.h"

using namespace blink;

namespace BlinKit {

namespace Impl {

static duk_ret_t AltKeyGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    UIEventWithKeyState *e = DukEvent::GetNativeThis<UIEventWithKeyState>(ctx);

    duk_push_boolean(ctx, e->altKey());
    return 1;
}

static duk_ret_t CtrlKeyGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    UIEventWithKeyState *e = DukEvent::GetNativeThis<UIEventWithKeyState>(ctx);

    duk_push_boolean(ctx, e->ctrlKey());
    return 1;
}

static duk_ret_t GetModifierState(duk_context *ctx)
{
    duk_push_this(ctx);
    UIEventWithKeyState *e = DukEvent::GetNativeThis<UIEventWithKeyState>(ctx);

    duk_push_boolean(ctx, e->getModifierState(Duk::ToWTFString(ctx, 0)));
    return 1;
}

static duk_ret_t MetaKeyGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    UIEventWithKeyState *e = DukEvent::GetNativeThis<UIEventWithKeyState>(ctx);

    duk_push_boolean(ctx, e->metaKey());
    return 1;
}

static duk_ret_t ShiftKeyGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    UIEventWithKeyState *e = DukEvent::GetNativeThis<UIEventWithKeyState>(ctx);

    duk_push_boolean(ctx, e->shiftKey());
    return 1;
}

} // namespace Impl

void DukUIEventWithKeyState::RegisterToPrototypeEntry(PrototypeEntry &entry)
{
    static const PrototypeEntry::Property Properties[] = {
        { "altKey",   Impl::AltKeyGetter,   nullptr },
        { "ctrlKey",  Impl::CtrlKeyGetter,  nullptr },
        { "metaKey",  Impl::MetaKeyGetter,  nullptr },
        { "shiftKey", Impl::ShiftKeyGetter, nullptr }
    };
    static const PrototypeEntry::Method Methods[] = {
        { "getModifierState", Impl::GetModifierState, 1 },
    };

    DukUIEvent::RegisterToPrototypeEntry(entry);
    entry.Add(Properties, WTF_ARRAY_LENGTH(Properties));
    entry.Add(Methods, WTF_ARRAY_LENGTH(Methods));
}

} // namespace BlinKit
