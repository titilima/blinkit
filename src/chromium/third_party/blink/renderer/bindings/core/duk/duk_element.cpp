// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_element.cpp
// Description: DukElement Class
//      Author: Ziming Li
//     Created: 2020-02-11
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "duk_element.h"

#include "blinkit/js/browser_context.h"
#include "third_party/blink/renderer/bindings/core/duk/duk.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_anchor_element.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_event_listener.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_exception_state.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_image_element.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_named_node_map.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_script_element.h"
#include "third_party/blink/renderer/core/event_type_names.h"
#include "third_party/blink/renderer/core/dom/non_document_type_child_node.h"
#include "third_party/blink/renderer/core/dom/parent_node.h"
#include "third_party/blink/renderer/platform/bindings/gc_pool.h"
#include "third_party/blink/renderer/platform/bindings/script_wrappers.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"

using namespace blink;

namespace BlinKit {

const char DukElement::ProtoName[] = "HTMLElement";

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Impl {

static duk_ret_t AttributesGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukScriptObject::To<Element>(ctx, -1);

    DukNamedNodeMap::Push(ctx, element->attributes());
    return 1;
}

static duk_ret_t FirstElementChildGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukScriptObject::To<Element>(ctx, -1);
    DukElement::Push(ctx, ParentNode::firstElementChild(*element));
    return 1;
}

static duk_ret_t GetAttribute(duk_context *ctx)
{
    const AtomicString name = Duk::To<AtomicString>(ctx, 0);

    duk_push_this(ctx);
    Element *element = DukScriptObject::To<Element>(ctx, -1);

    const AtomicString &ret = element->getAttribute(QualifiedName(g_null_atom, name, g_null_atom));
    Duk::PushString(ctx, ret);
    return 1;
}

static duk_ret_t InnerHTMLGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukScriptObject::To<Element>(ctx, -1);
    Duk::PushString(ctx, element->innerHTML());
    return 1;
}

static duk_ret_t InnerHTMLSetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukScriptObject::To<Element>(ctx, -1);

    NodeVector detachedChildren;
    DukExceptionState exceptionState(ctx);
    element->setInnerHTML(Duk::To<String>(ctx, 0), detachedChildren, exceptionState);
    if (exceptionState.HadException())
        exceptionState.ThrowIfNeeded();

    GCPool::From(ctx)->Save(detachedChildren);
    return 0;
}

static duk_ret_t LastElementChildGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukScriptObject::To<Element>(ctx, -1);
    DukElement::Push(ctx, ParentNode::lastElementChild(*element));
    return 1;
}

static duk_ret_t NextElementSiblingGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukScriptObject::To<Element>(ctx, -1);
    DukElement::Push(ctx, NonDocumentTypeChildNode::nextElementSibling(*element));
    return 1;
}

static duk_ret_t OnloadGetter(duk_context *ctx)
{
    ASSERT(false); // BKTODO:
    return 0;
}

static duk_ret_t OnloadSetter(duk_context *ctx)
{
    return DukElement::SetAttributeEventListenerImpl(ctx, event_type_names::kLoad);
}

static duk_ret_t OuterHTMLGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukScriptObject::To<Element>(ctx, -1);
    Duk::PushString(ctx, element->outerHTML());
    return 1;
}

static duk_ret_t OuterHTMLSetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukScriptObject::To<Element>(ctx, -1);

    NodeVector detachedNodes;
    DukExceptionState exceptionState(ctx);
    element->setOuterHTML(Duk::To<String>(ctx, 0), detachedNodes, exceptionState);
    if (exceptionState.HadException())
        exceptionState.ThrowIfNeeded();

    GCPool::From(ctx)->Save(detachedNodes);
    return 0;
}

static duk_ret_t PreviousElementSiblingGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukScriptObject::To<Element>(ctx, -1);
    DukElement::Push(ctx, NonDocumentTypeChildNode::previousElementSibling(*element));
    return 1;
}

static duk_ret_t SetAttribute(duk_context *ctx)
{
    const AtomicString name = Duk::To<AtomicString>(ctx, 0);
    const AtomicString value = Duk::To<AtomicString>(ctx, 1);

    duk_push_this(ctx);
    Element *element = DukScriptObject::To<Element>(ctx, -1);

    DukExceptionState exceptionState(ctx);
    element->setAttribute(name, value, exceptionState);
    if (exceptionState.HadException())
        exceptionState.ThrowIfNeeded();
    return 0;
}

