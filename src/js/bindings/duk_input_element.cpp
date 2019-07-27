// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_input_element.cpp
// Description: Bindings for Input Element
//      Author: Ziming Li
//     Created: 2019-07-27
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_input_element.h"

#include "bindings/duk_binding_impl.h"
#include "bindings/duk_exception_state.h"
#include "context/prototype_manager.h"
#include "wrappers/duk.h"

using namespace blink;

namespace BlinKit {

const char DukInputElement::ProtoName[] = "HTMLInputElement";

namespace Crawler {

static duk_ret_t TypeGetter(duk_context *ctx)
{
    return ElementAttributeGetter(ctx, "type");
}

static duk_ret_t TypeSetter(duk_context *ctx)
{
    return ElementAttributeSetter(ctx, "type");
}

static duk_ret_t ValueGetter(duk_context *ctx)
{
    return ElementAttributeGetter(ctx, "value");
}

static duk_ret_t ValueSetter(duk_context *ctx)
{
    return ElementAttributeSetter(ctx, "value");
}

} // namespace Crawler

#ifndef BLINKIT_CRAWLER_ONLY
namespace UI {

static duk_ret_t Select(duk_context *ctx)
{
    duk_push_this(ctx);
    HTMLInputElement *element = DukEventTarget::GetNativeThis<HTMLInputElement>(ctx);

    element->select();
    return 0;
}

static duk_ret_t TypeGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    HTMLInputElement *element = DukEventTarget::GetNativeThis<HTMLInputElement>(ctx);

    return Duk::PushString(ctx, element->type());
}

static duk_ret_t TypeSetter(duk_context *ctx)
{
    duk_push_this(ctx);
    HTMLInputElement *element = DukEventTarget::GetNativeThis<HTMLInputElement>(ctx);

    element->setType(Duk::ToAtomicString(ctx, 0));
    return 0;
}

static duk_ret_t ValueGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    HTMLInputElement *element = DukEventTarget::GetNativeThis<HTMLInputElement>(ctx);

    return Duk::PushString(ctx, element->value());
}

static duk_ret_t ValueSetter(duk_context *ctx)
{
    duk_push_this(ctx);
    HTMLInputElement *element = DukEventTarget::GetNativeThis<HTMLInputElement>(ctx);

    DukExceptionState es(ctx, "value", DukInputElement::ProtoName);
    element->setValue(Duk::ToWTFString(ctx, 0), es);
    if (es.hadException())
        es.throwIfNeeded();
    return 0;
}

} // namespace UI
#endif // BLINKIT_CRAWLER_ONLY

void DukInputElement::RegisterPrototypeForCrawler(duk_context *ctx, PrototypeManager &protos)
{
    static const PrototypeEntry::Property Properties[] = {
        { "type",  Crawler::TypeGetter,  Crawler::TypeSetter  },
        { "value", Crawler::ValueGetter, Crawler::ValueSetter },
    };
    static const PrototypeEntry::Method Methods[] = {
        { "select", Crawler::NothingToDo, 0 },
    };

    const auto worker = [](PrototypeEntry &entry)
    {
        DukElement::RegisterToPrototypeEntryForCrawler(entry);
        entry.Add(Properties, WTF_ARRAY_LENGTH(Properties));
        entry.Add(Methods, WTF_ARRAY_LENGTH(Methods));
    };
    protos.Register(ctx, ProtoName, worker);
}

#ifndef BLINKIT_CRAWLER_ONLY
void DukInputElement::RegisterPrototypeForUI(duk_context *ctx, PrototypeManager &protos)
{
    static const PrototypeEntry::Property Properties[] = {
        { "type",  UI::TypeGetter,  UI::TypeSetter  },
        { "value", UI::ValueGetter, UI::ValueSetter },
    };
    static const PrototypeEntry::Method Methods[] = {
        { "select", UI::Select, 0 },
    };

    const auto worker = [](PrototypeEntry &entry)
    {
        DukElement::RegisterToPrototypeEntryForUI(entry);
        entry.Add(Properties, WTF_ARRAY_LENGTH(Properties));
        entry.Add(Methods, WTF_ARRAY_LENGTH(Methods));
    };
    protos.Register(ctx, ProtoName, worker);
}
#endif // BLINKIT_CRAWLER_ONLY

} // namespace BlinKit
