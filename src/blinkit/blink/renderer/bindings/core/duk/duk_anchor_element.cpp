// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_anchor_element.cpp
// Description: DukAnchorElement class
//      Author: Ziming Li
//     Created: 2020-07-10
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "./duk_anchor_element.h"

#include "blinkit/blink/renderer/bindings/core/duk/duk.h"
#include "blinkit/blink/renderer/core/HTMLNames.h"

using namespace blink;

namespace BlinKit {

const char DukAnchorElement::ProtoName[] = "HTMLAnchorElement";

namespace Impl {

static duk_ret_t HrefGetter(duk_context *ctx)
{
    return DukElement::GetURLAttributeImpl(ctx, HTMLNames::hrefAttr);
}

static duk_ret_t HrefSetter(duk_context *ctx)
{
    return DukElement::SetAttributeImpl(ctx, HTMLNames::hrefAttr);
}

} // namespace Impl

void DukAnchorElement::FillPrototypeEntryForCrawler(PrototypeEntry &entry)
{
    static const PrototypeEntry::Property Properties[] = {
        { "href", Impl::HrefGetter, Impl::HrefSetter },
    };

    DukElement::FillPrototypeEntryForCrawler(entry);
    entry.Add(Properties, std::size(Properties));
}

void DukAnchorElement::RegisterPrototypeForCrawler(PrototypeHelper &helper)
{
    helper.Register(ProtoName, FillPrototypeEntryForCrawler);
}

} // namespace BlinKit
