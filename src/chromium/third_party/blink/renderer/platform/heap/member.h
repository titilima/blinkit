// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: member.h
// Description: Placeholders for Heap Part
//      Author: Ziming Li
//     Created: 2019-09-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_MEMBER_H
#define BLINKIT_BLINK_MEMBER_H

#pragma once

#include "third_party/blink/renderer/platform/heap/heap.h"
#include "third_party/blink/renderer/platform/wtf/allocator.h"

namespace blink {

template <typename T>
class WeakMember;

template<typename T>
class MemberBase
{
public:
    T* Get(void) const { return m_rawPtr; }
    T* Release(void)
    {
        T* result = m_rawPtr;
        m_rawPtr = nullptr;
        return result;
    }
    void Clear(void) { m_rawPtr = nullptr; }

    operator T*() const { return m_rawPtr; }
    T& operator*() const { return *m_rawPtr; }
    T* operator->() const { return m_rawPtr; }
    bool operator!() const { return !m_rawPtr; }

    template <typename U>
    MemberBase& operator=(const MemberBase<U> &other)
    {
        m_rawPtr = other;
        return *this;
    }

    void Swap(MemberBase<T> &other)
    {
        std::swap(m_rawPtr, other.m_rawPtr);
    }
protected:
    MemberBase(T *ptr) : m_rawPtr(ptr) {}
    MemberBase(const MemberBase &other) : m_rawPtr(other.Get()) {}
    MemberBase(T &rawObj) : m_rawPtr(&rawObj) {}
private:
    friend class Visitor;
    T *m_rawPtr;
};

template<typename T>
class Member : public MemberBase<T>
{
public:
    Member(T *ptr = nullptr) : MemberBase<T>(ptr) {}
    Member(const Member &other) : MemberBase<T>(other) {}
    Member(T &rawObj) : MemberBase<T>(rawObj) {}

    template <typename U>
    Member& operator=(const Member<U> &other)
    {
        MemberBase<T>::operator=(other);
        return *this;
    }
    template <typename U>
    Member& operator=(const WeakMember<U> &other)
    {
        MemberBase<T>::operator=(other);
        return *this;
    }
};

template<typename T>
class WeakMember : public MemberBase<T>
{
public:
    WeakMember(T *ptr = nullptr) : MemberBase<T>(ptr) {}
    WeakMember(const WeakMember &other) : MemberBase<T>(other) {}
    WeakMember(T &rawObj) : MemberBase<T>(rawObj) {}
};

} // namespace blink

namespace std {

template <typename U>
struct hash<blink::Member<U>> {
  std::size_t operator()(const blink::Member<U> &m) const noexcept {
    return reinterpret_cast<std::size_t>(m.Get());
  }
};

template <typename U>
struct hash<blink::WeakMember<U>> {
  std::size_t operator()(const blink::WeakMember<U> &m) const noexcept {
    return reinterpret_cast<std::size_t>(m.Get());
  }
};

}

#endif // BLINKIT_BLINK_MEMBER_H
