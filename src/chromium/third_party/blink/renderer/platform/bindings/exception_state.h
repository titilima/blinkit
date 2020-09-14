// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: exception_state.h
// Description: ExceptionState & Related Classes
//      Author: Ziming Li
//     Created: 2019-01-21
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_EXCEPTION_STATE_H
#define BLINKIT_BLINK_EXCEPTION_STATE_H

#pragma once

#include "third_party/blink/renderer/platform/bindings/exception_code.h"
#include "third_party/blink/renderer/platform/wtf/allocator.h"
#include "third_party/blink/renderer/platform/wtf/noncopyable.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

namespace blink {

class ExceptionState
{
    STACK_ALLOCATED();
    WTF_MAKE_NONCOPYABLE(ExceptionState);
public:
    ExceptionCode Code(void) const { return m_exceptionCode; }
    bool HadException(void) const { return 0 != m_exceptionCode; }

    virtual void ThrowDOMException(DOMExceptionCode code, const String &message);
    virtual void ThrowRangeError(const String &message);
    virtual void ThrowTypeError(const String &message);
    virtual void ThrowSecurityError(const String &sanitizedMessage, const String &unsanitizedMessage = String());

    void ClearException(void);

    bool ThrowIfNeeded(void)
    {
        if (!HadException())
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

class TrackExceptionState final : public ExceptionState
{
public:
    TrackExceptionState(void) = default;
};

} // namespace blink

#ifdef NDEBUG
#   define ASSERT_NO_EXCEPTION  (::blink::DummyExceptionStateForTesting().ReturnThis())
#else
#   define ASSERT_NO_EXCEPTION  (::blink::NonThrowableExceptionState(__FILE__, __LINE__).ReturnThis())
#endif

#define IGNORE_EXCEPTION                (::blink::DummyExceptionStateForTesting().ReturnThis())
#define IGNORE_EXCEPTION_FOR_TESTING    (::blink::DummyExceptionStateForTesting().ReturnThis())

#endif // BLINKIT_BLINK_EXCEPTION_STATE_H
