// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_anchor_element.h
// Description: DukAnchorElement class
//      Author: Ziming Li
//     Created: 2020-07-10
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DUK_ANCHOR_ELEMENT_H
#define BLINKIT_BLINK_DUK_ANCHOR_ELEMENT_H

#pragma once

#include "blinkit/blink/renderer/bindings/core/duk/duk_element.h"

namespace BlinKit {

class DukAnchorElement final : public DukElement
{
public:
    static const char ProtoName[];
    static void RegisterPrototypeForCrawler(PrototypeHelper &helper);
private:
    friend class DukElement;
    static void FillPrototypeEntryForCrawler(PrototypeEntry &entry);
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_ANCHOR_ELEMENT_H
