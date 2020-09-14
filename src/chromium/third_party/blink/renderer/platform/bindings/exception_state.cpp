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

#include "base/logging.h"

namespace blink {

void ExceptionState::ClearException(void)
{
    assert(false); // BKTODO:
}

void ExceptionState::ThrowDOMException(DOMExceptionCode code, const String &message)
{
    ASSERT(DOMExceptionCode::kSecurityError != code);
    m_exceptionCode = ToExceptionCode(code);
    m_message = message.StdUtf8();
    BKLOG("DOM Exception (%d): %s", m_exceptionCode, m_message.c_str());
}

void ExceptionState::ThrowException(void)
{
    NOTREACHED();
}

void ExceptionState::ThrowRangeError(const String &message)
{
    ASSERT(false); // BKTODO:
}

void ExceptionState::ThrowSecurityError(const String &sanitizedMessage, const String &unsanitizedMessage)
{
    ASSERT(false); // BKTODO:
}

void ExceptionState::ThrowTypeError(const String &message)
{
    ASSERT(false); // BKTODO:
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
