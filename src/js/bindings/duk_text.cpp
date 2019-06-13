// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_text.cpp
// Description: Bindings for Text
//      Author: Ziming Li
//     Created: 2019-06-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_text.h"

#include "context/prototype_manager.h"

using namespace blink;

namespace BlinKit {

const char DukText::ProtoName[] = "Text";

void DukText::RegisterPrototype(duk_context *ctx, PrototypeManager &protos)
{
    const auto worker = [](PrototypeEntry &entry)
    {
        DukNode::RegisterToPrototypeEntry(entry);
    };
    protos.Register(ctx, ProtoName, worker);
}

} // namespace BlinKit
