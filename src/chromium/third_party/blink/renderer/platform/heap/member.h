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

namespace blink {

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

    operator T*() const { return m_rawPtr; }
    T& operator*() const { return *m_rawPtr; }
    T* operator->() const { return m_rawPtr; }
    bool operator!() const { return !m_rawPtr; }

    void Swap(MemberBase<T> &other)
    {
        std::swap(m_rawPtr, other.m_rawPtr);
    }
protected:
    MemberBase(T *ptr) : m_rawPtr(ptr) {}
    MemberBase(const MemberBase &other) : m_rawPtr(other.Get()) {}
    MemberBase(T &rawObj) : m_rawPtr(&rawObj) {}
private:
    T *m_rawPtr;
};

template<typename T>
class Member : public MemberBase<T>
{
public:
    Member(T *ptr = nullptr) : MemberBase<T>(ptr) {}
    Member(const Member &other) : MemberBase<T>(other) {}
    explicit Member(T &rawObj) : MemberBase<T>(rawObj) {}
};

template<typename T>
class WeakMember : public MemberBase<T>
{
public:
    WeakMember(T *ptr = nullptr) : MemberBase<T>(ptr) {}
};

} // namespace blink

#endif // BLINKIT_BLINK_MEMBER_H
