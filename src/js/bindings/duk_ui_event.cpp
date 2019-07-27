// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_ui_event.cpp
// Description: Bindings for UIEvent
//      Author: Ziming Li
//     Created: 2019-07-27
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_ui_event.h"

#include "context/prototype_manager.h"

using namespace blink;

namespace BlinKit {

namespace Impl {

static duk_ret_t DetailGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    UIEvent *e = DukEvent::GetNativeThis<UIEvent>(ctx);

    duk_push_int(ctx, e->detail());
    return 1;
}

static duk_ret_t ViewGetter(duk_context *ctx)
{
    duk_push_global_object(ctx);
    return 1;
}

} // namespace Impl

void DukUIEvent::RegisterToPrototypeEntry(PrototypeEntry &entry)
{
    static const PrototypeEntry::Property Properties[] = {
        { "detail", Impl::DetailGetter, nullptr },
        { "view",   Impl::ViewGetter,   nullptr },
    };

    DukEvent::RegisterToPrototypeEntry(entry);
    entry.Add(Properties, WTF_ARRAY_LENGTH(Properties));
}

} // namespace BlinKit
