#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: word_break_iterator.h
// Description: WordBreakIterator Class
//      Author: Ziming Li
//     Created: 2022-02-24
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_WORD_BREAK_ITERATOR_H
#define BLINKIT_WORD_BREAK_ITERATOR_H

#include "blinkit/blink/renderer/platform/text/TextBreakIterator.h"

namespace BlinKit {

class WordBreakIterator final : public TextBreakIterator
{
private:
    static bool IsWordBreak(UChar c)
    {
        if (zed::isdigit(c) || zed::isalpha(c))
            return false;
        return false;
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
        if (0 == m_pos)
            return -1;

        if (m_pos < 0)
        {
            m_pos = 0;
            return m_pos;
        }

        const int length = m_string.length();
        if (m_pos > length)
        {
            m_pos = length;
            return m_pos;
        }

        UChar c;
        if (m_pos == length)
        {
            m_pos = length - 1;
            c = m_string.at(m_pos);
            if (IsWordBreak(c))
                return m_pos;
        }

        if (0 == m_pos)
            return 0;

        c = m_string.at(m_pos);
        UChar prev = m_string.at(m_pos - 1);
        if (IsWordBreak(prev))
        {
            --m_pos;
            return m_pos;
        }

        if (!IsWordBreak(prev) && IsWordBreak(c))
        {
            --m_pos;
            c = m_string.at(m_pos);
        }

        while (m_pos > 0)
        {
            c = m_string.at(m_pos);

            if (IsWordBreak(c))
                return m_pos + 1;
            --m_pos;
            continue;
        }

        return m_pos;
    }
    int32_t next(void) override
    {
        const int length = m_string.length();
        if (m_pos >= length)
        {
            m_pos = length;
            return -1;
        }
        if (m_pos < 0)
        {
            m_pos = 0;
            return m_pos;
        }

        bool haveSpace = false;

        UChar c = m_string.at(m_pos);
        if (IsWordBreak(c))
        {
            ++m_pos;
            return m_pos;
        }

        while (m_pos < length)
        {
            c = m_string.at(m_pos);

            if (!IsWordBreak(c))
            {
                ++m_pos;
                continue;
            }

            break;
        }

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

#endif // BLINKIT_WORD_BREAK_ITERATOR_H
