// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: text_break_iterator_impl.cpp
// Description: TextBreakIterator Classes
//      Author: Ziming Li
//     Created: 2021-09-24
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./TextBreakIterator.h"

#include <queue>
#include "blinkit/blink/renderer/platform/text/brkiter/char_break_iterator.h"
#include "blinkit/blink/renderer/platform/text/brkiter/line_break_iterator.h"

using namespace BlinKit;

namespace blink {

static std::queue<std::unique_ptr<TextBreakIterator>> g_lineBreakIterators;

static TextBreakIterator* AcquireLineBreakIterator(void)
{
    if (g_lineBreakIterators.empty())
        return new LineBreakIterator;

    TextBreakIterator *ret = g_lineBreakIterators.front().release();
    g_lineBreakIterators.pop();
    return ret;
}

TextBreakIterator* acquireLineBreakIterator(const LChar *string, int length, const AtomicString &locale, const UChar *priorContext, unsigned priorContextLength)
{
    ASSERT(false); // BKTODO:
    return nullptr;
}

TextBreakIterator* acquireLineBreakIterator(const UChar *string, int length, const AtomicString &locale, const UChar *priorContext, unsigned priorContextLength)
{
    TextBreakIterator *ret = AcquireLineBreakIterator();
    ret->Attach(string, length);
    return ret;
}

TextBreakIterator* cursorMovementIterator(const UChar *string, int length)
{
    static CharBreakIterator s_charBreakIterator;
    s_charBreakIterator.Attach(string, length);
    return &s_charBreakIterator;
}

bool isWordTextBreak(TextBreakIterator* iterator)
{
    ASSERT(false); // BKTODO:
    return false;
}

void releaseLineBreakIterator(TextBreakIterator *iterator)
{
    g_lineBreakIterators.emplace(iterator);
}

TextBreakIterator* sentenceBreakIterator(const UChar *string, int length)
{
    ASSERT(false); // BKTODO:
    return nullptr;
}

TextBreakIterator* wordBreakIterator(const UChar *string, int length)
{
    ASSERT(false); // BKTODO:
    return nullptr;
}

TextBreakIterator* wordBreakIterator(const String &string, int start, int length)
{
    if (string.isEmpty())
        return nullptr;
    ASSERT(false); // BKTODO:
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

NonSharedCharacterBreakIterator::NonSharedCharacterBreakIterator(const String &string)
{
    ASSERT(false); // BKTODO:
}

NonSharedCharacterBreakIterator::NonSharedCharacterBreakIterator(const UChar *buffer, unsigned length)
{
    ASSERT(false); // BKTODO:
}

NonSharedCharacterBreakIterator::~NonSharedCharacterBreakIterator(void)
{
    ASSERT(false); // BKTODO:
}

int NonSharedCharacterBreakIterator::following(int offset) const
{
    ASSERT(false); // BKTODO:
    return 0;
}

bool NonSharedCharacterBreakIterator::isBreak(int offset) const
{
    ASSERT(false); // BKTODO:
    return false;
}

int NonSharedCharacterBreakIterator::next(void)
{
    ASSERT(false); // BKTODO:
    return 0;
}

int NonSharedCharacterBreakIterator::preceding(int offset) const
{
    ASSERT(false); // BKTODO:
    return 0;
}

} // namespace blink
