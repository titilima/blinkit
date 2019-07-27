// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_attr.h
// Description: Bindings for Attr
//      Author: Ziming Li
//     Created: 2019-07-22
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_ATTR_H
#define BLINKIT_JS_DUK_ATTR_H

#pragma once

#include "core/dom/Attr.h"

#include "duk_node.h"

namespace BlinKit {

class DukAttr final : public DukNode
{
public:
    static const char ProtoName[];

    static void RegisterPrototype(duk_context *ctx, PrototypeManager &protos);
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_ATTR_H
