// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_form_element.h
// Description: Bindings for Form Element
//      Author: Ziming Li
//     Created: 2019-08-25
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_FORM_ELEMENT_H
#define BLINKIT_JS_DUK_FORM_ELEMENT_H

#pragma once

#include "duk_element.h"

namespace BlinKit {

class DukFormElement final : public DukElement
{
public:
    static const char ProtoName[];

    static void RegisterPrototype(duk_context *ctx, PrototypeManager &protos);
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_FORM_ELEMENT_H
