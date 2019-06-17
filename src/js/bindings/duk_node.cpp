// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_node.cpp
// Description: Bindings for Node
//      Author: Ziming Li
//     Created: 2019-06-01
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_node.h"

#include "core/dom/Document.h"

#include "bindings/duk_exception_state.h"
#include "context/duk_context.h"
#include "context/prototype_manager.h"
#include "wrappers/duk.h"

using namespace blink;

namespace BlinKit {

namespace Impl {

static duk_ret_t AppendChild(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukEventTarget::GetNativeThis<Node>(ctx);

    Node *newChild = DukEventTarget::GetNativeThis<Node>(ctx, 0);

    DukExceptionState es(ctx, "appendChild", "Node");
    PassRefPtr<Node> ret = node->appendChild(newChild, es);
    if (es.hadException())
    {
        es.throwIfNeeded();
        return 0;
    }

    DukContext::From(ctx)->PushNode(ctx, ret.get());
    return 1;
}

static duk_ret_t ChildNodesGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t CloneNode(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukEventTarget::GetNativeThis<Node>(ctx);
    PassRefPtr<Node> ret = node->cloneNode(duk_to_boolean(ctx, 0));
    DukContext::From(ctx)->PushNode(ctx, ret.get());
    return 1;
}

static duk_ret_t CompareDocumentPosition(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukEventTarget::GetNativeThis<Node>(ctx);
    Node *another = DukEventTarget::GetNativeThis<Node>(ctx, 0);
    duk_push_boolean(ctx, node->compareDocumentPosition(another));
    return 1;
}

static duk_ret_t Contains(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukEventTarget::GetNativeThis<Node>(ctx);
    Node *another = DukEventTarget::GetNativeThis<Node>(ctx, 0);
    duk_push_boolean(ctx, node->contains(another));
    return 1;
}

static duk_ret_t FirstChildGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukEventTarget::GetNativeThis<Node>(ctx);
    DukContext::From(ctx)->PushNode(ctx, node->firstChild());
    return 1;
}

static duk_ret_t HasChildNodes(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukEventTarget::GetNativeThis<Node>(ctx);
    duk_push_boolean(ctx, node->hasChildren());
    return 1;
}

static duk_ret_t InsertBefore(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukEventTarget::GetNativeThis<Node>(ctx);

    Node *newChild = DukEventTarget::GetNativeThis<Node>(ctx, 0);
    Node *refChild = DukEventTarget::GetNativeThis<Node>(ctx, 1);

    DukExceptionState es(ctx, "insertBefore", "Node");
    PassRefPtr<Node> ret = node->insertBefore(newChild, refChild, es);
    if (es.hadException())
    {
        es.throwIfNeeded();
        return 0;
    }

    DukContext::From(ctx)->PushNode(ctx, ret.get());
    return 1;
}

static duk_ret_t IsDefaultNamespace(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukEventTarget::GetNativeThis<Node>(ctx);
    duk_push_boolean(ctx, node->isDefaultNamespace(Duk::ToAtomicString(ctx, 0)));
    return 1;
}

static duk_ret_t IsEqualNode(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukEventTarget::GetNativeThis<Node>(ctx);
    Node *another = DukEventTarget::GetNativeThis<Node>(ctx, 0);
    duk_push_boolean(ctx, node->isEqualNode(another));
    return 1;
}

static duk_ret_t IsSameNode(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukEventTarget::GetNativeThis<Node>(ctx);
    Node *another = DukEventTarget::GetNativeThis<Node>(ctx, 0);
    duk_push_boolean(ctx, node->isSameNode(another));
    return 1;
}

static duk_ret_t LastChildGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukEventTarget::GetNativeThis<Node>(ctx);
    DukContext::From(ctx)->PushNode(ctx, node->lastChild());
    return 1;
}

static duk_ret_t NextSiblingGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukEventTarget::GetNativeThis<Node>(ctx);
    DukContext::From(ctx)->PushNode(ctx, node->nextSibling());
    return 1;
}

static duk_ret_t NodeNameGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukEventTarget::GetNativeThis<Node>(ctx);
    return Duk::PushString(ctx, node->nodeName());
}

static duk_ret_t NodeTypeGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukEventTarget::GetNativeThis<Node>(ctx);
    duk_push_uint(ctx, node->nodeType());
    return 1;
}

static duk_ret_t NodeValueGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukEventTarget::GetNativeThis<Node>(ctx);
    return Duk::PushString(ctx, node->nodeValue());
}

static duk_ret_t NodeValueSetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukEventTarget::GetNativeThis<Node>(ctx);
    node->setNodeValue(Duk::ToWTFString(ctx, 0));
    return 0;
}

