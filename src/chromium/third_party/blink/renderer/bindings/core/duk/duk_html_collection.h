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

#include "third_party/blink/renderer/bindings/core/duk/duk_script_object.h"
#include "third_party/blink/renderer/core/html/html_collection.h"

namespace BlinKit {

class DukHTMLCollection : public DukScriptObject
{
public:
    static duk_idx_t Push(duk_context *ctx, blink::HTMLCollection *collection);
private:
    static void InitializeProxy(duk_context *ctx);
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_HTML_COLLECTION_H
