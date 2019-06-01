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

#include "core/events/EventTarget.h"

#include "context/prototype_manager.h"
#include "wrappers/duk.h"

using namespace blink;

namespace BlinKit {

duk_ret_t DukEventTarget::Finalizer(duk_context *ctx)
{
    EventTarget *nativeThis = Get(ctx, 0);
    nativeThis->deref();
    return 0;
}

EventTarget* DukEventTarget::Get(duk_context *ctx, duk_idx_t idx)
{
    return reinterpret_cast<EventTarget *>(Duk::GetNativeThis(ctx, idx));
}

void DukEventTarget::OnCreate(ScriptWrappable *nativeThis)
{
    reinterpret_cast<EventTarget *>(nativeThis)->ref();
}

void DukEventTarget::RegisterToPrototypeEntry(PrototypeEntry &entry)
{
    entry.SetFinalizer(Finalizer);
}

} // namespace BlinKit
