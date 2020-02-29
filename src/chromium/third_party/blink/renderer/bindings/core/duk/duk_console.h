// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_console.h
// Description: DukConsole Class
//      Author: Ziming Li
//     Created: 2020-02-29
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DUK_CONSOLE_H
#define BLINKIT_BLINK_DUK_CONSOLE_H

#pragma once

#include "third_party/blink/renderer/bindings/core/duk/duk_script_object.h"

namespace BlinKit {

class DukConsole : public DukScriptObject
{
public:
    static const char ProtoName[];
    static void RegisterPrototype(PrototypeHelper &helper);
protected:
    static void FillPrototypeEntry(PrototypeEntry &entry);
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_CONSOLE_H
