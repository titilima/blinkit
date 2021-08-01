// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_timer.h
// Description: DukTimer Class
//      Author: Ziming Li
//     Created: 2020-07-05
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DUK_TIMER_H
#define BLINKIT_BLINK_DUK_TIMER_H

#pragma once

#include <string>
#include "blinkit/js/heap_retained.h"

namespace BlinKit {

class DukTimer : public HeapRetainedValue
{
public:
    DukTimer(duk_context *ctx, duk_idx_t idx, int argc);

    bool IsRepeatable(void) const { return m_repeatable; }
    void SetIsRepeatable(void) { m_repeatable = true; }

    unsigned Interval(void) const { return m_intervalInMs; }
    void SetInterval(unsigned intervalInMs) { m_intervalInMs = intervalInMs; }

    bool Fire(void);
private:
    static void DoEval(duk_context *ctx);
    static void DoCall(duk_context *ctx, duk_idx_t idx);

    bool m_repeatable = false;
    unsigned m_intervalInMs = 0;
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_TIMER_H
