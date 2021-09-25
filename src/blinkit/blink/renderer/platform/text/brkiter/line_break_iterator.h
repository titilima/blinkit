#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: line_break_iterator.h
// Description: LineBreakIterator Class
//      Author: Ziming Li
//     Created: 2021-09-24
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_LINE_BREAK_ITERATOR_H
#define BLINKIT_BLINKIT_LINE_BREAK_ITERATOR_H

#include "blinkit/blink/renderer/platform/text/TextBreakIterator.h"
#include "third_party/zed/include/zed/ctype.hpp"

namespace BlinKit {

class LineBreakIterator final : public blink::TextBreakIterator
{
private:
    static bool IsLineStop(UChar c)
    {
        if (zed::isdigit(c) || zed::isalpha(c))
            return false;
        return true;
    }

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
        ASSERT(false); // BKTODO:
        return 0;
    }
    int32_t next(void) override
    {
        const int32_t length = static_cast<int32_t>(m_string.length());
        if (m_pos >= length)
            return -1;

        ++m_pos;
        while (m_pos < length && !IsLineStop(m_string.at(m_pos)))
            ++m_pos;
        return m_pos;
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

#endif // BLINKIT_BLINKIT_LINE_BREAK_ITERATOR_H
