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

#include "core/HTMLNames.h"
#include "core/dom/Attr.h"
#include "core/dom/Comment.h"
#include "core/dom/DocumentFragment.h"
#include "core/dom/ElementTraversal.h"
#include "core/dom/Text.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "core/html/HTMLHeadElement.h"
#endif

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

static duk_ret_t BodyGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Document *document = DukEventTarget::GetNativeThis<Document>(ctx);

    Element *ret = nullptr;

    Element *de = document->documentElement();
    if (nullptr != de)
        ret = Traversal<Element>::firstChild(*de, HasTagName(HTMLNames::bodyTag));

    DukContext::From(ctx)->PushNode(ctx, ret);
    return 1;
}

static duk_ret_t BodySetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

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

static duk_ret_t ExecCommand(duk_context *ctx)
{
    if (duk_get_top(ctx) < 1)
        return duk_error(ctx, DUK_ERR_TYPE_ERROR, "Not enough arguments");

    const std::string command = Duk::ToString(ctx, 0);

    if (DukContext::From(ctx)->CheckIncantationAndPushCrawler(ctx, command))
        return 1;

    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t HeadGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Document *document = DukEventTarget::GetNativeThis<Document>(ctx);

    Element *ret = nullptr;

    Element *de = document->documentElement();
    if (nullptr != de)
        ret = Traversal<Element>::firstChild(*de, HasTagName(HTMLNames::headTag));

    DukContext::From(ctx)->PushNode(ctx, ret);
    return 1;
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

static duk_ret_t CreateAttribute(duk_context *ctx)
{
    duk_push_this(ctx);
    Document *document = DukEventTarget::GetNativeThis<Document>(ctx);

    DukExceptionState es(ctx, "createAttribute", "Document");
    PassRefPtr<Attr> ret = document->createAttribute(Duk::ToAtomicString(ctx, 0), es);
    if (es.hadException())
    {
        es.throwIfNeeded();
        return 0;
    }

    DukContext::From(ctx)->PushNode(ctx, ret.get());
    return 1;
}

static duk_ret_t CreateComment(duk_context *ctx)
{
    duk_push_this(ctx);
    Document *document = DukEventTarget::GetNativeThis<Document>(ctx);

    PassRefPtr<Comment> ret = document->createComment(Duk::ToWTFString(ctx, 0));
    DukContext::From(ctx)->PushNode(ctx, ret.get());
    return 1;
}

static duk_ret_t CreateDocumentFragment(duk_context *ctx)
{
    duk_push_this(ctx);
    Document *document = DukEventTarget::GetNativeThis<Document>(ctx);

    PassRefPtr<DocumentFragment> ret = document->createDocumentFragment();
    DukContext::From(ctx)->PushNode(ctx, ret.get());
    return 1;
}

static duk_ret_t CreateElement(duk_context *ctx)
{
    duk_push_this(ctx);
    Document *document = DukEventTarget::GetNativeThis<Document>(ctx);

    DukExceptionState es(ctx, "createElement", "Document");
    PassRefPtr<Element> ret = document->createElement(Duk::ToAtomicString(ctx, 0), es);
    if (es.hadException())
    {
        es.throwIfNeeded();
        return 0;
    }

    DukContext::From(ctx)->PushNode(ctx, ret.get());
    return 1;
}

static duk_ret_t CreateTextNode(duk_context *ctx)
{
    duk_push_this(ctx);
    Document *document = DukEventTarget::GetNativeThis<Document>(ctx);

    PassRefPtr<Text> ret = document->createTextNode(Duk::ToWTFString(ctx, 0));
    DukContext::From(ctx)->PushNode(ctx, ret.get());
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
    DukContext::From(ctx)->PushNode(ctx, ret);
    return 1;
}

static duk_ret_t GetElementsByName(duk_context *ctx)
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

#ifndef BLINKIT_CRAWLER_ONLY
namespace UI {

static duk_ret_t BodyGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    HTMLDocument *document = DukEventTarget::GetNativeThis<HTMLDocument>(ctx);

    DukContext::From(ctx)->PushNode(ctx, document->body());
    return 1;
}

static duk_ret_t Close(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t HeadGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    HTMLDocument *document = DukEventTarget::GetNativeThis<HTMLDocument>(ctx);

    DukContext::From(ctx)->PushNode(ctx, document->head());
    return 1;
}

} // namespace UI
#endif // BLINKIT_CRAWLER_ONLY

