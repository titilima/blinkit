// -------------------------------------------------
// BlinKit - stub Part
// -------------------------------------------------
//   File Name: brkiter.h
// Description: ICU Stub
//      Author: Ziming Li
//     Created: 2019-03-04
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_STUB_ICU_BRKITER_H
#define BLINKIT_STUB_ICU_BRKITER_H

#pragma once

#include "utypes.h"

namespace icu {

class BreakIterator {
public:
    virtual ~BreakIterator(void) = default;

    enum {
        DONE = (int32_t)-1
    };

    virtual int32_t first(void) = 0;
    virtual int32_t last(void) = 0;
    virtual int32_t previous(void) = 0;
    virtual int32_t next(void) = 0;
    virtual int32_t current(void) const = 0;
    virtual int32_t following(int32_t offset) = 0;
    virtual int32_t preceding(int32_t offset) = 0;
    virtual bool isBoundary(int32_t offset) = 0;
};

} // namespace icu

#endif // BLINKIT_STUB_ICU_BRKITER_H
