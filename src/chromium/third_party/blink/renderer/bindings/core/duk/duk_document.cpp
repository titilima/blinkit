// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_document.cpp
// Description: DukDocument Class
//      Author: Ziming Li
//     Created: 2020-02-07
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "duk_document.h"

#include "third_party/blink/renderer/bindings/core/duk/duk_exception_state.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"

using namespace blink;

namespace BlinKit {

const char DukDocument::ProtoName[] = "HTMLDocument";

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Impl {

static duk_ret_t Write(duk_context *ctx)
{
    std::vector<std::string> text;
    const duk_idx_t n = duk_get_top(ctx);
    for (duk_idx_t i = 0; i < n; ++i)
    {
        size_t l = 0;
        const char *s = duk_safe_to_lstring(ctx, i, &l);
        if (nullptr == s)
            continue;

        text.push_back(std::string(s, l));
    }

    duk_push_this(ctx);
    Document *doc = DukScriptObject::To<Document>(ctx, -1);

    duk_push_global_object(ctx);
    LocalDOMWindow *callingWindow = DukScriptObject::To<LocalDOMWindow>(ctx, -1);

    DukExceptionState es(ctx);
    doc->write(callingWindow, text, es);
    if (es.HadException())
        es.ThrowIfNeeded();
    return 0;
}

} // namespace Impl

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DukDocument::FillPrototypeEntryForCrawler(PrototypeEntry &entry)
{
    static const PrototypeEntry::Method Methods[] = {
        { "write",                  Impl::Write,                  DUK_VARARGS },
    };

    DukContainerNode::FillPrototypeEntry(entry);
    entry.Add(Methods, std::size(Methods));
}

void DukDocument::RegisterPrototypeForCrawler(PrototypeHelper &helper)
{
    helper.Register(ProtoName, FillPrototypeEntryForCrawler);
}

} // namespace BlinKit
