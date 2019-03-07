// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: exception_state.cpp
// Description: ExceptionState & Related Classes
//      Author: Ziming Li
//     Created: 2019-03-07
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "exception_state.h"

namespace blink {

void ExceptionState::throwDOMException(ExceptionCode exceptionCode, const String &message)
{
    assert(false); // BKTODO:
}

void ExceptionState::throwException(void)
{
    assert(false); // BKTODO:
}

void ExceptionState::throwSecurityError(const String &sanitizedMessage, const String &unsanitizedMessage)
{
    assert(false); // BKTODO:
}

void ExceptionState::throwTypeError(const String &message)
{
    assert(false); // BKTODO:
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

NonThrowableExceptionState::NonThrowableExceptionState(const char *fileName, int line)
    : m_fileName(fileName), m_line(line)
{
    // Nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DummyExceptionStateForTesting::DummyExceptionStateForTesting(void)
{
    // Nothing
}

} // namespace blink
