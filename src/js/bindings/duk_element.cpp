// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_element.cpp
// Description: Bindings for Element
//      Author: Ziming Li
//     Created: 2019-06-02
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_element.h"

#include "core/dom/NonDocumentTypeChildNode.h"
#include "core/dom/ParentNode.h"
#include "core/dom/StaticNodeList.h"

#include "bindings/duk_binding_impl.h"
#include "bindings/duk_exception_state.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "bindings/duk_css_style_declaration.h"
#endif
#include "bindings/duk_named_node_map.h"
#include "context/duk_context.h"
#include "context/prototype_manager.h"
#include "wrappers/duk.h"

using namespace blink;

namespace BlinKit {

const char DukElement::ProtoName[] = "HTMLElement";

duk_ret_t DukElement::DefaultAttributeGetter(duk_context *ctx, const QualifiedName &name)
{
    duk_push_this(ctx);
    Element *element = DukEventTarget::GetNativeThis<Element>(ctx);
    return Duk::PushString(ctx, element->getAttribute(name));
}

duk_ret_t DukElement::DefaultAttributeSetter(duk_context *ctx, const QualifiedName &name, duk_idx_t valueIdx)
{
    duk_push_this(ctx);
    Element *element = DukEventTarget::GetNativeThis<Element>(ctx);
    element->setAttribute(name, Duk::ToAtomicString(ctx, valueIdx));
    return 0;
}

namespace Impl {

static duk_ret_t AttributesGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukEventTarget::GetNativeThis<Element>(ctx);

    DukContext::From(ctx)->PushObject<DukNamedNodeMap>(ctx, element->attributesForBindings());
    return 1;
}

static duk_ret_t ChildrenGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t ClassListGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t ClassNameGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukEventTarget::GetNativeThis<Element>(ctx);
    return Duk::PushString(ctx, element->getClassAttribute());
}

static duk_ret_t ClassNameSetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukEventTarget::GetNativeThis<Element>(ctx);
    element->setAttribute(HTMLNames::classAttr, Duk::ToAtomicString(ctx, 0));
    return 0;
}

static duk_ret_t FirstElementChildGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukEventTarget::GetNativeThis<Element>(ctx);

    Element *ret = ParentNode::firstElementChild(*element);
    DukContext::From(ctx)->PushNode(ctx, ret);
    return 1;
}

static duk_ret_t GetAttribute(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukEventTarget::GetNativeThis<Element>(ctx);
    return Duk::PushString(ctx, element->getAttribute(Duk::ToAtomicString(ctx, 0)));
}

static duk_ret_t GetAttributeNode(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t HasAttribute(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukEventTarget::GetNativeThis<Element>(ctx);
    duk_push_boolean(ctx, element->hasAttribute(Duk::ToAtomicString(ctx, 0)));
    return 1;
}

static duk_ret_t HasAttributes(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukEventTarget::GetNativeThis<Element>(ctx);
    duk_push_boolean(ctx, element->hasAttributes());
    return 1;
}

static duk_ret_t IdGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukEventTarget::GetNativeThis<Element>(ctx);
    return Duk::PushString(ctx, element->getIdAttribute());
}

static duk_ret_t IdSetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukEventTarget::GetNativeThis<Element>(ctx);
    element->setIdAttribute(Duk::ToAtomicString(ctx, 0));
    return 0;
}

static duk_ret_t InnerHTMLGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukEventTarget::GetNativeThis<Element>(ctx);
    return Duk::PushString(ctx, element->innerHTML());
}

static duk_ret_t InnerHTMLSetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukEventTarget::GetNativeThis<Element>(ctx);

    DukExceptionState es(ctx, "innerHTML", DukElement::ProtoName);
    element->setInnerHTML(Duk::ToWTFString(ctx, 0), es);
    if (es.hadException())
        es.throwIfNeeded();
    return 0;
}

static duk_ret_t LastElementChildGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukEventTarget::GetNativeThis<Element>(ctx);

    Element *ret = ParentNode::lastElementChild(*element);
    DukContext::From(ctx)->PushNode(ctx, ret);
    return 1;
}

