#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: exception_state_placeholders.h
// Description: ExceptionState Placeholders
//      Author: Ziming Li
//     Created: 2021-07-05
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_EXCEPTION_STATE_PLACEHOLDERS_H
#define BLINKIT_EXCEPTION_STATE_PLACEHOLDERS_H

#include "blinkit/blink/renderer/bindings/core/exception_state.h"

namespace blink {

class DummyExceptionStateForTesting final : public ExceptionState
{
public:
    DummyExceptionStateForTesting(void) = default;

    ExceptionState& ReturnThis(void) { return *this; }
};

} // namespace blink

#ifdef NDEBUG
#   define ASSERT_NO_EXCEPTION  (::blink::DummyExceptionStateForTesting().ReturnThis())
#else
#   define ASSERT_NO_EXCEPTION  (::blink::NonThrowableExceptionState(__FILE__, __LINE__).ReturnThis())
#endif

#define IGNORE_EXCEPTION                (::blink::DummyExceptionStateForTesting().ReturnThis())
#define IGNORE_EXCEPTION_FOR_TESTING    (::blink::DummyExceptionStateForTesting().ReturnThis())

#endif // BLINKIT_EXCEPTION_STATE_PLACEHOLDERS_H
