// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_script_element.h
// Description: DukScriptElement class
//      Author: Ziming Li
//     Created: 2020-04-01
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DUK_SCRIPT_ELEMENT_H
#define BLINKIT_BLINK_DUK_SCRIPT_ELEMENT_H

#pragma once

#include "blinkit/blink/renderer/bindings/core/duk/duk_element.h"

namespace BlinKit {

class DukScriptElement final : public DukElement
{
public:
    static const char ProtoName[];
    static void RegisterPrototypeForCrawler(PrototypeHelper &helper);
private:
    friend class DukElement;
    static void FillPrototypeEntryForCrawler(PrototypeEntry &entry);
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_SCRIPT_ELEMENT_H
