// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: duk_dom_token_list.h
// Description: DukDOMTokenList Class
//      Author: Ziming Li
//     Created: 2021-11-16
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_DUK_DOM_TOKEN_LIST_H
#define BLINKIT_BLINKIT_DUK_DOM_TOKEN_LIST_H

#pragma once

#include "blinkit/blink/renderer/bindings/core/duk/duk_script_object.h"
#include "blinkit/blink/renderer/core/dom/DOMTokenList.h"

namespace BlinKit {

class DukDOMTokenList : public DukScriptObject
{
public:
    static const char ProtoName[];
    static void RegisterPrototype(PrototypeHelper &helper);

    static duk_idx_t Push(duk_context *ctx, blink::DOMTokenList &tokenList);
private:
    static void FillPrototypeEntry(PrototypeEntry &entry);
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_DUK_DOM_TOKEN_LIST_H
