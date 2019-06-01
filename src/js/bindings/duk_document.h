// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_document.h
// Description: Bindings for Document
//      Author: Ziming Li
//     Created: 2019-06-01
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_DOCUMENT_H
#define BLINKIT_JS_DUK_DOCUMENT_H

#pragma once

#include "core/dom/Document.h"

#include "duk_node.h"

namespace BlinKit {

class DukDocument final : public DukNode
{
public:
    static const char ProtoName[];

    static void RegisterPrototypeForCrawler(duk_context *ctx, PrototypeManager &protos);
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_DOCUMENT_H
