// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: browser_context.cpp
// Description: BrowserContext Class
//      Author: Ziming Li
//     Created: 2020-08-09
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "browser_context.h"

#include "third_party/blink/renderer/bindings/core/duk/duk.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_element.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_global.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_window.h"
#include "third_party/blink/renderer/bindings/core/duk/script_controller.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/platform/bindings/gc_pool.h"

using namespace blink;

namespace BlinKit {

static const char Globals[] = "globals";
static const char NativeContext[] = "nativeContext";

static void DefaultConsoleOutput(int type, const char *msg)
{
    BkLog("%s", msg);
}

BrowserContext::BrowserContext(const LocalFrame &frame, const PrototypeMap &prototypeMap)
    : m_frame(frame)
    , m_prototypeMap(prototypeMap)
    , m_consoleMessager(std::bind(DefaultConsoleOutput, std::placeholders::_1, std::placeholders::_2))
{
}

BrowserContext::~BrowserContext(void) = default;

void BrowserContext::Clear(void)
{
    duk_context *ctx = GetRawContext();

    GCPool gcPool(ctx);
    NewGlobalObject();
    duk_gc(ctx, 0);
}

void BrowserContext::ExposeGlobals(duk_context *ctx, duk_idx_t dst)
{
    Duk::StackGuard sg(ctx);

    duk_push_heap_stash(ctx);
    duk_get_prop_string(ctx, -1, Globals);

    static const duk_uint_t flags =
        DUK_ENUM_OWN_PROPERTIES_ONLY
        | DUK_ENUM_INCLUDE_NONENUMERABLE
        | DUK_ENUM_INCLUDE_SYMBOLS;
    duk_enum(ctx, -1, flags);
    while (duk_next(ctx, -1, true))
        duk_put_prop(ctx, dst);
}

BrowserContext* BrowserContext::From(duk_context *ctx)
{
    BrowserContext *ret = nullptr;

    duk_push_heap_stash(ctx);
    duk_get_prop_string(ctx, -1, NativeContext);
    ret = reinterpret_cast<BrowserContext*>(duk_get_pointer(ctx, -1));
    duk_pop_2(ctx);

    return ret;
}

BrowserContext* BrowserContext::From(ExecutionContext *executionContext)
{
    if (executionContext->IsDocument())
    {
        Document *document = static_cast<Document *>(executionContext);
        return document->GetFrame()->GetScriptController().GetContext();
    }

    NOTREACHED();
    return nullptr;
}

void BrowserContext::InitializeHeapStash(DukWorker worker)
{
    duk_context *ctx = GetRawContext();

    Duk::StackGuard sg(ctx);
    duk_push_heap_stash(ctx);

    duk_push_pointer(ctx, this);
    duk_put_prop_string(ctx, -2, NativeContext);

    duk_push_global_object(ctx);
    DukGlobal::Attach(ctx, -1);
    duk_put_prop_string(ctx, -2, Globals);

    worker(ctx);
}

const char* BrowserContext::LookupPrototypeName(const std::string &tagName) const
{
    auto it = m_prototypeMap.find(tagName);
    if (std::end(m_prototypeMap) == it)
        return DukElement::ProtoName;
    return it->second;
}

void BrowserContext::NewGlobalObject(void)
{
    duk_context *ctx = GetRawContext();

    duk_idx_t idx = duk_push_object(ctx);
    ExposeGlobals(ctx, idx);
    duk_set_global_object(ctx);
}

void BrowserContext::UpdateDocument(void)
{
    duk_context *ctx = GetRawContext();
    DukWindow::Attach(ctx, *(m_frame.DomWindow()));
}

} // namespace BlinKit
