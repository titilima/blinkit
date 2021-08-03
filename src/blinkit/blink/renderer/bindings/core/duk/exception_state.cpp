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

#include "./exception_state.h"

// BKTODO: #include "base/logging.h"

namespace blink {

#if 0 // BKTODO:
void ExceptionState::ClearException(void)
{
    ASSERT(false); // BKTODO:
}
#endif

void ExceptionState::throwDOMException(ExceptionCode code, const String &message)
{
    ASSERT(false); // BKTODO:
#if 0
    m_exceptionCode = exceptionCode;
    m_message = message;
    BKLOG("DOM Exception (%d): %s", m_exceptionCode, m_message.to_string().c_str());
#endif
}

void ExceptionState::ThrowException(void)
{
    NOTREACHED();
}

#if 0 // BKTODO:
void ExceptionState::ThrowRangeError(const String &message)
{
    ASSERT(false); // BKTODO:
}

void ExceptionState::ThrowSecurityError(const String &sanitizedMessage, const String &unsanitizedMessage)
{
    ASSERT(false); // BKTODO:
}
#endif

void ExceptionState::throwTypeError(const String &message)
{
    ASSERT(false); // BKTODO:
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

NonThrowableExceptionState::NonThrowableExceptionState(const char *fileName, int line)
    : m_fileName(fileName), m_line(line)
{
    // Nothing
}

} // namespace blink
