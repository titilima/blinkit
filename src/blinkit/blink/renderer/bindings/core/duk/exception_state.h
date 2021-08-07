#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: exception_state.h
// Description: ExceptionState & Related Classes
//      Author: Ziming Li
//     Created: 2019-01-21
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_EXCEPTION_STATE_H
#define BLINKIT_BLINKIT_EXCEPTION_STATE_H

#include "blinkit/blink/renderer/core/dom/ExceptionCode.h"
#include "blinkit/blink/renderer/wtf/Allocator.h"
#include "blinkit/blink/renderer/wtf/Noncopyable.h"
#include "blinkit/blink/renderer/wtf/text/WTFString.h"

namespace blink {

class ExceptionState
{
    WTF_MAKE_NONCOPYABLE(ExceptionState);
public:
    ExceptionCode Code(void) const { return m_exceptionCode; }
    bool hadException(void) const { return 0 != m_exceptionCode; }

    virtual void throwDOMException(ExceptionCode code, const String &message);
    // BKTODO: virtual void ThrowRangeError(const String &message);
    virtual void throwTypeError(const String &message);
    // BKTODO: virtual void ThrowSecurityError(const String &sanitizedMessage, const String &unsanitizedMessage = String());

    void clearException(void);

    bool ThrowIfNeeded(void)
    {
        if (!hadException())
            return false;
        ThrowException();
        return true;
    }
protected:
    ExceptionState(void) = default;

    ExceptionCode m_exceptionCode = 0;
    std::string m_message;
private:
    virtual void ThrowException(void);
};

class NonThrowableExceptionState final : public ExceptionState
{
public:
    NonThrowableExceptionState(const char* fileName, int line);

    ExceptionState& ReturnThis(void) { return *this; }
private:
    const char* m_fileName;
    const int m_line;
};

class TrackExceptionState final : public ExceptionState
{
public:
    TrackExceptionState(void) = default;
};

} // namespace blink

#endif // BLINKIT_BLINKIT_EXCEPTION_STATE_H
