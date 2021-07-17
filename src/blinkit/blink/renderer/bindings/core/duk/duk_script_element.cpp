// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: duk_script_element.cpp
// Description: DukScriptElement class
//      Author: Ziming Li
//     Created: 2020-04-01
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "./duk_script_element.h"

#include "blinkit/blink/renderer/bindings/core/duk/duk.h"
#include "blinkit/blink/renderer/core/HTMLNames.h"

using namespace blink;

namespace BlinKit {

const char DukScriptElement::ProtoName[] = "HTMLScriptElement";

namespace Impl {

static duk_ret_t SrcGetter(duk_context *ctx)
{
    return DukElement::GetURLAttributeImpl(ctx, HTMLNames::srcAttr);
}

static duk_ret_t SrcSetter(duk_context *ctx)
{
    return DukElement::SetAttributeImpl(ctx, HTMLNames::srcAttr);
}

} // namespace Impl

void DukScriptElement::FillPrototypeEntryForCrawler(PrototypeEntry &entry)
{
    static const PrototypeEntry::Property Properties[] = {
        { "src", Impl::SrcGetter, Impl::SrcSetter },
    };

    DukElement::FillPrototypeEntryForCrawler(entry);
    entry.Add(Properties, std::size(Properties));
}

void DukScriptElement::RegisterPrototypeForCrawler(PrototypeHelper &helper)
{
    helper.Register(ProtoName, FillPrototypeEntryForCrawler);
}

} // namespace BlinKit
