// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_script_object.h
// Description: DukScriptObject Class
//      Author: Ziming Li
//     Created: 2020-01-31
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DUK_SCRIPT_OBJECT_H
#define BLINKIT_BLINK_DUK_SCRIPT_OBJECT_H

#pragma once

#include "third_party/blink/renderer/bindings/core/duk/prototype_helper.h"
#include "third_party/blink/renderer/platform/wtf/allocator.h"

namespace BlinKit {

class DukScriptObject
{
    STATIC_ONLY(DukScriptObject);
protected:
    static void FillPrototypeEntry(PrototypeEntry &entry);
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_SCRIPT_OBJECT_H
