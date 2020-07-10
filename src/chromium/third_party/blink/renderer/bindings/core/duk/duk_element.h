// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_element.h
// Description: DukElement Class
//      Author: Ziming Li
//     Created: 2020-02-11
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DUK_ELEMENT_H
#define BLINKIT_BLINK_DUK_ELEMENT_H

#pragma once

#include <unordered_map>
#include "third_party/blink/renderer/bindings/core/duk/duk_container_node.h"
#include "third_party/blink/renderer/core/dom/element.h"

namespace BlinKit {

class DukElement : DukContainerNode
{
public:
    static const char ProtoName[];
    static void RegisterPrototypeForCrawler(PrototypeHelper &helper);

    static duk_idx_t Push(duk_context *ctx, blink::Element *element);

    static duk_ret_t GetURLAttributeImpl(duk_context *ctx, const blink::QualifiedName &name);
    static duk_ret_t SetAttributeImpl(duk_context *ctx, const blink::QualifiedName &name);
    static duk_ret_t SetAttributeEventListenerImpl(duk_context *ctx, const WTF::AtomicString &attrName);

    static const std::unordered_map<std::string, const char *>& PrototypeMapForCrawler(void);
#ifndef BLINKIT_CRAWLER_ONLY
    static const std::unordered_map<std::string, const char *>& PrototypeMapForUI(void);
#endif
protected:
    static void FillPrototypeEntryForCrawler(PrototypeEntry &entry);
private:
    friend class DukNode;
    static void Create(duk_context *ctx, blink::Element &element);
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_ELEMENT_H
