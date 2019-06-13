// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_text.h
// Description: Bindings for Text
//      Author: Ziming Li
//     Created: 2019-06-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_TEXT_H
#define BLINKIT_JS_DUK_TEXT_H

#pragma once

#include "core/dom/Text.h"

#include "duk_node.h"

namespace BlinKit {

class DukText final : public DukNode
{
public:
    static const char ProtoName[];

    static void RegisterPrototype(duk_context *ctx, PrototypeManager &protos);
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_TEXT_H
