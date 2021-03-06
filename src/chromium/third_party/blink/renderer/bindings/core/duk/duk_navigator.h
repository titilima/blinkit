// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_navigator.h
// Description: DukNavigator Class
//      Author: Ziming Li
//     Created: 2020-03-25
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DUK_NAVIGATOR_H
#define BLINKIT_BLINK_DUK_NAVIGATOR_H

#pragma once

#include "third_party/blink/renderer/bindings/core/duk/duk_script_object.h"
#include "third_party/blink/renderer/core/frame/navigator.h"

namespace BlinKit {

class DukNavigator final : public DukScriptObject
{
public:
    static const char ProtoName[];
    static void RegisterPrototype(PrototypeHelper &helper);
private:
    static void FillPrototypeEntry(PrototypeEntry &entry);
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_NAVIGATOR_H