static duk_ret_t Normalize(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukEventTarget::GetNativeThis<Node>(ctx);
    node->normalize();
    return 0;
}

static duk_ret_t OwnerDocumentGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukEventTarget::GetNativeThis<Node>(ctx);
    DukContext::From(ctx)->PushNode(ctx, node->ownerDocument());
    return 1;
}

static duk_ret_t ParentNodeGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukEventTarget::GetNativeThis<Node>(ctx);
    DukContext::From(ctx)->PushNode(ctx, node->parentNode());
    return 1;
}

static duk_ret_t PreviousSiblingGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukEventTarget::GetNativeThis<Node>(ctx);
    DukContext::From(ctx)->PushNode(ctx, node->previousSibling());
    return 1;
}

static duk_ret_t Remove(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukEventTarget::GetNativeThis<Node>(ctx);

    DukExceptionState es(ctx, "remove", "Node");
    node->remove(es);
    if (es.hadException())
        es.throwIfNeeded();
    return 0;
}

static duk_ret_t RemoveChild(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukEventTarget::GetNativeThis<Node>(ctx);

    Node *child = DukEventTarget::GetNativeThis<Node>(ctx, 0);

    DukExceptionState es(ctx, "replaceChild", "Node");
    PassRefPtr<Node> ret = node->removeChild(child, es);
    if (es.hadException())
    {
        es.throwIfNeeded();
        return 0;
    }

    DukContext::From(ctx)->PushNode(ctx, ret.get());
    return 1;
}

static duk_ret_t ReplaceChild(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukEventTarget::GetNativeThis<Node>(ctx);

    Node *newChild = DukEventTarget::GetNativeThis<Node>(ctx, 0);
    Node *oldChild = DukEventTarget::GetNativeThis<Node>(ctx, 1);

    DukExceptionState es(ctx, "replaceChild", "Node");
    PassRefPtr<Node> ret = node->replaceChild(newChild, oldChild, es);
    if (es.hadException())
    {
        es.throwIfNeeded();
        return 0;
    }

    DukContext::From(ctx)->PushNode(ctx, ret.get());
    return 1;
}

} // namespace Impl

void DukNode::RegisterToPrototypeEntry(PrototypeEntry &entry)
{
    static const PrototypeEntry::Property Properties[] = {
        { "childNodes",      Impl::ChildNodesGetter,      nullptr               },
        { "firstChild",      Impl::FirstChildGetter,      nullptr               },
        { "lastChild",       Impl::LastChildGetter,       nullptr               },
        { "nextSibling",     Impl::NextSiblingGetter,     nullptr               },
        { "nodeName",        Impl::NodeNameGetter,        nullptr               },
        { "nodeType",        Impl::NodeTypeGetter,        nullptr               },
        { "nodeValue",       Impl::NodeValueGetter,       Impl::NodeValueSetter },
        { "ownerDocument",   Impl::OwnerDocumentGetter,   nullptr               },
        { "parentNode",      Impl::ParentNodeGetter,      nullptr               },
        { "previousSibling", Impl::PreviousSiblingGetter, nullptr               },
        { "textContent",     TextContentGetter,           TextContentSetter     },
    };
    static const PrototypeEntry::Method Methods[] = {
        { "appendChild",             Impl::AppendChild,             1 },
        { "cloneNode",               Impl::CloneNode,               1 },
        { "compareDocumentPosition", Impl::CompareDocumentPosition, 1 },
        { "contains",                Impl::Contains,                1 },
        { "hasChildNodes",           Impl::HasChildNodes,           0 },
        { "insertBefore",            Impl::InsertBefore,            2 },
        { "isDefaultNamespace",      Impl::IsDefaultNamespace,      1 },
        { "isEqualNode",             Impl::IsEqualNode,             1 },
        { "isSameNode",              Impl::IsSameNode,              1 },
        { "normalize",               Impl::Normalize,               0 },
        { "remove",                  Impl::Remove,                  0 },
        { "removeChild",             Impl::RemoveChild,             1 },
        { "replaceChild",            Impl::ReplaceChild,            2 },
    };

    DukEventTarget::RegisterToPrototypeEntry(entry);
    entry.Add(Properties, WTF_ARRAY_LENGTH(Properties));
    entry.Add(Methods, WTF_ARRAY_LENGTH(Methods));
}

duk_ret_t DukNode::TextContentGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukEventTarget::GetNativeThis<Node>(ctx);
    return Duk::PushString(ctx, node->textContent());
}

duk_ret_t DukNode::TextContentSetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukEventTarget::GetNativeThis<Node>(ctx);
    node->setTextContent(Duk::ToWTFString(ctx, 0));
    return 0;
}

} // namespace BlinKit
