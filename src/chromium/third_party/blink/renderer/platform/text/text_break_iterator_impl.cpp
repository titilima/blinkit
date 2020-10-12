// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: text_break_iterator_impl.cpp
// Description: TextBreakIterator Implementations
//      Author: Ziming Li
//     Created: 2020-10-12
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "text_break_iterator.h"

namespace blink {

NonSharedCharacterBreakIterator::NonSharedCharacterBreakIterator(const StringView &string)
{
    if (string.IsEmpty())
        return;
    ASSERT(false); // BKTODO:
}

NonSharedCharacterBreakIterator::~NonSharedCharacterBreakIterator(void)
{
    ASSERT(false); // BKTODO:
}

int NonSharedCharacterBreakIterator::Following(int offset) const
{
    ASSERT(false); // BKTODO:
    return 0;
}

bool NonSharedCharacterBreakIterator::IsBreak(int offset) const
{
    ASSERT(false); // BKTODO:
    return false;
}

int NonSharedCharacterBreakIterator::Preceding(int offset) const
{
    ASSERT(false); // BKTODO:
    return 0;
}

} // namespace blink
