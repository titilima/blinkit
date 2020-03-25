// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_attr.h
// Description: DukAttr class
//      Author: Ziming Li
//     Created: 2019-07-22
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DUK_ATTR_H
#define BLINKIT_BLINK_DUK_ATTR_H

#pragma once

#include "third_party/blink/renderer/bindings/core/duk/duk_node.h"
#include "third_party/blink/renderer/core/dom/attr.h"

namespace BlinKit {

class DukAttr final : public DukNode
{
public:
    static const char ProtoName[];
    static void RegisterPrototype(PrototypeHelper &helper);
private:
    friend class DukNode;
    static void FillPrototypeEntry(PrototypeEntry &entry);
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_ATTR_H
