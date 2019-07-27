// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_attr.cpp
// Description: Bindings for Attr
//      Author: Ziming Li
//     Created: 2019-07-22
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_attr.h"

#include "context/prototype_manager.h"

using namespace blink;

namespace BlinKit {

const char DukAttr::ProtoName[] = "Attr";

namespace Impl {

static duk_ret_t NameGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t OwnerElementGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t PrefixGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t SpecifiedGetter(duk_context *ctx)
{
    duk_push_boolean(ctx, true);
    return 1;
}

static duk_ret_t ValueGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t ValueSetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

} // namespace Impl

void DukAttr::RegisterPrototype(duk_context *ctx, PrototypeManager &protos)
{
    static const PrototypeEntry::Property Properties[] = {
        { "name",         Impl::NameGetter,         nullptr           },
        { "ownerElement", Impl::OwnerElementGetter, nullptr           },
        { "prefix",       Impl::PrefixGetter,       nullptr           },
        { "specified",    Impl::SpecifiedGetter,    nullptr           },
        { "value",        Impl::ValueGetter,        Impl::ValueSetter },
    };

    const auto worker = [](PrototypeEntry &entry)
    {
        DukNode::RegisterToPrototypeEntry(entry);
        entry.Add(Properties, WTF_ARRAY_LENGTH(Properties));
    };
    protos.Register(ctx, ProtoName, worker);
}

} // namespace BlinKit
