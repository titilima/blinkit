// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_location.h
// Description: DukLocation Class
//      Author: Ziming Li
//     Created: 2020-03-26
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DUK_LOCATION_H
#define BLINKIT_BLINK_DUK_LOCATION_H

#pragma once

#include "third_party/blink/renderer/bindings/core/duk/duk_script_object.h"
#include "third_party/blink/renderer/core/frame/location.h"

namespace BlinKit {

class DukLocation final : public DukScriptObject
{
public:
    static const char ProtoName[];
    static void RegisterPrototype(PrototypeHelper &helper);
private:
    static void FillPrototypeEntry(PrototypeEntry &entry);
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_LOCATION_H
