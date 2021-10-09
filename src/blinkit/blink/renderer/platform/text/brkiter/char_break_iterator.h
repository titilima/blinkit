#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: char_break_iterator.h
// Description: CharBreakIterator Class
//      Author: Ziming Li
//     Created: 2021-10-08
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_CHAR_BREAK_ITERATOR_H
#define BLINKIT_BLINKIT_CHAR_BREAK_ITERATOR_H

#include "blinkit/blink/renderer/platform/text/TextBreakIterator.h"

namespace BlinKit {

class CharBreakIterator final : public blink::TextBreakIterator
{
private:
    int32_t first(void) override
    {
        ASSERT(false); // BKTODO:
        return 0;
    }
    int32_t last(void) override
    {
        ASSERT(false); // BKTODO:
        return 0;
    }
    int32_t previous(void) override
    {
        if (m_pos <= 0)
            return -1;
        if (m_pos > static_cast<int32_t>(m_string.length()))
            m_pos = m_string.length() - 1;
        return --m_pos;
    }
    int32_t next(void) override
    {
        if (m_pos >= static_cast<int32_t>(m_string.length()))
            return m_string.length() - 1;
        return ++m_pos;
    }
    int32_t current(void) const override
    {
        ASSERT(false); // BKTODO:
        return 0;
    }
    UBool isBoundary(int32_t offset) override
    {
        ASSERT(false); // BKTODO:
        return FALSE;
    }
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CHAR_BREAK_ITERATOR_H
