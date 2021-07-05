// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_event_target.h
// Description: DukEventTarget Class
//      Author: Ziming Li
//     Created: 2020-01-31
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DUK_EVENT_TARGET_H
#define BLINKIT_BLINK_DUK_EVENT_TARGET_H

#pragma once

#include "blinkit/blink/renderer/bindings/core/duk/duk_script_object.h"
#include "blinkit/blink/renderer/core/events/EventTarget.h"

namespace BlinKit {

class DukEventTarget : public DukScriptObject
{
protected:
    static void FillPrototypeEntry(PrototypeEntry &entry);
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_EVENT_TARGET_H
