// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_comment.cpp
// Description: Bindings for Comment
//      Author: Ziming Li
//     Created: 2019-07-26
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_comment.h"

#include "context/prototype_manager.h"

using namespace blink;

namespace BlinKit {

const char DukComment::ProtoName[] = "Comment";

void DukComment::RegisterPrototype(duk_context *ctx, PrototypeManager &protos)
{
    const auto worker = [](PrototypeEntry &entry)
    {
        DukNode::RegisterToPrototypeEntry(entry);
    };
    protos.Register(ctx, ProtoName, worker);
}

} // namespace BlinKit
