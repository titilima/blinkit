#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: gc_root.h
// Description: Root Object Wrappers
//      Author: Ziming Li
//     Created: 2021-09-03
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_GC_ROOT_H
#define BLINKIT_BLINKIT_GC_ROOT_H

#include "blinkit/gc/gc_def.h"
#include "blinkit/gc/gc_visitor.h"

namespace BlinKit {

template <class T>
struct GCRootDeleter final : public std::default_delete<T>
{
    void operator()(T *p) const
    {
        {
            GCVisitor visitor;
            p->trace(&visitor);
        }
        std::default_delete<T>::operator()(p);
    }
};

template <class T>
using GCUniqueRoot = std::unique_ptr<T, GCRootDeleter<T>>;

template <class T>
GCUniqueRoot<T> WrapUniqueRoot(T *p)
{
    return GCUniqueRoot<T>(p);
}

} // namespace BlinKit

using BlinKit::GCUniqueRoot;

#endif // BLINKIT_BLINKIT_GC_ROOT_H
