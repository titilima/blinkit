// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_document_fragment.h
// Description: Bindings for DocumentFragment
//      Author: Ziming Li
//     Created: 2019-07-21
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_DOCUMENT_FRAGMENT_H
#define BLINKIT_JS_DUK_DOCUMENT_FRAGMENT_H

#pragma once

#include "core/dom/DocumentFragment.h"
#include "duk_container_node.h"

namespace BlinKit {

class DukDocumentFragment final : public DukContainerNode
{
public:
    static const char ProtoName[];

    static void RegisterPrototype(duk_context *ctx, PrototypeManager &protos);
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_DOCUMENT_FRAGMENT_H
