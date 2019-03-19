// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: character_break_iterator.h
// Description: CharacterBreakIterator Class
//      Author: Ziming Li
//     Created: 2019-03-19
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_CHARACTER_BREAK_ITERATOR_H
#define BLINKIT_BLINK_CHARACTER_BREAK_ITERATOR_H

#pragma once

#include "break_iterator_base.h"

namespace blink {

class CharacterBreakIterator final : public BreakIteratorT<UChar>
{
private:
    int32_t previous(void) override;
    int32_t next(void) override;
};

} // namespace blink

#endif // BLINKIT_BLINK_CHARACTER_BREAK_ITERATOR_H
