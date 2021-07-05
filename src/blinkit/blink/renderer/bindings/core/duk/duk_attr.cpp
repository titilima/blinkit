// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_attr.cpp
// Description: DukAttr class
//      Author: Ziming Li
//     Created: 2019-07-22
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "./duk_attr.h"

#include "blinkit/blink/renderer/bindings/core/duk/duk.h"

using namespace blink;

namespace BlinKit {

const char DukAttr::ProtoName[] = "Attr";

namespace Impl {

static duk_ret_t NameGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Attr *attr = DukScriptObject::To<Attr>(ctx, -1);
    Duk::PushString(ctx, attr->name());
    return 1;
}

static duk_ret_t OwnerElementGetter(duk_context *ctx)
{
    ASSERT(false); // BKTODO:
    return 0;
}

static duk_ret_t PrefixGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Attr *attr = DukScriptObject::To<Attr>(ctx, -1);
    Duk::PushString(ctx, attr->prefix());
    return 1;
}

static duk_ret_t SpecifiedGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Attr *attr = DukScriptObject::To<Attr>(ctx, -1);
    duk_push_boolean(ctx, attr->specified());
    return 0;
}

static duk_ret_t ValueGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Attr *attr = DukScriptObject::To<Attr>(ctx, -1);
    Duk::PushString(ctx, attr->value());
    return 1;
}

static duk_ret_t ValueSetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Attr *attr = DukScriptObject::To<Attr>(ctx, -1);
    attr->setValue(Duk::To<AtomicString>(ctx, 0));
    return 0;
}

} // namespace Impl

void DukAttr::FillPrototypeEntry(PrototypeEntry &entry)
{
    static const PrototypeEntry::Property Properties[] = {
        { "name",         Impl::NameGetter,         nullptr           },
        { "ownerElement", Impl::OwnerElementGetter, nullptr           },
        { "prefix",       Impl::PrefixGetter,       nullptr           },
        { "specified",    Impl::SpecifiedGetter,    nullptr           },
        { "value",        Impl::ValueGetter,        Impl::ValueSetter },
    };

    DukNode::FillPrototypeEntry(entry);
    entry.Add(Properties, std::size(Properties));
}

void DukAttr::RegisterPrototype(PrototypeHelper &helper)
{
    helper.Register(ProtoName, FillPrototypeEntry);
}

} // namespace BlinKit
