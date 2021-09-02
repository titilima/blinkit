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

#include "./script_wrappers.h"

#include "blinkit/blink/renderer/bindings/core/duk/duk_script_object.h"
#include "blinkit/blink/renderer/bindings/core/duk/script_wrappable.h"

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
        if (GCObject *o = m_nativeObject->ObjectForGC())
            o->IncRef();
    }

    ASSERT(duk_get_heapptr(m_ctx, -1) == m_nativeObject->m_contextObject);
}

} // namespace BlinKit
