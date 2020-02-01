// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_event_target.cpp
// Description: DukEventTarget Class
//      Author: Ziming Li
//     Created: 2020-01-31
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "duk_event_target.h"

namespace BlinKit {

void DukEventTarget::FillPrototypeEntry(PrototypeEntry &entry)
{
    DukScriptObject::FillPrototypeEntry(entry);
}

} // namespace BlinKit
