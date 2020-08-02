// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: persistent.h
// Description: Placeholders for Heap Part
//      Author: Ziming Li
//     Created: 2020-08-02
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_PERSISTENT_H
#define BLINKIT_BLINK_PERSISTENT_H

#pragma once

namespace blink {

template <class T>
class Persistent
{
public:
    Persistent(void)
    {
        ASSERT(false); // BKTODO:
    }
    T* Get(void) const
    {
        ASSERT(false); // BKTODO:
        return nullptr;
    }
};

} // BLINKIT_BLINK_PERSISTENT_H

#endif // BLINKIT_BLINK_PERSISTENT_H
