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
#include "third_party/blink/renderer/platform/bindings/gc_pool.h"
#include "third_party/blink/renderer/platform/bindings/script_wrappable.h"

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
        m_nativeObject->m_contextObject = duk_get_heapptr(m_ctx, -1);
    else
        ASSERT(duk_get_heapptr(m_ctx, -1) == m_nativeObject->m_contextObject);

    if (m_nativeObject->IsInGCPool())
    {
        m_nativeObject->RetainByContext();
        ContextImpl::From(m_ctx)->GetGCPool().Restore(*m_nativeObject);
    }
}

} // namespace BlinKit
