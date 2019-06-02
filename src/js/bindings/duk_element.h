// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_element.h
// Description: Bindings for Element
//      Author: Ziming Li
//     Created: 2019-06-02
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_ELEMENT_H
#define BLINKIT_JS_DUK_ELEMENT_H

#pragma once

#include "core/dom/Element.h"

#include "duk_node.h"

namespace BlinKit {

class DukElement final : public DukNode
{
public:
    static const char ProtoName[];

    static void RegisterPrototypeForCrawler(duk_context *ctx, PrototypeManager &protos);
};

} // namespace BlinKit

#endif