static duk_ret_t NextElementSiblingGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukEventTarget::GetNativeThis<Element>(ctx);

    Element *ret = NonDocumentTypeChildNode::nextElementSibling(*element);
    DukContext::From(ctx)->PushNode(ctx, ret);
    return 1;
}

static duk_ret_t OuterHTMLGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukEventTarget::GetNativeThis<Element>(ctx);
    return Duk::PushString(ctx, element->outerHTML());
}

static duk_ret_t OuterHTMLSetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukEventTarget::GetNativeThis<Element>(ctx);

    DukExceptionState es(ctx, "outerHTML", DukElement::ProtoName);
    element->setOuterHTML(Duk::ToWTFString(ctx, 0), es);
    if (es.hadException())
        es.throwIfNeeded();
    return 0;
}

static duk_ret_t NamespaceURIGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukEventTarget::GetNativeThis<Element>(ctx);
    return Duk::PushString(ctx, element->namespaceURI());
}

static duk_ret_t PreviousElementSiblingGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukEventTarget::GetNativeThis<Element>(ctx);

    Element *ret = NonDocumentTypeChildNode::previousElementSibling(*element);
    DukContext::From(ctx)->PushNode(ctx, ret);
    return 1;
}

static duk_ret_t RemoveAttribute(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukEventTarget::GetNativeThis<Element>(ctx);
    element->removeAttribute(Duk::ToAtomicString(ctx, 0));
    return 0;
}

static duk_ret_t RemoveAttributeNode(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t SetAttribute(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukEventTarget::GetNativeThis<Element>(ctx);

    DukExceptionState es(ctx, "setAttribute", DukElement::ProtoName);
    element->setAttribute(Duk::ToAtomicString(ctx, 0), Duk::ToAtomicString(ctx, 1), es);
    if (es.hadException())
        es.throwIfNeeded();
    return 0;
}

static duk_ret_t SetAttributeNode(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t TagNameGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukEventTarget::GetNativeThis<Element>(ctx);
    return Duk::PushString(ctx, element->tagName());
}

} // namespace Impl

#ifndef BLINKIT_CRAWLER_ONLY
namespace UI {

static duk_ret_t Focus(duk_context *ctx)
{
    duk_push_this(ctx);
    HTMLElement *element = DukEventTarget::GetNativeThis<HTMLElement>(ctx);

    element->focus();
    return 0;
}

static duk_ret_t StyleGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukEventTarget::GetNativeThis<Element>(ctx);

    DukContext::From(ctx)->PushObject<DukCSSStyleDeclaration>(ctx, element->style());
    return 1;
}

} // namespace UI
#endif // BLINKIT_CRAWLER_ONLY

void DukElement::RegisterToPrototypeEntryForCrawler(PrototypeEntry &entry)
{
    static const PrototypeEntry::Property Properties[] = {
        { "attributes",             Impl::AttributesGetter,             nullptr                    },
        { "children",               Impl::ChildrenGetter,               nullptr                    },
        { "classList",              Impl::ClassListGetter,              nullptr                    },
        { "className",              Impl::ClassNameGetter,              Impl::ClassNameSetter      },
        { "firstElementChild",      Impl::FirstElementChildGetter,      nullptr                    },
        { "id",                     Impl::IdGetter,                     Impl::IdSetter             },
        { "innerHTML",              Impl::InnerHTMLGetter,              Impl::InnerHTMLSetter      },
        { "innerText",              DukNode::TextContentGetter,         DukNode::TextContentSetter },
        { "lastElementChild",       Impl::LastElementChildGetter,       nullptr                    },
        { "namespaceURI",           Impl::NamespaceURIGetter,           nullptr                    },
        { "nextElementSibling",     Impl::NextElementSiblingGetter,     nullptr                    },
        { "outerHTML",              Impl::OuterHTMLGetter,              Impl::OuterHTMLSetter      },
        { "previousElementSibling", Impl::PreviousElementSiblingGetter, nullptr                    },
        { "tagName",                Impl::TagNameGetter,                nullptr                    },
    };
    static const PrototypeEntry::Method Methods[] = {
        { "focus",                  Crawler::NothingToDo,      0 },
        { "getAttribute",           Impl::GetAttribute,        1 },
        { "getAttributeNode",       Impl::GetAttributeNode,    1 },
        { "hasAttribute",           Impl::HasAttribute,        1 },
        { "hasAttributes",          Impl::HasAttributes,       0 },
        { "removeAttribute",        Impl::RemoveAttribute,     1 },
        { "removeAttributeNode",    Impl::RemoveAttributeNode, 1 },
        { "setAttribute",           Impl::SetAttribute,        2 },
        { "setAttributeNode",       Impl::SetAttributeNode,    1 },
    };

    DukContainerNode::RegisterToPrototypeEntry(entry);
    entry.Add(Properties, WTF_ARRAY_LENGTH(Properties));
    entry.AddObject("style");
    entry.Add(Methods, WTF_ARRAY_LENGTH(Methods));
}

