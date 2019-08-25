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

#ifdef BLINKIT_CRAWLER_ONLY
#   include "core/dom/Element.h"
#else
#   include "core/html/HTMLElement.h"
#endif
#include "duk_container_node.h"

namespace BlinKit {

class DukElement : public DukContainerNode
{
public:
    static const char ProtoName[];

    static void RegisterPrototypeForCrawler(duk_context *ctx, PrototypeManager &protos);
#ifndef BLINKIT_CRAWLER_ONLY
    static void RegisterPrototypeForUI(duk_context *ctx, PrototypeManager &protos);
#endif

    static duk_ret_t DefaultAttributeGetter(duk_context *ctx, const blink::QualifiedName &name);
protected:
    static void RegisterToPrototypeEntryForCrawler(PrototypeEntry &entry);
#ifndef BLINKIT_CRAWLER_ONLY
    static void RegisterToPrototypeEntryForUI(PrototypeEntry &entry);
#endif
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_ELEMENT_H
