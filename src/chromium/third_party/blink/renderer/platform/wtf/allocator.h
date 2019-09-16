// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: allocator.h
// Description: Placeholders for WTF Part
//      Author: Ziming Li
//     Created: 2019-09-16
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_ALLOCATOR_H
#define BLINKIT_BLINK_ALLOCATOR_H

#pragma once

#define DISALLOW_NEW()                                          \
    private:                                                    \
        void* operator new(size_t) = delete;                    \
        void* operator new(size_t, void*) = delete;             \
    public:

#endif // BLINKIT_BLINK_ALLOCATOR_H
