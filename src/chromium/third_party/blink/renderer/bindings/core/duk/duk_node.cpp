// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_node.cpp
// Description: DukNode Class
//      Author: Ziming Li
//     Created: 2020-02-07
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "duk_node.h"

#include "third_party/blink/renderer/bindings/core/duk/duk.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_attr.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_document.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_element.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_node_list.h"
#include "third_party/blink/renderer/platform/bindings/script_wrappers.h"

using namespace blink;

namespace BlinKit {

namespace ProtoNames {
const char Comment[] = "Comment";
const char DocumentFragment[] = "DocumentFragment";
const char DocumentType[] = "DocumentType";
const char Text[] = "Text";
}

namespace Impl {

static duk_ret_t AppendChild(duk_context *ctx)
{
    Node *newChild = DukScriptObject::To<Node>(ctx, 0);

    duk_push_this(ctx);
    Node *node = DukScriptObject::To<Node>(ctx, -1);

    DukExceptionState exceptionState(ctx);
    Node *ret = node->appendChild(newChild, exceptionState);
    if (exceptionState.HadException())
    {
        exceptionState.ThrowIfNeeded();
        return 0;
    }

    DukNode::Push(ctx, ret);
    return 1;
}

static duk_ret_t ChildNodesGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukScriptObject::To<Node>(ctx, -1);
    DukNodeList::Push(ctx, node->childNodes());
    return 1;
}

static duk_ret_t CloneNode(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukScriptObject::To<Node>(ctx, -1);

    DukExceptionState exceptionState(ctx);
    Node *ret = node->cloneNode(duk_to_boolean(ctx, 0), exceptionState);
    if (exceptionState.HadException())
    {
        exceptionState.ThrowIfNeeded();
        return 0;
    }

    DukNode::Push(ctx, ret);
    return 1;
}

static duk_ret_t FirstChildGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukScriptObject::To<Node>(ctx, -1);
    DukNode::Push(ctx, node->firstChild());
    return 1;
}

static duk_ret_t InsertBefore(duk_context *ctx)
{
    Node *newChild = DukScriptObject::To<Node>(ctx, 0);
    Node *refChild = DukScriptObject::To<Node>(ctx, 1);

    duk_push_this(ctx);
    Node *node = DukScriptObject::To<Node>(ctx, -1);

    DukExceptionState exceptionState(ctx);
    Node *ret = node->insertBefore(newChild, refChild, exceptionState);
    if (exceptionState.HadException())
    {
        exceptionState.ThrowIfNeeded();
        return 0;
    }

    DukNode::Push(ctx, ret);
    return 1;
}

static duk_ret_t LastChildGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukScriptObject::To<Node>(ctx, -1);
    DukNode::Push(ctx, node->lastChild());
    return 1;
}

static duk_ret_t NodeNameGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukScriptObject::To<Node>(ctx, -1);
    Duk::PushString(ctx, node->nodeName());
    return 1;
}

static duk_ret_t NodeTypeGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukScriptObject::To<Node>(ctx, -1);
    duk_push_uint(ctx, node->nodeType());
    return 1;
}

static duk_ret_t OwnerDocumentGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukScriptObject::To<Node>(ctx, -1);
    DukScriptObject::Push<DukDocument>(ctx, node->ownerDocument());
    return 1;
}

static duk_ret_t ParentNodeGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukScriptObject::To<Node>(ctx, -1);
    DukNode::Push(ctx, node->parentNode());
    return 1;
}

static duk_ret_t Remove(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukScriptObject::To<Node>(ctx, -1);

    DukExceptionState exceptionState(ctx);
    node->remove(exceptionState);
    if (exceptionState.HadException())
        exceptionState.ThrowIfNeeded();
    return 0;
}

static duk_ret_t RemoveChild(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukScriptObject::To<Node>(ctx, -1);

    Node *child = DukScriptObject::To<Node>(ctx, 0);

    DukExceptionState exceptionState(ctx);
    Node *ret = node->removeChild(child, exceptionState);
    if (exceptionState.HadException())
    {
        exceptionState.ThrowIfNeeded();
        return 0;
    }

    DukNode::Push(ctx, ret);
    return 1;
}

} // namespace Impl

void DukNode::FillPrototypeEntry(PrototypeEntry &entry)
{
    static const PrototypeEntry::Method Methods[] = {
        { "appendChild",             Impl::AppendChild,             1 },
        { "cloneNode",               Impl::CloneNode,               1 },
        { "insertBefore",            Impl::InsertBefore,            2 },
        { "removeChild",             Impl::RemoveChild,             1 },
        { "remove",                  Impl::Remove,                  0 },
    };
    static const PrototypeEntry::Property Properties[] = {
        { "childNodes",      Impl::ChildNodesGetter,      nullptr               },
        { "firstChild",      Impl::FirstChildGetter,      nullptr               },
        { "lastChild",       Impl::LastChildGetter,       nullptr               },
        { "nodeName",        Impl::NodeNameGetter,        nullptr               },
        { "nodeType",        Impl::NodeTypeGetter,        nullptr               },
        { "ownerDocument",   Impl::OwnerDocumentGetter,   nullptr               },
        { "parentNode",      Impl::ParentNodeGetter,      nullptr               },
        { "textContent",     TextContentGetter,           TextContentSetter     },
    };

    DukEventTarget::FillPrototypeEntry(entry);
    entry.Add(Methods, std::size(Methods));
    entry.Add(Properties, std::size(Properties));
}

duk_idx_t DukNode::Push(duk_context *ctx, Node *node)
{
    PushWrapper w(ctx, node);
    do {
        if (DukScriptObject::Push(ctx, node))
            break;

        ASSERT(nullptr != node);
        if (node->IsElementNode())
        {
            DukElement::Create(ctx, ToElement(*node));
            break;
        }
        if (node->IsTextNode())
        {
            PrototypeHelper::CreateScriptObject(ctx, ProtoNames::Text, node);
            break;
        }
        if (node->IsDocumentFragment())
        {
            PrototypeHelper::CreateScriptObject(ctx, ProtoNames::DocumentFragment, node);
            break;
        }
        if (node->IsAttributeNode())
        {
            PrototypeHelper::CreateScriptObject(ctx, DukAttr::ProtoName, node);
            break;
        }
        if (node->getNodeType() == Node::kCommentNode)
        {
            PrototypeHelper::CreateScriptObject(ctx, ProtoNames::Comment, node);
            break;
        }
        if (node->getNodeType() == Node::kDocumentTypeNode)
        {
            PrototypeHelper::CreateScriptObject(ctx, ProtoNames::DocumentType, node);
            break;
        }

        ASSERT(false); // BKTODO:
        duk_push_undefined(ctx);
    } while (false);
    return duk_get_top_index(ctx);
}

void DukNode::RegisterPrototype(PrototypeHelper &helper, const char *name)
{
    helper.Register(name, FillPrototypeEntry);
}

duk_ret_t DukNode::TextContentGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukScriptObject::To<Node>(ctx, -1);
    Duk::PushString(ctx, node->textContent());
    return 1;
}

duk_ret_t DukNode::TextContentSetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukScriptObject::To<Node>(ctx, -1);
    node->setTextContent(Duk::To<String>(ctx, 0));
    return 0;
}

} // namespace BlinKit
