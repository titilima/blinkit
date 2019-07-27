// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_ui_event_with_key_state.h
// Description: Bindings for UIEventWithKeyState
//      Author: Ziming Li
//     Created: 2019-07-27
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_UI_EVENT_WITH_KEY_STATE_H
#define BLINKIT_JS_DUK_UI_EVENT_WITH_KEY_STATE_H

#pragma once

#include "core/events/UIEventWithKeyState.h"
#include "duk_ui_event.h"

namespace BlinKit {

class DukUIEventWithKeyState : public DukUIEvent
{
protected:
    static void RegisterToPrototypeEntry(PrototypeEntry &entry);
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_UI_EVENT_WITH_KEY_STATE_H
