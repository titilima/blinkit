// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_html_collection.cpp
// Description: DukHTMLCollection Class
//      Author: Ziming Li
//     Created: 2020-03-07
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "./duk_html_collection.h"

#include "blinkit/blink/renderer/bindings/core/duk/duk_element.h"

using namespace blink;

namespace BlinKit {

void DukHTMLCollection::PushItem(duk_context *ctx, HTMLCollection *collection, duk_uarridx_t index)
{
    DukElement::Push(ctx, collection->item(index));
}

} // namespace BlinKit
