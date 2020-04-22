// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_window.cpp
// Description: DukWindow Class
//      Author: Ziming Li
//     Created: 2020-01-30
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "duk_window.h"

#include "third_party/blink/renderer/bindings/core/duk/duk_console.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_document.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_location.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_navigator.h"

using namespace blink;

namespace BlinKit {

const char DukWindow::ProtoName[] = "Window";

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Crawler {

static duk_ret_t GetComputedStyle(duk_context *ctx)
{
    duk_push_object(ctx);
    return 1;
}

} // namespace Crawler

namespace Impl {

static duk_ret_t AToB(duk_context *ctx)
{
    duk_dup(ctx, 0);
    duk_base64_decode(ctx, -1);
    duk_buffer_to_string(ctx, -1);
    return 1;
}

static duk_ret_t BToA(duk_context *ctx)
{
    duk_dup(ctx, 0);
    duk_base64_encode(ctx, -1);
    duk_buffer_to_string(ctx, -1);
    return 1;
}

static duk_ret_t ConsoleGetter(duk_context *ctx)
{
    DukScriptObject::Create<DukConsole>(ctx);
    return 1;
}

static duk_ret_t DocumentGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    LocalDOMWindow *w = DukScriptObject::To<LocalDOMWindow>(ctx, -1);
    DukScriptObject::Push<DukDocument>(ctx, w->document());
    return 1;
}

static duk_ret_t LocationGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    LocalDOMWindow *w = DukScriptObject::To<LocalDOMWindow>(ctx, -1);
    DukScriptObject::Push<DukLocation>(ctx, w->location());
    return 1;
}

static duk_ret_t LocationSetter(duk_context *ctx)
{
    ASSERT(false); // BKTODO:
    return 0;
}

static duk_ret_t NavigatorGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    LocalDOMWindow *w = DukScriptObject::To<LocalDOMWindow>(ctx, -1);
    DukScriptObject::Push<DukNavigator>(ctx, w->navigator());
    return 1;
}

static duk_ret_t WindowGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    return 1;
}

} // namespace Impl

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DukWindow::FillPrototypeEntryForCrawler(PrototypeEntry &entry)
{
    static const PrototypeEntry::Method Methods[] = {
        { "atob",             Impl::AToB,                1           },
        { "btoa",             Impl::BToA,                1           },
        { "getComputedStyle", Crawler::GetComputedStyle, 2           },
    };
    static const PrototypeEntry::Property Properties[] = {
        { "console",   Impl::ConsoleGetter,   nullptr              },
        { "document",  Impl::DocumentGetter,  nullptr              },
        { "location",  Impl::LocationGetter,  Impl::LocationSetter },
        { "navigator", Impl::NavigatorGetter, nullptr              },
        { "self",      Impl::WindowGetter,    nullptr              },
        { "top",       Impl::WindowGetter,    nullptr              },
        { "window",    Impl::WindowGetter,    nullptr              },
    };

    DukEventTarget::FillPrototypeEntry(entry);
    entry.Add(Methods, std::size(Methods));
    entry.Add(Properties, std::size(Properties));
}

void DukWindow::RegisterPrototypeForCrawler(PrototypeHelper &helper)
{
    helper.Register(ProtoName, FillPrototypeEntryForCrawler);
}

} // namespace BlinKit
