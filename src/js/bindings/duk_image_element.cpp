// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_image_element.cpp
// Description: Bindings for Image Element
//      Author: Ziming Li
//     Created: 2019-08-28
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_image_element.h"

#include "context/prototype_manager.h"
#include "wrappers/duk.h"

using namespace blink;

namespace BlinKit {

const char DukImageElement::ProtoName[] = "HTMLImageElement";

namespace Impl {

static duk_ret_t SrcGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukEventTarget::GetNativeThis<Element>(ctx);
    return Duk::PushString(ctx, element->getURLAttribute(HTMLNames::srcAttr));
}

static duk_ret_t SrcSetter(duk_context *ctx)
{
    return DukElement::DefaultAttributeSetter(ctx, HTMLNames::srcAttr);
}

} // namespace Impl

void DukImageElement::RegisterPrototype(duk_context *ctx, PrototypeManager &protos)
{
    static const PrototypeEntry::Property Properties[] = {
        { "src", Impl::SrcGetter, Impl::SrcSetter },
    };

    const auto worker = [](PrototypeEntry &entry)
    {
        DukElement::RegisterToPrototypeEntryForCrawler(entry);
        entry.Add(Properties, WTF_ARRAY_LENGTH(Properties));
    };
    protos.Register(ctx, ProtoName, worker);
}

} // namespace BlinKit
