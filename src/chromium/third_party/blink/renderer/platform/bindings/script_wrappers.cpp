// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: script_wrappers.cpp
// Description: Script Wrapper Classes
//      Author: Ziming Li
//     Created: 2020-03-15
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "script_wrappers.h"

#include "blinkit/js/context_impl.h"
#include "third_party/blink/renderer/core/dom/node.h"

using namespace blink;

namespace BlinKit {

PushWrapper::PushWrapper(duk_context *ctx, ScriptWrappable *nativeObject) : m_ctx(ctx), m_nativeObject(nativeObject)
{
}

PushWrapper::~PushWrapper(void)
{
    if (nullptr == m_nativeObject)
        return;

    if (nullptr == m_nativeObject->m_contextObject)
    {
        m_nativeObject->m_contextObject = duk_get_heapptr(m_ctx, -1);
        if (m_nativeObject->GetGCType() == ScriptWrappable::GC_IN_FINALIZER)
            m_nativeObject->SetGarbageFlag();
    }

    ASSERT(duk_get_heapptr(m_ctx, -1) == m_nativeObject->m_contextObject);
}

NodePushWrapper::NodePushWrapper(duk_context *ctx, Node *node) : PushWrapper(ctx, node)
{
    if (nullptr != node && nullptr == node->ParentOrShadowHostNode())
        node->SetGarbageFlag();
}

} // namespace BlinKit