void DukDocument::RegisterPrototypeForCrawler(duk_context *ctx, PrototypeManager &protos)
{
    static const PrototypeEntry::Property Properties[] = {
        { "body",            Crawler::BodyGetter,         Crawler::BodySetter   },
        { "cookie",          Crawler::CookieGetter,       Crawler::CookieSetter },
        { "doctype",         Impl::DoctypeGetter,         nullptr               },
        { "documentElement", Impl::DocumentElementGetter, nullptr               },
        { "documentURI",     Impl::URLGetter,             nullptr               },
        { "domain",          Crawler::DomainGetter,       Crawler::DomainSetter },
        { "head",            Crawler::HeadGetter,         nullptr               },
        { "implementation",  Impl::ImplementationGetter,  nullptr               },
        { "inputEncoding",   Impl::InputEncodingGetter,   nullptr               },
        { "lastModified",    Crawler::LastModifiedGetter, nullptr               },
        { "readyState",      Impl::ReadyStateGetter,      nullptr               },
        { "referrer",        Crawler::ReferrerGetter,     nullptr               },
        { "title",           Impl::TitleGetter,           Impl::TitleSetter     },
        { "URL",             Impl::URLGetter,             nullptr               },
    };
    static const PrototypeEntry::Method Methods[] = {
        { "close",                  Crawler::Close,               0           },
        { "createAttribute",        Impl::CreateAttribute,        1           },
        { "createComment",          Impl::CreateComment,          1           },
        { "createDocumentFragment", Impl::CreateDocumentFragment, 0           },
        { "createElement",          Impl::CreateElement,          1           },
        { "createTextNode",         Impl::CreateTextNode,         1           },
        { "execCommand",            Crawler::ExecCommand,         DUK_VARARGS },
        { "getElementById",         Impl::GetElementById,         1           },
        { "getElementsByName",      Impl::GetElementsByName,      1           },
        { "open",                   Crawler::Open,                2           },
        { "write",                  Impl::Write,                  DUK_VARARGS },
        { "writeln",                Impl::Writeln,                DUK_VARARGS },
    };

    const auto worker = [](PrototypeEntry &entry)
    {
        DukContainerNode::RegisterToPrototypeEntry(entry);

        entry.Add(Properties, WTF_ARRAY_LENGTH(Properties));
        entry.Add(Methods, WTF_ARRAY_LENGTH(Methods));
    };
    protos.Register(ctx, ProtoName, worker);
}

#ifndef BLINKIT_CRAWLER_ONLY
void DukDocument::RegisterPrototypeForUI(duk_context *ctx, PrototypeManager &protos)
{
    static const PrototypeEntry::Property Properties[] = {
        { "body",            UI::BodyGetter,              nullptr },
        { "doctype",         Impl::DoctypeGetter,         nullptr },
        { "documentElement", Impl::DocumentElementGetter, nullptr },
        { "documentURI",     Impl::URLGetter,             nullptr },
        { "head",            UI::HeadGetter,              nullptr },
        { "implementation",  Impl::ImplementationGetter,  nullptr },
        { "inputEncoding",   Impl::InputEncodingGetter,   nullptr },
        { "readyState",      Impl::ReadyStateGetter,      nullptr },
        { "title",           Impl::TitleGetter,           nullptr },
        { "URL",             Impl::URLGetter,             nullptr },
    };
    static const PrototypeEntry::Method Methods[] = {
        { "close",                  UI::Close,                    0           },
        { "createAttribute",        Impl::CreateAttribute,        1           },
        { "createComment",          Impl::CreateComment,          1           },
        { "createDocumentFragment", Impl::CreateDocumentFragment, 0           },
        { "createElement",          Impl::CreateElement,          1           },
        { "createTextNode",         Impl::CreateTextNode,         1           },
        { "getElementById",         Impl::GetElementById,         1           },
        { "getElementsByName",      Impl::GetElementsByName,      1           },
        { "write",                  Impl::Write,                  DUK_VARARGS },
        { "writeln",                Impl::Writeln,                DUK_VARARGS },
    };

    const auto worker = [](PrototypeEntry &entry)
    {
        DukContainerNode::RegisterToPrototypeEntry(entry);

        entry.Add(Properties, WTF_ARRAY_LENGTH(Properties));
        entry.Add(Methods, WTF_ARRAY_LENGTH(Methods));
    };
    protos.Register(ctx, ProtoName, worker);
}
#endif // BLINKIT_CRAWLER_ONLY

} // namespace BlinKit
