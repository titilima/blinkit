// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_document.cpp
// Description: Bindings for Document
//      Author: Ziming Li
//     Created: 2019-06-01
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_document.h"

#include "bindings/duk_element.h"
#include "bindings/duk_exception_state.h"
#include "bindings/duk_node_list.h"
#include "context/duk_context.h"
#include "context/prototype_manager.h"
#include "wrappers/duk.h"

using namespace blink;

namespace BlinKit {

const char DukDocument::ProtoName[] = "HTMLDocument";

namespace Crawler {

static duk_ret_t Close(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t CookieGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t CookieSetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t DomainGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Document *document = DukEventTarget::GetNativeThis<Document>(ctx);
    return Duk::PushString(ctx, document->domain());
}

static duk_ret_t DomainSetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t LastModifiedGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Document *document = DukEventTarget::GetNativeThis<Document>(ctx);
    return Duk::PushString(ctx, document->lastModified());
}

static duk_ret_t Open(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t ReferrerGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Document *document = DukEventTarget::GetNativeThis<Document>(ctx);
    return Duk::PushString(ctx, document->referrer());
}

} // namespace Crawler

namespace Impl {

static duk_ret_t BodyGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t BodySetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t CreateAttribute(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t CreateComment(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t CreateDocumentFragment(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t CreateElement(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t CreateTextNode(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t DoctypeGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t DocumentElementGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t GetElementById(duk_context *ctx)
{
    duk_push_this(ctx);
    Document *document = DukEventTarget::GetNativeThis<Document>(ctx);

    Element *ret = document->getElementById(Duk::ToAtomicString(ctx, 0));
    DukContext::From(ctx)->PushNode(ret);
    return 1;
}

static duk_ret_t GetElementsByClassName(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t GetElementsByName(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t GetElementsByTagName(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t ImplementationGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t InputEncodingGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Document *document = DukEventTarget::GetNativeThis<Document>(ctx);
    return Duk::PushString(ctx, document->characterSet());
}

static duk_ret_t QuerySelector(duk_context *ctx)
{
    duk_push_this(ctx);
    Document *document = DukEventTarget::GetNativeThis<Document>(ctx);

    DukExceptionState es(ctx, "querySelector", "Document");
    PassRefPtr<Element> ret = document->querySelector(Duk::ToAtomicString(ctx, 0), es);
    if (es.hadException())
    {
        es.throwIfNeeded();
        return 0;
    }

    DukContext::From(ctx)->PushNode(ret.get());
    return 1;
}

static duk_ret_t QuerySelectorAll(duk_context *ctx)
{
    duk_push_this(ctx);
    Document *document = DukEventTarget::GetNativeThis<Document>(ctx);

    DukExceptionState es(ctx, "querySelectorAll", "Document");
    PassRefPtr<StaticElementList> ret = document->querySelectorAll(Duk::ToAtomicString(ctx, 0), es);
    if (es.hadException())
    {
        es.throwIfNeeded();
        return 0;
    }

    DukContext::From(ctx)->PushObject<DukNodeList>(ret.get());
    return 1;
}

static duk_ret_t ReadyStateGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Document *document = DukEventTarget::GetNativeThis<Document>(ctx);
    return Duk::PushString(ctx, document->readyState());
}

static duk_ret_t TitleGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Document *document = DukEventTarget::GetNativeThis<Document>(ctx);
    return Duk::PushString(ctx, document->title());
}

static duk_ret_t TitleSetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Document *document = DukEventTarget::GetNativeThis<Document>(ctx);
    document->setTitle(Duk::ToWTFString(ctx, 0));
    return 0;
}

static duk_ret_t URLGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Document *document = DukEventTarget::GetNativeThis<Document>(ctx);
    return Duk::PushString(ctx, document->url().string());
}

static duk_ret_t Write(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t Writeln(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

} // namespace Impl

void DukDocument::RegisterPrototypeForCrawler(duk_context *ctx, PrototypeManager &protos)
{
    static const PrototypeEntry::Property Properties[] = {
        { "body",            Impl::BodyGetter,            Impl::BodySetter },
        { "cookie",          Crawler::CookieGetter,       Crawler::CookieSetter },
        { "doctype",         Impl::DoctypeGetter,         nullptr },
        { "documentElement", Impl::DocumentElementGetter, nullptr },
        { "documentURI",     Impl::URLGetter,             nullptr },
        { "domain",          Crawler::DomainGetter,       Crawler::DomainSetter },
        { "implementation",  Impl::ImplementationGetter,  nullptr                 },
        { "inputEncoding",   Impl::InputEncodingGetter,   nullptr },
        { "lastModified",    Crawler::LastModifiedGetter, nullptr },
        { "readyState",      Impl::ReadyStateGetter,      nullptr },
        { "referrer",        Crawler::ReferrerGetter,     nullptr },
        { "title",           Impl::TitleGetter,           Impl::TitleSetter },
        { "URL",             Impl::URLGetter,             nullptr },
    };
    static const PrototypeEntry::Method Methods[] = {
        { "close",                  Crawler::Close,               0           },
        { "createAttribute",        Impl::CreateAttribute,        1           },
        { "createComment",          Impl::CreateComment,          1           },
        { "createDocumentFragment", Impl::CreateDocumentFragment, 0           },
        { "createElement",          Impl::CreateElement,          1           },
        { "createTextNode",         Impl::CreateTextNode,         1           },
        { "getElementById",         Impl::GetElementById,         1           },
        { "getElementsByClassName", Impl::GetElementsByClassName, 1           },
        { "getElementsByName",      Impl::GetElementsByName,      1           },
        { "getElementsByTagName",   Impl::GetElementsByTagName,   1           },
        { "open",                   Crawler::Open,                2           },
        { "querySelector",          Impl::QuerySelector,          1           },
        { "querySelectorAll",       Impl::QuerySelectorAll,       1           },
        { "write",                  Impl::Write,                  DUK_VARARGS },
        { "writeln",                Impl::Writeln,                DUK_VARARGS },
    };

    const auto worker = [](PrototypeEntry &entry)
    {
        DukNode::RegisterToPrototypeEntry(entry);

        entry.Add(Properties, WTF_ARRAY_LENGTH(Properties));
        entry.Add(Methods, WTF_ARRAY_LENGTH(Methods));
    };
    protos.Register(ctx, ProtoName, worker);
}

} // namespace BlinKit
