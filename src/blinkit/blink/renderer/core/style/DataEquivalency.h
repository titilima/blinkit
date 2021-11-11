// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: DataEquivalency.h
// Description: dataEquivalent Implementations
//      Author: Ziming Li
//     Created: 2021-11-11
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DataEquivalency_h
#define DataEquivalency_h

#include "wtf/OwnPtr.h"
#include "wtf/RefPtr.h"

namespace blink {

template <typename T>
bool dataEquivalent(const T* a, const T* b)
{
    if (a == b)
        return true;
    if (!a || !b)
        return false;
    return *a == *b;
}

template <typename T>
bool dataEquivalent(const GCRefPtr<T> &a, const GCRefPtr<T> &b)
{
    return dataEquivalent(a.get(), b.get());
}

template <typename T>
bool dataEquivalent(const RefPtr<T>& a, const RefPtr<T>& b)
{
    return dataEquivalent(a.get(), b.get());
}

template <typename T>
bool dataEquivalent(const Persistent<T>& a, const Persistent<T>& b)
{
    return dataEquivalent(a.get(), b.get());
}

template <typename T>
bool dataEquivalent(const Member<T>& a, const Member<T>& b)
{
    return dataEquivalent(a.get(), b.get());
}

template <typename T>
bool dataEquivalent(const OwnPtr<T>& a, const OwnPtr<T>& b)
{
    return dataEquivalent(a.get(), b.get());
}

} // namespace blink

#endif // DataEquivalency_h
