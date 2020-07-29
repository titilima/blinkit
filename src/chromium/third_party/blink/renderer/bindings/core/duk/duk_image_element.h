// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_image_element.h
// Description: DukImageElement class
//      Author: Ziming Li
//     Created: 2020-07-29
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DUK_IMAGE_ELEMENT_H
#define BLINKIT_BLINK_DUK_IMAGE_ELEMENT_H

#pragma once

#include "third_party/blink/renderer/bindings/core/duk/duk_element.h"

namespace BlinKit {

class DukImageElement final : public DukElement
{
public:
    static const char ProtoName[];
    static void RegisterPrototypeForCrawler(PrototypeHelper &helper);
private:
    friend class DukElement;
    static void FillPrototypeEntryForCrawler(PrototypeEntry &entry);
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_IMAGE_ELEMENT_H
