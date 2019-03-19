// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: break_iterator_base.h
// Description: Base Classes for Break Iterators
//      Author: Ziming Li
//     Created: 2019-03-19
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_BREAK_ITERATOR_BASE_H
#define BLINKIT_BLINK_BREAK_ITERATOR_BASE_H

#pragma once

#include <string_view>
#include <unicode/brkiter.h>

namespace blink {

template <typename CharT>
class BreakIteratorT : public icu::BreakIterator
{
public:
    void SetText(const CharT *text, size_t length) {
        m_text = std::basic_string_view<CharT>(text, length);
        m_offset = 0;
    }
protected:
    CharT CurrentChar(void) const { return m_text.at(m_offset); }

    std::basic_string_view<CharT> m_text;
    int32_t m_offset = 0;
private:
    // icu::BreakIterator
    int32_t first(void) override {
        m_offset = 0;
        return m_offset;
    }
    int32_t last(void) override {
        m_offset = m_text.length();
        return m_offset;
    }
    int32_t previous(void) override {
        assert(false); // Not reached!
        return 0;
    }
    int32_t next(void) override {
        assert(false); // Not reached!
        return 0;
    }
    int32_t current(void) const override {
        return m_offset;
    }
    int32_t following(int32_t offset) override {
        m_offset = offset;
        return next();
    }
    int32_t preceding(int32_t offset) override {
        m_offset = offset;
        return previous();
    }
    bool isBoundary(int32_t offset) override {
        assert(false); // Not reached!
        return false;
    }
};

} // namespace blink

#endif // BLINKIT_BLINK_BREAK_ITERATOR_BASE_H
