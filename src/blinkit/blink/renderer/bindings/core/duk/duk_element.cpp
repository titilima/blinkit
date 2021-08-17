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

#include "./duk_element.h"

#include "blinkit/blink/renderer/bindings/core/duk/duk.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_anchor_element.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_event_listener.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_exception_state.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_image_element.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_named_node_map.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_script_element.h"
#include "blinkit/blink/renderer/bindings/core/duk/script_controller.h"
#include "blinkit/blink/renderer/bindings/core/duk/script_wrappers.h"
#include "blinkit/blink/renderer/core/dom/NonDocumentTypeChildNode.h"
#include "blinkit/blink/renderer/core/dom/ParentNode.h"
#include "blinkit/blink/renderer/wtf/MainThread.h"

using namespace blink;

namespace BlinKit {

const char DukElement::ProtoName[] = "HTMLElement";

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Impl {

static duk_ret_t AttributesGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Element *element = DukScriptObject::To<Element>(ctx, -1);

    DukNamedNodeMap::Push(ctx, element->attributesForBindings());
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

    const AtomicString &ret = element->getAttribute(QualifiedName(nullAtom, name, HTMLNames::xhtmlNamespaceURI));
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

    DukExceptionState exceptionState(ctx);
    element->setInnerHTML(Duk::To<String>(ctx, 0), exceptionState);
    if (exceptionState.hadException())
        exceptionState.ThrowIfNeeded();
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
    return DukElement::SetAttributeEventListenerImpl(ctx, EventTypeNames::load);
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
    element->setOuterHTML(Duk::To<String>(ctx, 0), exceptionState);
    if (exceptionState.hadException())
        exceptionState.ThrowIfNeeded();
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
    if (exceptionState.hadException())
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
    ret += element->tagName().stdUtf8();
    ret += "Element]";
    ret.at(12) = toupper(ret.at(12));
    Duk::PushString(ctx, ret);
    return 1;
}

} // namespace Impl

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DukElement::Create(duk_context *ctx, Element &element)
{
    const char *protoName = ScriptController::From(ctx)->LookupPrototypeName(element.nodeName().stdUtf8());
    PrototypeHelper::CreateScriptObject(ctx, protoName, &element);
}

duk_ret_t DukElement::GetURLAttributeImpl(duk_context *ctx, const QualifiedName &name)
{
    duk_push_this(ctx);
    Element *element = DukScriptObject::To<Element>(ctx, -1);

    zed::url ret = element->getURLAttribute(name);
    Duk::PushString(ctx, ret.spec());
    return 1;
}

duk_idx_t DukElement::Push(duk_context *ctx, Element *element)
{
    PushWrapper w(ctx, element);
    if (!DukScriptObject::Push(ctx, element))
    {
        ASSERT(nullptr != element);
        Create(ctx, *element);
    }
    return duk_get_top_index(ctx);
}

duk_ret_t DukElement::SetAttributeEventListenerImpl(duk_context *ctx, const AtomicString &attrName)
{
    duk_push_this(ctx);
    Element *element = DukScriptObject::To<Element>(ctx, -1);

    std::shared_ptr<EventListener> listener = DukEventListener::Get(ctx, 0, element, attrName, true);
    element->setAttributeEventListener(attrName, listener.get());

    return 0;
}

duk_ret_t DukElement::SetAttributeImpl(duk_context *ctx, const QualifiedName &name)
{
    const AtomicString value = Duk::To<AtomicString>(ctx, 0);

    duk_push_this(ctx);
    Element *element = DukScriptObject::To<Element>(ctx, -1);

    DukExceptionState exceptionState(ctx);
    element->setAttribute(name.localName(), value, exceptionState);
    if (exceptionState.hadException())
        exceptionState.ThrowIfNeeded();
    return 0;
}

#ifdef BLINKIT_CRAWLER_ENABLED
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

const std::unordered_map<std::string, const char *>& DukElement::PrototypeMapForCrawler(void)
{
    static std::unordered_map<std::string, const char *> s_prototypeMapForCrawler;

    ASSERT(isMainThread());
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

void DukElement::RegisterPrototypeForCrawler(PrototypeHelper &helper)
{
    helper.Register(ProtoName, FillPrototypeEntryForCrawler);
}
#endif // BLINKIT_CRAWLER_ENABLED

#ifdef BLINKIT_UI_ENABLED
void DukElement::FillPrototypeEntryForUI(PrototypeEntry &entry)
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

const std::unordered_map<std::string, const char *>& DukElement::PrototypeMapForUI(void)
{
    static std::unordered_map<std::string, const char *> s_prototypeMapForUI;

    ASSERT(isMainThread());
    if (s_prototypeMapForUI.empty())
    {
        s_prototypeMapForUI.insert({
            { "a",      DukAnchorElement::ProtoName },
            { "img",    DukImageElement::ProtoName  },
            { "script", DukScriptElement::ProtoName }
        });
    }

    return s_prototypeMapForUI;
}

void DukElement::RegisterPrototypeForUI(PrototypeHelper &helper)
{
    helper.Register(ProtoName, FillPrototypeEntryForUI);
}
#endif // BLINKIT_UI_ENABLED

} // namespace BlinKit
