// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_navigator.cpp
// Description: DukNavigator Class
//      Author: Ziming Li
//     Created: 2020-03-25
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "duk_navigator.h"

#include "third_party/blink/renderer/bindings/core/duk/duk.h"

using namespace blink;

namespace BlinKit {

const char DukNavigator::ProtoName[] = "Navigator";

namespace Impl {

static duk_ret_t UserAgentGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Navigator *navigator = DukScriptObject::To<Navigator>(ctx, 0);
    Duk::PushString(ctx, navigator->userAgent());
    return 1;
}

} // namespace Impl

void DukNavigator::FillPrototypeEntry(PrototypeEntry &entry)
{
    static const PrototypeEntry::Property Properties[] = {
        { "userAgent",     Impl::UserAgentGetter,     nullptr },
    };

    DukScriptObject::FillPrototypeEntry(entry);
    entry.Add(Properties, std::size(Properties));
}

void DukNavigator::RegisterPrototype(PrototypeHelper &helper)
{
    helper.Register(ProtoName, FillPrototypeEntry);
}

} // namespace BlinKit
