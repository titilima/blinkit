// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_document_fragment.cpp
// Description: Bindings for DocumentFragment
//      Author: Ziming Li
//     Created: 2019-07-21
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_document_fragment.h"

#include "context/prototype_manager.h"

namespace BlinKit {

const char DukDocumentFragment::ProtoName[] = "DocumentFragment";

void DukDocumentFragment::RegisterPrototype(duk_context *ctx, PrototypeManager &protos)
{
    const auto worker = [](PrototypeEntry &entry)
    {
        DukContainerNode::RegisterToPrototypeEntry(entry);
    };
    protos.Register(ctx, ProtoName, worker);
}

} // namespace BlinKit
