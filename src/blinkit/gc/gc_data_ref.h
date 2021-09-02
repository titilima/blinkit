#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: gc_data_ref.h
// Description: GCDataRef Class
//      Author: Ziming Li
//     Created: 2021-09-02
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_GC_DATA_REF_H
#define BLINKIT_BLINKIT_GC_DATA_REF_H

#include "blinkit/gc/gc_def.h"

namespace BlinKit {

template <class T>
class GCDataRef final : public GCPersistentMember<T>
{
public:
    GCDataRef(void) = default;
    GCDataRef(const GCDataRef<T> &o)
    {
        GCPersistentMember<T>::operator=(o);
    }

    const T* get(void) const { return this->m_ptr; }

    const T& operator*() const { return *get(); }
    const T* operator->() const { return get(); }

    T* access(void)
    {
        if (!get()->IsUnique())
            GCPersistentMember<T>::operator=(get()->copy());
        return this->m_ptr;
    }

    void init(void)
    {
        ASSERT(nullptr == this->m_ptr);
        GCPersistentMember<T>::operator=(T::create());
    }

    bool operator==(const GCDataRef<T>& o) const
    {
        ASSERT(nullptr != this->m_ptr);
        ASSERT(nullptr != o.m_ptr);
        return this->m_ptr == o.m_ptr || *(this->m_ptr) == *(o.m_ptr);
    }

    bool operator!=(const GCDataRef<T>& o) const
    {
        ASSERT(nullptr != this->m_ptr);
        ASSERT(nullptr != o.m_ptr);
        return this->m_ptr != o.m_ptr && *(this->m_ptr) != *(o.m_ptr);
    }

    void operator=(std::nullptr_t) { this->Release(); }
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_GC_DATA_REF_H
