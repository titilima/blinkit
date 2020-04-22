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

#include "third_party/blink/renderer/bindings/core/duk/duk.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_element.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_exception_state.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_location.h"
#include "third_party/blink/renderer/core/dom/comment.h"
#include "third_party/blink/renderer/core/dom/document_fragment.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"

using namespace blink;

namespace BlinKit {

const char DukDocument::ProtoName[] = "HTMLDocument";

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void CollectStringArgs(duk_context *ctx, std::vector<std::string> &dst)
{
    const duk_idx_t n = duk_get_top(ctx);
    for (duk_idx_t i = 0; i < n; ++i)
    {
        size_t l = 0;
        const char *s = duk_safe_to_lstring(ctx, i, &l);
        if (nullptr == s)
            continue;

        dst.push_back(std::string(s, l));
    }
}

namespace Impl {

static duk_ret_t BodyGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Document *document = DukScriptObject::To<Document>(ctx, -1);
    DukElement::Push(ctx, document->body());
    return 1;
}

static duk_ret_t CreateComment(duk_context *ctx)
{
    const String data = Duk::To<String>(ctx, 0);

    duk_push_this(ctx);
    Document *document = DukScriptObject::To<Document>(ctx, -1);

    Comment *ret = document->createComment(data);
    DukNode::Push(ctx, ret);
    return 1;
}

static duk_ret_t CreateDocumentFragment(duk_context *ctx)
{
    duk_push_this(ctx);
    Document *document = DukScriptObject::To<Document>(ctx, -1);

    DocumentFragment *ret = document->createDocumentFragment();
    DukNode::Push(ctx, ret);
    return 1;
}

static duk_ret_t CreateElement(duk_context *ctx)
{
    const AtomicString name = Duk::To<AtomicString>(ctx, 0);

    duk_push_this(ctx);
    Document *document = DukScriptObject::To<Document>(ctx, -1);

    DukExceptionState exceptionState(ctx);
    Element *ret = document->createElement(name, exceptionState);
    if (exceptionState.HadException())
    {
        exceptionState.ThrowIfNeeded();
        return 0;
    }

    DukElement::Push(ctx, ret);
    return 1;
}

static duk_ret_t DocumentElementGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Document *document = DukScriptObject::To<Document>(ctx, -1);

    DukElement::Push(ctx, document->documentElement());
    return 1;
}

static duk_ret_t GetElementById(duk_context *ctx)
{
    const AtomicString id = Duk::To<AtomicString>(ctx, 0);

    duk_push_this(ctx);
    Document *document = DukScriptObject::To<Document>(ctx, -1);

    Element *ret = document->getElementById(id);
    DukElement::Push(ctx, ret);
    return 1;
}

static duk_ret_t LocationGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Document *document = DukScriptObject::To<Document>(ctx, -1);
    DukScriptObject::Push<DukLocation>(ctx, document->location());
    return 1;
}

static duk_ret_t LocationSetter(duk_context *ctx)
{
    ASSERT(false); // BKTODO:
    return 0;
}

static duk_ret_t URLGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Document *document = DukScriptObject::To<Document>(ctx, -1);
    BkURL ret = document->urlForBinding();
    Duk::PushString(ctx, ret.AsString());
    return 1;
}

static duk_ret_t Write(duk_context *ctx)
{
    std::vector<std::string> text;
    CollectStringArgs(ctx, text);

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

static duk_ret_t Writeln(duk_context *ctx)
{
    std::vector<std::string> text;
    CollectStringArgs(ctx, text);

    duk_push_this(ctx);
    Document *doc = DukScriptObject::To<Document>(ctx, -1);

    duk_push_global_object(ctx);
    LocalDOMWindow *callingWindow = DukScriptObject::To<LocalDOMWindow>(ctx, -1);

    DukExceptionState es(ctx);
    doc->writeln(callingWindow, text, es);
    if (es.HadException())
        es.ThrowIfNeeded();
    return 0;
}

} // namespace Impl

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DukDocument::FillPrototypeEntryForCrawler(PrototypeEntry &entry)
{
    static const PrototypeEntry::Method Methods[] = {
        { "createComment",          Impl::CreateComment,          1           },
        { "createDocumentFragment", Impl::CreateDocumentFragment, 0           },
        { "createElement",          Impl::CreateElement,          1           },
        { "getElementById",         Impl::GetElementById,         1           },
        { "write",                  Impl::Write,                  DUK_VARARGS },
        { "writeln",                Impl::Writeln,                DUK_VARARGS },
    };
    static const PrototypeEntry::Property Properties[] = {
        { "body",            Impl::BodyGetter,            nullptr               },
        { "documentElement", Impl::DocumentElementGetter, nullptr               },
        { "location",        Impl::LocationGetter,        Impl::LocationSetter  },
        { "URL",             Impl::URLGetter,            nullptr               },
    };

    DukContainerNode::FillPrototypeEntry(entry);
    entry.Add(Methods, std::size(Methods));
    entry.Add(Properties, std::size(Properties));
}

void DukDocument::RegisterPrototypeForCrawler(PrototypeHelper &helper)
{
    helper.Register(ProtoName, FillPrototypeEntryForCrawler);
}

} // namespace BlinKit
