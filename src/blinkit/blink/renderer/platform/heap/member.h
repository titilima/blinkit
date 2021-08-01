// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: member.h
// Description: Placeholders for Heap Part
//      Author: Ziming Li
//     Created: 2019-09-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_MEMBER_H
#define BLINKIT_BLINKIT_MEMBER_H

#pragma once

#include "blinkit/blink/renderer/wtf/Assertions.h"
#include "blinkit/blink/renderer/wtf/RawPtr.h"

namespace blink {

template <typename T>
class WeakMember;

template <typename T>
class MemberBase
{
public:
    T* get(void) const { return m_rawPtr; }
    T* release(void)
    {
        T* result = m_rawPtr;
        m_rawPtr = nullptr;
        return result;
    }
    void clear(void) { m_rawPtr = nullptr; }

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
    template <typename U>
    MemberBase& operator=(const RawPtr<U> &other)
    {
        m_rawPtr = other;
        return *this;
    }

    void swap(MemberBase<T> &other)
    {
        std::swap(m_rawPtr, other.m_rawPtr);
    }
protected:
    MemberBase(T *ptr) : m_rawPtr(ptr) {}
    MemberBase(const MemberBase &other) : m_rawPtr(other.get()) {}
    MemberBase(T &rawObj) : m_rawPtr(&rawObj) {}
private:
    friend class Visitor;
    T *m_rawPtr;
};

template <typename T>
class Member : public MemberBase<T>
{
public:
    Member(T *ptr = nullptr) : MemberBase<T>(ptr) {}
    Member(const Member &other) : MemberBase<T>(other) {}
    Member(T &rawObj) : MemberBase<T>(rawObj) {}
    template <typename U>
    Member(const RawPtr<U> &other) : MemberBase<T>(other.get()) {}

    template<typename U>
    operator RawPtr<U>() const { return MemberBase<T>::get(); }

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
    template <typename U>
    Member& operator=(const RawPtr<U> &other)
    {
        MemberBase<T>::operator=(other);
        return *this;
    }
};

template <typename T>
class WeakMember : public MemberBase<T>
{
public:
    WeakMember(T *ptr = nullptr) : MemberBase<T>(ptr) {}
    WeakMember(const WeakMember &other) : MemberBase<T>(other) {}
    WeakMember(T &rawObj) : MemberBase<T>(rawObj) {}

    template <typename U>
    WeakMember& operator=(const RawPtr<U> &other)
    {
        MemberBase<T>::operator=(other);
        return *this;
    }
};

template <typename T>
class UntracedMember : public MemberBase<T>
{
public:
    UntracedMember(T *ptr = nullptr) : MemberBase<T>(ptr) {}
    UntracedMember(const UntracedMember &other) : MemberBase<T>(other) {}
    UntracedMember(T &rawObj) : MemberBase<T>(rawObj) {}

    template <typename U>
    UntracedMember& operator=(const WeakMember<U> &other)
    {
        MemberBase<T>::operator=(other);
        return *this;
    }
};

} // namespace blink

namespace std {

template <typename U>
struct hash<blink::Member<U>> {
    std::size_t operator()(const blink::Member<U> &m) const noexcept {
        return reinterpret_cast<std::size_t>(m.get());
    }
};

template <typename U>
struct hash<blink::WeakMember<U>> {
    std::size_t operator()(const blink::WeakMember<U> &m) const noexcept {
        return reinterpret_cast<std::size_t>(m.get());
    }
};

}

#endif // BLINKIT_BLINKIT_MEMBER_H