static duk_ret_t TagNameGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukScriptObject::To<Element>(ctx, -1);
    Duk::PushString(ctx, element->tagName());
    return 1;
}

static duk_ret_t ToString(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukScriptObject::To<Element>(ctx, -1);

    std::string ret("[object HTML");
    ret += element->tagName().StdUtf8();
    ret += "Element]";
    ret.at(12) = toupper(ret.at(12));
    Duk::PushString(ctx, ret);
    return 1;
}

} // namespace Impl

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DukElement::Create(duk_context *ctx, Element &element)
{
    const char *protoName = BrowserContext::From(ctx)->LookupPrototypeName(element.nodeName().StdUtf8());
    PrototypeHelper::CreateScriptObject(ctx, protoName, &element);
}

void DukElement::FillPrototypeEntryForCrawler(PrototypeEntry &entry)
{
    static const PrototypeEntry::Method Methods[] = {
        { "getAttribute",           Impl::GetAttribute,        1 },
        { "setAttribute",           Impl::SetAttribute,        2 },
        { "toString",               Impl::ToString,            0 },
    };
    static const PrototypeEntry::Property Properties[] = {
        { "attributes",             Impl::AttributesGetter,             nullptr                    },
        { "firstElementChild",      Impl::FirstElementChildGetter,      nullptr                    },
        { "innerHTML",              Impl::InnerHTMLGetter,              Impl::InnerHTMLSetter      },
        { "lastElementChild",       Impl::LastElementChildGetter,       nullptr                    },
        { "nextElementSibling",     Impl::NextElementSiblingGetter,     nullptr                    },
        { "onload",                 Impl::OnloadGetter,                 Impl::OnloadSetter         },
        { "outerHTML",              Impl::OuterHTMLGetter,              Impl::OuterHTMLSetter      },
        { "previousElementSibling", Impl::PreviousElementSiblingGetter, nullptr                    },
        { "tagName",                Impl::TagNameGetter,                nullptr                    },
    };

    DukContainerNode::FillPrototypeEntry(entry);
    entry.Add(Methods, std::size(Methods));
    entry.Add(Properties, std::size(Properties));
    entry.Add("style", DUK_TYPE_OBJECT);
}

duk_ret_t DukElement::GetURLAttributeImpl(duk_context *ctx, const QualifiedName &name)
{
    duk_push_this(ctx);
    Element *element = DukScriptObject::To<Element>(ctx, -1);

    GURL ret = element->GetURLAttribute(name);
    Duk::PushString(ctx, ret.spec());
    return 1;
}

const std::unordered_map<std::string, const char *>& DukElement::PrototypeMapForCrawler(void)
{
    static std::unordered_map<std::string, const char *> s_prototypeMapForCrawler;

    ASSERT(IsMainThread());
    if (s_prototypeMapForCrawler.empty())
    {
        s_prototypeMapForCrawler.insert({
            { "a",      DukAnchorElement::ProtoName },
            { "img",    DukImageElement::ProtoName  },
            { "script", DukScriptElement::ProtoName }
        });
    }

    return s_prototypeMapForCrawler;
}

duk_idx_t DukElement::Push(duk_context *ctx, Element *element)
{
    NodePushWrapper w(ctx, element);
    if (!DukScriptObject::Push(ctx, element))
    {
        ASSERT(nullptr != element);
        Create(ctx, *element);
    }
    return duk_get_top_index(ctx);
}

void DukElement::RegisterPrototypeForCrawler(PrototypeHelper &helper)
{
    helper.Register(ProtoName, FillPrototypeEntryForCrawler);
}

duk_ret_t DukElement::SetAttributeEventListenerImpl(duk_context *ctx, const AtomicString &attrName)
{
    duk_push_this(ctx);
    Element *element = DukScriptObject::To<Element>(ctx, -1);

    std::shared_ptr<EventListener> listener = DukEventListener::Get(ctx, 0, element, attrName, true);
    element->SetAttributeEventListener(attrName, listener.get());

    return 0;
}

duk_ret_t DukElement::SetAttributeImpl(duk_context *ctx, const QualifiedName &name)
{
    const AtomicString value = Duk::To<AtomicString>(ctx, 0);

    duk_push_this(ctx);
    Element *element = DukScriptObject::To<Element>(ctx, -1);

    DukExceptionState exceptionState(ctx);
    element->setAttribute(name.LocalName(), value, exceptionState);
    if (exceptionState.HadException())
        exceptionState.ThrowIfNeeded();
    return 0;
}

} // namespace BlinKit
