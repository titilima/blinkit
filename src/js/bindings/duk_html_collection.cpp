// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_html_collection.cpp
// Description: Bindings for HTMLCollection
//      Author: Ziming Li
//     Created: 2019-07-20
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_html_collection.h"

#include "bindings/duk_element.h"
#include "context/duk_context.h"
#include "context/prototype_manager.h"
#include "wrappers/duk.h"

using namespace blink;

namespace BlinKit {

const char DukHTMLCollection::ProtoName[] = "HTMLCollection";

static HTMLCollection* Get(duk_context *ctx, duk_idx_t idx = -1)
{
    return static_cast<HTMLCollection *>(Duk::GetNativeThis(ctx, idx));
}

duk_ret_t DukHTMLCollection::Finalizer(duk_context *ctx)
{
    HTMLCollection *nativeThis = Get(ctx, 0);
    DukContext::From(ctx)->RemoveObjectFromPool(nativeThis);
    return 0;
}

void DukHTMLCollection::OnCreate(duk_context *ctx, ScriptWrappable *nativeThis)
{
    DukObjectImpl::OnCreate(ctx, nativeThis);

    HTMLCollection *collection = static_cast<HTMLCollection *>(nativeThis);

    DukContext *context = DukContext::From(ctx);

    unsigned length = collection->length();
    for (unsigned i = 0; i < length; ++i)
    {
        context->PushNode(ctx, collection->item(i));
        duk_put_prop_index(ctx, -2, i);
    }

    Vector<String> names;
    collection->namedPropertyEnumerator(names, ASSERT_NO_EXCEPTION);
    for (const String &name : names)
    {
        const std::string utf8 = name.to_string();
        context->PushNode(ctx, collection->namedItem(AtomicString::fromUTF8(utf8.data(), utf8.length())));
        duk_put_prop_lstring(ctx, -2, utf8.data(), utf8.length());
    }
}

namespace Impl {

static duk_ret_t Item(duk_context *ctx)
{
    duk_push_this(ctx);
    HTMLCollection *collection = Get(ctx);

    Node *node = collection->item(duk_to_uint(ctx, 0));
    DukContext::From(ctx)->PushNode(ctx, node);
    return 1;
}

static duk_ret_t LengthGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    HTMLCollection *collection = Get(ctx);

    duk_push_uint(ctx, collection->length());
    return 1;
}

static duk_ret_t NamedItem(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

} // namespace Impl

void DukHTMLCollection::RegisterPrototype(duk_context *ctx, PrototypeManager &protos)
{
    static const PrototypeEntry::Property Properties[] = {
        { "length", Impl::LengthGetter, nullptr }
    };
    static const PrototypeEntry::Method Methods[] = {
        { "item",      Impl::Item,      1 },
        { "namedItem", Impl::NamedItem, 1 },
    };

    const auto worker = [](PrototypeEntry &entry)
    {
        entry.Add(Properties, WTF_ARRAY_LENGTH(Properties));
        entry.Add(Methods, WTF_ARRAY_LENGTH(Methods));
    };
    protos.Register(ctx, ProtoName, worker);
}

} // namespace BlinKit
