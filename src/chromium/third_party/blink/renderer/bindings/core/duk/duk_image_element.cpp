// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_image_element.cpp
// Description: DukImageElement class
//      Author: Ziming Li
//     Created: 2020-07-29
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "duk_image_element.h"

#include "third_party/blink/renderer/bindings/core/duk/duk.h"
#include "third_party/blink/renderer/core/html_names.h"

using namespace blink;

namespace BlinKit {

const char DukImageElement::ProtoName[] = "HTMLImageElement";

namespace Impl {

static duk_ret_t SrcGetter(duk_context *ctx)
{
    return DukElement::GetURLAttributeImpl(ctx, html_names::kSrcAttr);
}

static duk_ret_t SrcSetter(duk_context *ctx)
{
    return DukElement::SetAttributeImpl(ctx, html_names::kSrcAttr);
}

} // namespace Impl

void DukImageElement::FillPrototypeEntryForCrawler(PrototypeEntry &entry)
{
    static const PrototypeEntry::Property Properties[] = {
        { "src", Impl::SrcGetter, Impl::SrcSetter },
    };

    DukElement::FillPrototypeEntryForCrawler(entry);
    entry.Add(Properties, std::size(Properties));
}

void DukImageElement::RegisterPrototypeForCrawler(PrototypeHelper &helper)
{
    helper.Register(ProtoName, FillPrototypeEntryForCrawler);
}

} // namespace BlinKit
