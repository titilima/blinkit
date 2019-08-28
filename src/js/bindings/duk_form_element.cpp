// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_form_element.cpp
// Description: Bindings for Form Element
//      Author: Ziming Li
//     Created: 2019-08-25
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_form_element.h"

#include "blinkit/crawler/crawler_form_element.h"

#include "context/prototype_manager.h"

namespace BlinKit {

const char DukFormElement::ProtoName[] = "HTMLFormElement";

namespace Impl {

static duk_ret_t Submit(duk_context *ctx)
{
    duk_push_this(ctx);
    CrawlerFormElement *form = DukEventTarget::GetNativeThis<CrawlerFormElement>(ctx);
    form->Submit();
    return 0;
}

} // namespace Impl

void DukFormElement::RegisterPrototype(duk_context *ctx, PrototypeManager &protos)
{
    static const PrototypeEntry::Method Methods[] = {
        { "submit", Impl::Submit, 0 },
    };

    const auto worker = [](PrototypeEntry &entry)
    {
        DukElement::RegisterToPrototypeEntryForCrawler(entry);
        entry.Add(Methods, WTF_ARRAY_LENGTH(Methods));
    };
    protos.Register(ctx, ProtoName, worker);
}

} // namespace BlinKit
