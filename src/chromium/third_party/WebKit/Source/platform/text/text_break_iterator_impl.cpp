// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: text_break_iterator_impl.cpp
// Description: Text Break Iterators
//      Author: Ziming Li
//     Created: 2019-03-09
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "TextBreakIterator.h"

#include "platform/text/break_iterators/character_break_iterator.h"
#include "platform/text/break_iterators/word_break_iterator.h"
#include "wtf/ThreadingPrimitives.h"

using namespace WTF;

namespace blink {

static TextBreakIterator* characterBreakIterator(const UChar *string, int length)
{
    static CharacterBreakIterator s_characterBreakIterator;
    s_characterBreakIterator.SetText(string, length);
    return &s_characterBreakIterator;
}

static TextBreakIterator* wordBreakIterator(const LChar *string, int length)
{
    assert(false); // BKTODO:
    return nullptr;
}

TextBreakIterator* wordBreakIterator(const UChar *string, int length)
{
    static WordBreakIterator s_wordBreakIterator;
    s_wordBreakIterator.SetText(string, length);
    return &s_wordBreakIterator;
}

TextBreakIterator* wordBreakIterator(const String &string, int start, int length)
{
    if (string.isEmpty())
        return nullptr;
    if (string.is8Bit())
        return wordBreakIterator(string.characters8() + start, length);
    return wordBreakIterator(string.characters16() + start, length);
}

TextBreakIterator* acquireLineBreakIterator(const LChar* string, int length, const AtomicString& locale, const UChar* priorContext, unsigned priorContextLength)
{
    assert(false); // BKTODO:
    return nullptr;
}

TextBreakIterator* acquireLineBreakIterator(const UChar* string, int length, const AtomicString& locale, const UChar* priorContext, unsigned priorContextLength)
{
    assert(false); // BKTODO:
    return nullptr;
}

void releaseLineBreakIterator(TextBreakIterator *iterator)
{
    assert(false); // BKTODO:
}

static TextBreakIterator* nonSharedCharacterBreakIterator = nullptr;

static inline bool compareAndSwapNonSharedCharacterBreakIterator(TextBreakIterator *expected, TextBreakIterator *newValue)
{
    DEFINE_STATIC_LOCAL(Mutex, nonSharedCharacterBreakIteratorMutex, ());
    MutexLocker locker(nonSharedCharacterBreakIteratorMutex);
    if (nonSharedCharacterBreakIterator != expected)
        return false;
    nonSharedCharacterBreakIterator = newValue;
    return true;
}

NonSharedCharacterBreakIterator::NonSharedCharacterBreakIterator(const String &string)
    : m_is8Bit(true)
    , m_charaters8(0)
    , m_offset(0)
    , m_length(0)
    , m_iterator(0)
{
    if (string.isEmpty())
        return;

    m_is8Bit = string.is8Bit();

    if (m_is8Bit) {
        m_charaters8 = string.characters8();
        m_offset = 0;
        m_length = string.length();
        return;
    }

    createIteratorForBuffer(string.characters16(), string.length());
}

NonSharedCharacterBreakIterator::NonSharedCharacterBreakIterator(const UChar *buffer, unsigned length)
    : m_is8Bit(false)
    , m_charaters8(0)
    , m_offset(0)
    , m_length(0)
    , m_iterator(0)
{
    createIteratorForBuffer(buffer, length);
}

void NonSharedCharacterBreakIterator::createIteratorForBuffer(const UChar *buffer, unsigned length)
{
    m_iterator = nonSharedCharacterBreakIterator;
    bool createdIterator = m_iterator && compareAndSwapNonSharedCharacterBreakIterator(m_iterator, 0);
    assert(false); // BKTODO:
#if 0
    if (!createdIterator) {
        UErrorCode errorCode = U_ZERO_ERROR;
        m_iterator = icu::BreakIterator::createCharacterInstance(icu::Locale(currentTextBreakLocaleID()), errorCode);
        ASSERT_WITH_MESSAGE(U_SUCCESS(errorCode), "ICU could not open a break iterator: %s (%d)", u_errorName(errorCode), errorCode);
    }

    setText16(m_iterator, buffer, length);
#endif
}

NonSharedCharacterBreakIterator::~NonSharedCharacterBreakIterator(void)
{
    if (m_is8Bit)
        return;
    if (!compareAndSwapNonSharedCharacterBreakIterator(0, m_iterator))
        delete m_iterator;
}

int NonSharedCharacterBreakIterator::next(void)
{
    if (!m_is8Bit)
        return m_iterator->next();

    if (m_offset >= m_length)
        return TextBreakDone;

    m_offset += clusterLengthStartingAt(m_offset);
    return m_offset;
}

int NonSharedCharacterBreakIterator::current()
{
    if (!m_is8Bit)
        return m_iterator->current();
    return m_offset;
}

bool NonSharedCharacterBreakIterator::isBreak(int offset) const
{
    if (!m_is8Bit)
        return m_iterator->isBoundary(offset);
    return !isLFAfterCR(offset);
}

int NonSharedCharacterBreakIterator::preceding(int offset) const
{
    if (!m_is8Bit)
        return m_iterator->preceding(offset);
    if (offset <= 0)
        return TextBreakDone;
    if (isLFAfterCR(offset))
        return offset - 2;
    return offset - 1;
}

int NonSharedCharacterBreakIterator::following(int offset) const
{
    if (!m_is8Bit)
        return m_iterator->following(offset);
    if (static_cast<unsigned>(offset) >= m_length)
        return TextBreakDone;
    return offset + clusterLengthStartingAt(offset);
}

TextBreakIterator* sentenceBreakIterator(const UChar *string, int length)
{
    assert(false); // BKTODO:
    return nullptr;
}

bool isWordTextBreak(TextBreakIterator *iterator)
{
    assert(false); // BKTODO:
    return false;
}

TextBreakIterator* cursorMovementIterator(const UChar *string, int length)
{
    return characterBreakIterator(string, length);
}

} // namespace blink
