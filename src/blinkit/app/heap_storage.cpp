// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: heap_storage.cpp
// Description: HeapStorage Class
//      Author: Ziming Li
//     Created: 2020-10-21
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "heap_storage.h"

#include "third_party/blink/renderer/platform/wtf/wtf.h"

namespace BlinKit {

HeapStorage* HeapStorage::theStorage = nullptr;

HeapStorage::HeapStorage(void)
{
    ASSERT(nullptr == theStorage);
    theStorage = this;
}

HeapStorage::~HeapStorage(void)
{
    for (auto &it : m_persistentObjectFinalizers)
        it.second();
    m_persistentObjectFinalizers.clear();

    ASSERT(theStorage == this);
    theStorage = nullptr;
}

bool HeapStorage::PersistentObjectSaved(void *p)
{
    auto it = m_persistentObjectFinalizers.find(p);
    return std::end(m_persistentObjectFinalizers) != it;
}

void HeapStorage::SavePersistentObjectImpl(void *p, Finalizer &&finalizer)
{
    ASSERT(IsMainThread());
    ASSERT(!PersistentObjectSaved(p));
    m_persistentObjectFinalizers[p] = finalizer;
}

} // namespace BlinKit
