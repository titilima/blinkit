// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_element.h
// Description: DukElement Class
//      Author: Ziming Li
//     Created: 2020-02-11
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DUK_ELEMENT_H
#define BLINKIT_BLINK_DUK_ELEMENT_H

#pragma once

#include "third_party/blink/renderer/bindings/core/duk/duk_container_node.h"
#include "third_party/blink/renderer/core/dom/element.h"

namespace BlinKit {

class DukElement : public DukContainerNode
{
public:
    static const char ProtoName[];
    static void RegisterPrototypeForCrawler(PrototypeHelper &helper);
private:
    static void FillPrototypeEntryForCrawler(PrototypeEntry &entry);
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_ELEMENT_H
