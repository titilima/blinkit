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
#include "third_party/blink/renderer/bindings/core/duk/duk_timer.h"

using namespace blink;

namespace BlinKit {

const char DukWindow::ProtoName[] = "Window";

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * NOTE:
 * As the Window object is the global object, `duk_push_this` will push `undefined` in strict mode.
 * So in all members of Window, `duk_push_global_object` will be used instead of `duk_push_this`.
 */

namespace Crawler {

static duk_ret_t GetComputedStyle(duk_context *ctx)
{
    duk_push_object(ctx);
    return 1;
}

} // namespace Crawler

namespace Impl {

static duk_ret_t DocumentGetter(duk_context *ctx)
{
    duk_push_global_object(ctx);
    LocalDOMWindow *w = DukScriptObject::To<LocalDOMWindow>(ctx, -1);
    DukScriptObject::Push<DukDocument>(ctx, w->document());
    return 1;
}

static duk_ret_t LocationGetter(duk_context *ctx)
{
    duk_push_global_object(ctx);
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
    duk_push_global_object(ctx);
    LocalDOMWindow *w = DukScriptObject::To<LocalDOMWindow>(ctx, -1);
    DukScriptObject::Push<DukNavigator>(ctx, w->navigator());
    return 1;
}

static duk_ret_t SetInterval(duk_context *ctx)
{
    return DukWindow::SetTimerImpl(ctx, true);
}

static duk_ret_t SetTimeout(duk_context *ctx)
{
    return DukWindow::SetTimerImpl(ctx, false);
}

static duk_ret_t WindowGetter(duk_context *ctx)
{
    duk_push_global_object(ctx);
    return 1;
}

} // namespace Impl

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DukWindow::Attach(duk_context *ctx, LocalDOMWindow &window)
{
    duk_push_global_object(ctx);
    DukScriptObject::BindNative(ctx, -1, window);
    PrototypeHelper::AttachToScriptObject(ctx, -1, ProtoName);
    duk_pop(ctx);
}

void DukWindow::FillPrototypeEntryForCrawler(PrototypeEntry &entry)
{
    static const PrototypeEntry::Method Methods[] = {
        { "getComputedStyle", Crawler::GetComputedStyle, 2           },
        { "setInterval",      Impl::SetInterval,         DUK_VARARGS },
        { "setTimeout",       Impl::SetTimeout,          DUK_VARARGS },
    };
    static const PrototypeEntry::Property Properties[] = {
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

duk_ret_t DukWindow::SetTimerImpl(duk_context *ctx, bool repeatable)
{
    int argc = duk_get_top(ctx);
    if (0 == argc)
        return duk_type_error(ctx, "Not enough arguments");

    std::unique_ptr<DukTimer> timer = std::make_unique<DukTimer>(ctx, 0, argc > 2 ? argc - 2 : 0);
    if (repeatable)
        timer->SetIsRepeatable();
    if (argc > 1)
        timer->SetInterval(duk_to_uint(ctx, 1));

    duk_push_global_object(ctx);
    LocalDOMWindow *w = DukScriptObject::To<LocalDOMWindow>(ctx, -1);
    duk_push_uint(ctx, w->AddTimer(timer));
    return 1;
}

} // namespace BlinKit
