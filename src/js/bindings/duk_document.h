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

#ifdef BLINKIT_CRAWLER_ONLY
#   include "core/dom/Document.h"
#else
#   include "core/html/HTMLDocument.h"
#endif

#include "duk_container_node.h"

namespace BlinKit {

class DukDocument final : public DukContainerNode
{
public:
    static const char ProtoName[];

    static void RegisterPrototypeForCrawler(duk_context *ctx, PrototypeManager &protos);
#ifndef BLINKIT_CRAWLER_ONLY
    static void RegisterPrototypeForUI(duk_context *ctx, PrototypeManager &protos);
#endif
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_DOCUMENT_H
