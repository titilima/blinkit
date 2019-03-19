// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: word_break_iterator.h
// Description: WordBreakIterator Class
//      Author: Ziming Li
//     Created: 2019-03-18
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_WORD_BREAK_ITERATOR_H
#define BLINKIT_BLINK_WORD_BREAK_ITERATOR_H

#pragma once

#include "break_iterator_base.h"

namespace blink {

class WordBreakIterator final : public BreakIteratorT<UChar>
{
private:
    static bool IsWordBreak(UChar c);

    // icu::BreakIterator
    int32_t previous(void) override;
    int32_t next(void) override;
};

} // namespace blink

#endif // BLINKIT_BLINK_WORD_BREAK_ITERATOR_H
