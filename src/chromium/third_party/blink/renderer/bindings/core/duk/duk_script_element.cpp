// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_script_element.cpp
// Description: DukScriptElement class
//      Author: Ziming Li
//     Created: 2020-04-01
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "duk_script_element.h"

#include "third_party/blink/renderer/bindings/core/duk/duk.h"
#include "third_party/blink/renderer/core/html_names.h"

using namespace blink;

namespace BlinKit {

const char DukScriptElement::ProtoName[] = "HTMLScriptElement";

namespace Impl {

static duk_ret_t SrcGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukScriptObject::To<Element>(ctx, -1);

    GURL ret = element->GetURLAttribute(html_names::kSrcAttr);
    Duk::PushString(ctx, ret.spec());
    return 1;
}

static duk_ret_t SrcSetter(duk_context *ctx)
{
    const AtomicString src = Duk::To<AtomicString>(ctx, 0);

    duk_push_this(ctx);
    Element *element = DukScriptObject::To<Element>(ctx, -1);

    DukExceptionState exceptionState(ctx);
    element->setAttribute(html_names::kSrcAttr.LocalName(), src, exceptionState);
    if (exceptionState.HadException())
        exceptionState.ThrowIfNeeded();
    return 0;
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
