#pragma once
// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: ExceptionStatePlaceholder.h
// Description: ExceptionState Placeholders
//      Author: Ziming Li
//     Created: 2021-07-05
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_EXCEPTION_STATE_PLACEHOLDERS_H
#define BLINKIT_BLINKIT_EXCEPTION_STATE_PLACEHOLDERS_H

#include "blinkit/blink/renderer/bindings/core/duk/exception_state.h"

namespace blink {

class NonThrowableExceptionState final : public ExceptionState
{
public:
    NonThrowableExceptionState(const char *fileName, int line);

    ExceptionState& ReturnThis(void) { return *this; }
private:
    const char *m_fileName;
    const int m_line;
};

class DummyExceptionStateForTesting final : public ExceptionState
{
public:
    DummyExceptionStateForTesting(void);

    ExceptionState& ReturnThis(void) { return *this; }
};

#if 0 // BKTODO: Check if necessary.
class TrackExceptionState final : public ExceptionState
{
public:
    TrackExceptionState(void) = default;
};
#endif

} // namespace blink

#ifdef NDEBUG
#   define ASSERT_NO_EXCEPTION  (::blink::DummyExceptionStateForTesting().ReturnThis())
#else
#   define ASSERT_NO_EXCEPTION  (::blink::NonThrowableExceptionState(__FILE__, __LINE__).ReturnThis())
#endif

#define IGNORE_EXCEPTION                (::blink::DummyExceptionStateForTesting().ReturnThis())
#define IGNORE_EXCEPTION_FOR_TESTING    (::blink::DummyExceptionStateForTesting().ReturnThis())

#endif // BLINKIT_BLINKIT_EXCEPTION_STATE_PLACEHOLDERS_H
