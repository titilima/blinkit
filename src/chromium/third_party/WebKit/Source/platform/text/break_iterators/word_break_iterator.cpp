// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: word_break_iterator.cpp
// Description: WordBreakIterator Class
//      Author: Ziming Li
//     Created: 2019-03-18
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "word_break_iterator.h"

#include <cctype>

namespace blink {

bool WordBreakIterator::IsWordBreak(UChar c)
{
    return !isalnum(c);
}

int32_t WordBreakIterator::next(void)
{
    if (m_offset >= static_cast<int32_t>(m_text.length()))
    {
        m_offset = m_text.length();
        return DONE;
    }

    if (m_offset < 0)
    {
        m_offset = 0;
        return m_offset;
    }

    if (IsWordBreak(CurrentChar()))
        return ++m_offset;

    while (m_offset < static_cast<int32_t>(m_text.length()))
    {
        if (IsWordBreak(CurrentChar()))
            break;
        ++m_offset;
    }
    return m_offset;
}

int32_t WordBreakIterator::previous(void)
{
    if (0 == m_offset)
        return DONE;

    if (m_offset < 0)
    {
        m_offset = 0;
        return m_offset;
    }

    if (m_offset > static_cast<int32_t>(m_text.length()))
    {
        m_offset = m_text.length();
        return m_offset;
    }

    if (m_offset == m_text.length())
    {
        m_offset = m_text.length() - 1;
        if (IsWordBreak(CurrentChar()))
            return m_offset;
    }

    if (0 == m_offset)
        return 0;

    UChar prevChar = m_text.at(m_offset - 1);
    if (IsWordBreak(prevChar))
    {
        --m_offset;
        return m_offset;
    }

    if (!IsWordBreak(CurrentChar()))
        --m_offset;

    while (m_offset > 0)
    {
        if (IsWordBreak(CurrentChar()))
            return m_offset + 1;

        --m_offset;
    }

    return m_offset;
}

} // namespace blink
