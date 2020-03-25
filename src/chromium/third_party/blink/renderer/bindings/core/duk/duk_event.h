// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_event.h
// Description: DukEvent Class
//      Author: Ziming Li
//     Created: 2020-03-21
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DUK_EVENT_H
#define BLINKIT_BLINK_DUK_EVENT_H

#pragma once

#include "third_party/blink/renderer/bindings/core/duk/duk_script_object.h"
#include "third_party/blink/renderer/core/dom/events/event.h"

namespace BlinKit {

class DukEvent : public DukScriptObject
{
public:
    static const char ProtoName[];
    static void RegisterPrototype(PrototypeHelper &helper);

    static duk_idx_t Push(duk_context *ctx, blink::Event *event);
private:
    static void FillPrototypeEntry(PrototypeEntry &entry);
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_EVENT_H
