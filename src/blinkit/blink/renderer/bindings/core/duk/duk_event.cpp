// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_event.cpp
// Description: DukEvent Class
//      Author: Ziming Li
//     Created: 2020-03-21
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "./duk_event.h"

#include "blinkit/blink/renderer/bindings/core/duk/script_wrappers.h"

using namespace blink;

namespace BlinKit {

const char DukEvent::ProtoName[] = "Event";

void DukEvent::FillPrototypeEntry(PrototypeEntry &entry)
{
    DukScriptObject::FillPrototypeEntry(entry);
}

duk_idx_t DukEvent::Push(duk_context *ctx, Event *event)
{
    PushWrapper w(ctx, event);
    do {
        if (DukScriptObject::Push(ctx, event))
            break;

        ASSERT(nullptr != event);
        PrototypeHelper::CreateScriptObject(ctx, ProtoName, event);
    } while (false);
    return duk_get_top_index(ctx);
}

void DukEvent::RegisterPrototype(PrototypeHelper &helper)
{
    helper.Register(ProtoName, FillPrototypeEntry);
}

} // namespace BlinKit
