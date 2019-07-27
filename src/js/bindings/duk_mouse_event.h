// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_mouse_event.h
// Description: Bindings for MouseEvent
//      Author: Ziming Li
//     Created: 2019-07-27
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_MOUSE_EVENT_H
#define BLINKIT_JS_DUK_MOUSE_EVENT_H

#pragma once

#include "core/events/MouseEvent.h"
#include "duk_ui_event_with_key_state.h"

namespace BlinKit {

class DukMouseEvent final : public DukUIEventWithKeyState
{
public:
    static const char ProtoName[];

    static void RegisterPrototype(duk_context *ctx, PrototypeManager &protos);
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_MOUSE_EVENT_H
