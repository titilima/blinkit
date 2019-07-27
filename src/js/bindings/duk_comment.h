// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_comment.h
// Description: Bindings for Comment
//      Author: Ziming Li
//     Created: 2019-07-26
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_COMMENT_H
#define BLINKIT_JS_DUK_COMMENT_H

#pragma once

#include "core/dom/Comment.h"

#include "duk_node.h"

namespace BlinKit {

class DukComment final : public DukNode
{
public:
    static const char ProtoName[];

    static void RegisterPrototype(duk_context *ctx, PrototypeManager &protos);
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_COMMENT_H
