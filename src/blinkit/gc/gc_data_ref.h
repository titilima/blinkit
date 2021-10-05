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
class GCDataRef final
{
public:
    const T* get(void) const { return m_data.get(); }

    const T& operator*() const { return *get(); }
    const T* operator->() const { return get(); }

    T* access(void)
    {
        if (!m_data->IsUnique())
            m_data = m_data->copy();
        return m_data.get();
    }

    void init(void)
    {
        ASSERT(!m_data);
        m_data = T::create();
    }

    bool operator==(const GCDataRef<T> &o) const
    {
        ASSERT(m_data);
        ASSERT(o.m_data);
        return m_data == o.m_data || *m_data == *o.m_data;
    }
    bool operator!=(const GCDataRef<T> &o) const
    {
        ASSERT(m_data);
        ASSERT(o.m_data);
        return m_data != o.m_data && *m_data != *o.m_data;
    }

    void operator=(std::nullptr_t) { m_data.clear(); }
private:
    GCRefPtr<T> m_data;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_GC_DATA_REF_H
