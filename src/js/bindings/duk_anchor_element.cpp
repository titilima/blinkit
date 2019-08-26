// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_anchor_element.cpp
// Description: Bindings for Anchor Element
//      Author: Ziming Li
//     Created: 2019-08-25
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_anchor_element.h"

#include "core/html/parser/HTMLParserIdioms.h"

#include "context/prototype_manager.h"
#include "wrappers/duk.h"

using namespace blink;

namespace BlinKit {

const char DukAnchorElement::ProtoName[] = "HTMLAnchorElement";

namespace Impl {

static duk_ret_t HrefGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukEventTarget::GetNativeThis<Element>(ctx);

    const AtomicString &href = element->getAttribute(HTMLNames::hrefAttr);
    KURL URL = element->document().completeURL(stripLeadingAndTrailingHTMLSpaces(href));
    return Duk::PushString(ctx, URL.string());
}

static duk_ret_t HrefSetter(duk_context *ctx)
{
    return DukElement::DefaultAttributeSetter(ctx, HTMLNames::hrefAttr);
}

} // namespace Impl

void DukAnchorElement::RegisterPrototype(duk_context *ctx, PrototypeManager &protos)
{
    static const PrototypeEntry::Property Properties[] = {
        { "href", Impl::HrefGetter, Impl::HrefSetter },
    };

    const auto worker = [](PrototypeEntry &entry)
    {
        DukElement::RegisterToPrototypeEntryForCrawler(entry);
        entry.Add(Properties, WTF_ARRAY_LENGTH(Properties));
    };
    protos.Register(ctx, ProtoName, worker);
}

} // namespace BlinKit
