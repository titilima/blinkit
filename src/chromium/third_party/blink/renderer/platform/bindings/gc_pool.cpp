// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: gc_pool.cpp
// Description: GCPool Class
//      Author: Ziming Li
//     Created: 2020-03-15
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "gc_pool.h"

#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/platform/bindings/script_wrappable.h"

using namespace blink;

namespace BlinKit {

void GCPool::CollectGarbage(void)
{
    for (ScriptWrappable *object : m_objects)
        object->PreCollectGarbage(*this);
    for (ScriptWrappable *object : m_objects)
        delete object;
    m_objects.clear();
}

GCPool& GCPool::From(const Document &document)
{
    return document.GetFrame()->GetGCPool();
}

void GCPool::Restore(ScriptWrappable &object)
{
    m_objects.erase(&object);
    object.m_inGCPool = false;
}

void GCPool::Save(ScriptWrappable &object)
{
    ASSERT(object.CanBePooled());
    if (!object.IsContextRetained())
    {
        m_objects.insert(&object);
        object.m_inGCPool = true;
    }
}

} // namespace BlinKit
