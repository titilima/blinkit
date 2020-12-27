// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: weak_ptr.h
// Description: WeakPtr Stuff
//      Author: Ziming Li
//     Created: 2020-12-27
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BASE_WEAK_PTR_H
#define BLINKIT_BASE_WEAK_PTR_H

#pragma once

#include <memory>

namespace base {

template<typename T> class WeakPtrFactory;

template <typename T>
class WeakPtr
{
public:
    WeakPtr(void) = default;
    WeakPtr(WeakPtr &other) : m_slot(other.slot) {}

    operator bool() const { return m_slot && nullptr != *m_slot; }
    T* operator->() const { return *m_slot; }
    WeakPtr<T>& operator=(const WeakPtr<T> &other)
    {
        m_slot = other.m_slot;
        return *this;
    }

    T* get(void) const { return *m_slot; }
    void reset(void) { m_slot.reset(); }
private:
    friend class WeakPtrFactory<T>;
    WeakPtr(const std::shared_ptr<T *> &slot) : m_slot(slot) {}

    std::shared_ptr<T *> m_slot;
};

template <typename T>
class WeakPtrFactory
{
public:
    WeakPtrFactory(T *p) : m_slot(std::make_shared<T *>())
    {
        *m_slot = p;
    }
    ~WeakPtrFactory(void)
    {
        *m_slot = nullptr;
    }

    base::WeakPtr<T> GetWeakPtr(void) { return base::WeakPtr<T>(m_slot); }
private:
    const std::shared_ptr<T *> m_slot;
};

} // namespace base

#endif // BLINKIT_BASE_WEAK_PTR_H
