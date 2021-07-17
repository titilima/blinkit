// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_html_collection.h
// Description: DukHTMLCollection Class
//      Author: Ziming Li
//     Created: 2020-03-07
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DUK_HTML_COLLECTION_H
#define BLINKIT_BLINK_DUK_HTML_COLLECTION_H

#pragma once

#include "blinkit/blink/renderer/bindings/core/duk/duk_array_like_object_impl.hpp"
#include "blinkit/blink/renderer/core/html/HTMLCollection.h"

namespace BlinKit {

class DukHTMLCollection final : public DukArrayLikeObjectImpl<DukHTMLCollection, blink::HTMLCollection>
{
private:
    template <class T, class N> friend class DukArrayLikeObjectImpl;
    static void PushItem(duk_context *ctx, blink::HTMLCollection *collection, duk_uarridx_t index);
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_HTML_COLLECTION_H
