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
class Member
{
public:
    Member(T *ptr = nullptr) : m_rawPtr(ptr) {}
    Member(const Member &other) : m_rawPtr(other.Get()) {}

    T* Get(void) const { return m_rawPtr; }

    operator T*() const { return m_rawPtr; }
    T& operator*() const { return *m_rawPtr; }
    T* operator->() const { return m_rawPtr; }
    bool operator!() const { return !m_rawPtr; }
private:
    T *m_rawPtr;
};

} // namespace blink

#endif // BLINKIT_BLINK_MEMBER_H
