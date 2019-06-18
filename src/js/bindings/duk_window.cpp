// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_window.cpp
// Description: Bindings for Window
//      Author: Ziming Li
//     Created: 2019-05-19
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_window.h"

#include "bindings/duk_binding_impl.h"
#include "bindings/duk_console.h"
#include "bindings/duk_document.h"
#include "bindings/duk_location.h"
#include "bindings/duk_xhr.h"
#include "context/duk_context.h"
#include "context/prototype_manager.h"
#include "wrappers/duk.h"

using namespace blink;

namespace BlinKit {

const char DukWindow::ProtoName[] = "Window";

namespace Crawler {

static duk_ret_t Confirm(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t GetComputedStyle(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t GetSelection(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t MatchMedia(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t Prompt(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

} // namespace Crawler

namespace Impl {

static duk_ret_t AToB(duk_context *ctx)
{
    duk_dup(ctx, 0);
    duk_base64_decode(ctx, -1);
    return 1;
}

static duk_ret_t BToA(duk_context *ctx)
{
    duk_dup(ctx, 0);
    duk_base64_encode(ctx, -1);
    return 1;
}

static duk_ret_t ClearInterval(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t ClearTimeout(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t ClosedGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t ConsoleGetter(duk_context *ctx)
{
    DukContext::From(ctx)->CreateObject<DukConsole>(ctx);
    return 1;
}

static duk_ret_t ConsoleSetter(duk_context *ctx)
{
    return Duk::DefaultSetter(ctx, "console");
}

static duk_ret_t DocumentGetter(duk_context *ctx)
{
    duk_push_global_object(ctx);
    DOMWindow *window = DukEventTarget::GetNativeThis<DOMWindow>(ctx);
    DukContext::From(ctx)->PushNode(ctx, window->document());
    return 1;
}

static duk_ret_t FramesGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t FramesSetter(duk_context *ctx)
{
    return Duk::DefaultSetter(ctx, "frames");
}

static duk_ret_t LocationGetter(duk_context *ctx)
{
    duk_push_global_object(ctx);
    DOMWindow *window = DukEventTarget::GetNativeThis<DOMWindow>(ctx);
    DukContext::From(ctx)->PushObject<DukLocation>(ctx, window->location());
    return 1;
}

static duk_ret_t LocationSetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t NavigatorGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t ParentSetter(duk_context *ctx)
{
    return Duk::DefaultSetter(ctx, "parent");
}

static duk_ret_t SetInterval(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t SetTimeout(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t WindowGetter(duk_context *ctx)
{
    duk_push_global_object(ctx);
    return 1;
}

} // namespace Impl

#ifndef BLINKIT_CRAWLER_ONLY
void DukWindow::RegisterPrototype(duk_context *ctx, PrototypeManager &protos)
{
    static const PrototypeEntry::Property Properties[] = {
        { "console", Impl::ConsoleGetter, Impl::ConsoleSetter }
    };
    const auto worker = [](PrototypeEntry & entry)
    {
        entry.Add(Properties, WTF_ARRAY_LENGTH(Properties));
    };
    protos.Register(ctx, ProtoName, worker);
}
#endif // BLINKIT_CRAWLER_ONLY

void DukWindow::RegisterPrototypeForCrawler(duk_context *ctx, PrototypeManager &protos)
{
    static const PrototypeEntry::Property Properties[] = {
        { "closed",    Impl::ClosedGetter,      nullptr                 },
        { "console",   Impl::ConsoleGetter,     Impl::ConsoleSetter     },
        { "document",  Impl::DocumentGetter,    nullptr                 },
        { "frames",    Impl::FramesGetter,      Impl::FramesSetter      },
        { "location",  Impl::LocationGetter,    Impl::LocationSetter    },
        { "navigator", Impl::NavigatorGetter,   nullptr                 },
        { "parent",    Impl::WindowGetter,      Impl::ParentSetter      },
        { "self",      Impl::WindowGetter,      nullptr                 },
        { "top",       Impl::WindowGetter,      nullptr                 },
        { "window",    Impl::WindowGetter,      nullptr                 },
    };
    static const PrototypeEntry::Method Methods[] = {
        { "alert",            Crawler::NothingToDo,      1           },
        { "atob",             Impl::AToB,                1           },
        { "btoa",             Impl::BToA,                1           },
        { "clearInterval",    Impl::ClearInterval,       1           },
        { "clearTimeout",     Impl::ClearTimeout,        1           },
        { "confirm",          Crawler::Confirm,          1           },
        { "getComputedStyle", Crawler::GetComputedStyle, 2           },
        { "getSelection",     Crawler::GetSelection,     0           },
        { "matchMedia",       Crawler::MatchMedia,       1           },
        { "moveBy",           Crawler::NothingToDo,      2           },
        { "moveTo",           Crawler::NothingToDo,      2           },
        { "print",            Crawler::NothingToDo,      0           },
        { "prompt",           Crawler::Prompt,           2           },
        { "resizeBy",         Crawler::NothingToDo,      2           },
        { "resizeTo",         Crawler::NothingToDo,      2           },
        { "setInterval",      Impl::SetInterval,         DUK_VARARGS },
        { "setTimeout",       Impl::SetTimeout,          DUK_VARARGS },
        { "stop",             Crawler::NothingToDo,      DUK_VARARGS },
        { DukXHR::ProtoName,  DukXHR::Constructor,       0           },
    };

    const auto worker = [](PrototypeEntry &entry)
    {
        DukEventTarget::RegisterToPrototypeEntry(entry);

        entry.Add(Properties, WTF_ARRAY_LENGTH(Properties));
        entry.AddObject("external");

        entry.Add(Methods, WTF_ARRAY_LENGTH(Methods));
    };
    protos.Register(ctx, ProtoName, worker);
}

} // namespace BlinKit
