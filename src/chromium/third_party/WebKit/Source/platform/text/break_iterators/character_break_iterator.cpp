// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: character_break_iterator.cpp
// Description: CharacterBreakIterator Class
//      Author: Ziming Li
//     Created: 2019-03-19
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "character_break_iterator.h"

namespace blink {

int32_t CharacterBreakIterator::next(void)
{
    if (m_offset >= static_cast<int32_t>(m_text.length()))
        return m_text.length() - 1;
    else
        return ++m_offset;
}

int32_t CharacterBreakIterator::previous(void)
{
    if (m_offset <= 0)
        return -1;
    if (m_offset > static_cast<int32_t>(m_text.length()))
        m_offset = m_text.length() - 1;
    return --m_offset;
}

} // namespace blink
