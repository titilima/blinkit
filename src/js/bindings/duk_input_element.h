// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_input_element.h
// Description: Bindings for Input Element
//      Author: Ziming Li
//     Created: 2019-07-27
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_INPUT_ELEMENT_H
#define BLINKIT_JS_DUK_INPUT_ELEMENT_H

#pragma once

#ifndef BLINKIT_CRAWLER_ONLY
#   include "core/html/HTMLInputElement.h"
#endif
#include "duk_element.h"

namespace BlinKit {

class DukInputElement final : public DukElement
{
public:
    static const char ProtoName[];

    static void RegisterPrototypeForCrawler(duk_context *ctx, PrototypeManager &protos);
#ifndef BLINKIT_CRAWLER_ONLY
    static void RegisterPrototypeForUI(duk_context *ctx, PrototypeManager &protos);
#endif
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_INPUT_ELEMENT_H
