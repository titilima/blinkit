// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_ui_event.h
// Description: Bindings for UIEvent
//      Author: Ziming Li
//     Created: 2019-07-27
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_UI_EVENT_H
#define BLINKIT_JS_DUK_UI_EVENT_H

#pragma once

#include "core/events/UIEvent.h"
#include "duk_event.h"

namespace BlinKit {

class DukUIEvent : public DukEvent
{
protected:
    static void RegisterToPrototypeEntry(PrototypeEntry &entry);
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_UI_EVENT_H