void DukElement::RegisterPrototypeForCrawler(duk_context *ctx, PrototypeManager &protos)
{
    const auto worker = [](PrototypeEntry &entry)
    {
        RegisterToPrototypeEntryForCrawler(entry);
    };
    protos.Register(ctx, ProtoName, worker);
}

#ifndef BLINKIT_CRAWLER_ONLY

void DukElement::RegisterToPrototypeEntryForUI(PrototypeEntry &entry)
{
    static const PrototypeEntry::Property Properties[] = {
        { "attributes",             Impl::AttributesGetter,             nullptr                    },
        { "children",               Impl::ChildrenGetter,               nullptr                    },
        { "classList",              Impl::ClassListGetter,              nullptr                    },
        { "className",              Impl::ClassNameGetter,              Impl::ClassNameSetter      },
        { "firstElementChild",      Impl::FirstElementChildGetter,      nullptr                    },
        { "id",                     Impl::IdGetter,                     Impl::IdSetter             },
        { "innerHTML",              Impl::InnerHTMLGetter,              Impl::InnerHTMLSetter      },
        { "innerText",              DukNode::TextContentGetter,         DukNode::TextContentSetter },
        { "lastElementChild",       Impl::LastElementChildGetter,       nullptr                    },
        { "nextElementSibling",     Impl::NextElementSiblingGetter,     nullptr                    },
        { "outerHTML",              Impl::OuterHTMLGetter,              Impl::OuterHTMLSetter      },
        { "previousElementSibling", Impl::PreviousElementSiblingGetter, nullptr                    },
        { "style",                  UI::StyleGetter,                    nullptr                    },
        { "tagName",                Impl::TagNameGetter,                nullptr                    },
    };
    static const PrototypeEntry::Method Methods[] = {
        { "focus",                  UI::Focus,                 0 },
        { "getAttribute",           Impl::GetAttribute,        1 },
        { "getAttributeNode",       Impl::GetAttributeNode,    1 },
        { "hasAttribute",           Impl::HasAttribute,        1 },
        { "hasAttributes",          Impl::HasAttributes,       0 },
        { "removeAttribute",        Impl::RemoveAttribute,     1 },
        { "removeAttributeNode",    Impl::RemoveAttributeNode, 1 },
        { "setAttribute",           Impl::SetAttribute,        2 },
        { "setAttributeNode",       Impl::SetAttributeNode,    1 },
    };

    DukContainerNode::RegisterToPrototypeEntry(entry);
    entry.Add(Properties, WTF_ARRAY_LENGTH(Properties));
    entry.Add(Methods, WTF_ARRAY_LENGTH(Methods));
}

void DukElement::RegisterPrototypeForUI(duk_context *ctx, PrototypeManager &protos)
{
    const auto worker = [](PrototypeEntry &entry)
    {
        RegisterToPrototypeEntryForUI(entry);
    };
    protos.Register(ctx, ProtoName, worker);
}

#endif // BLINKIT_CRAWLER_ONLY

} // namespace BlinKit
