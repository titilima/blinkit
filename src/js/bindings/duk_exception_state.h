// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_exception_state.h
// Description: DukExceptionState Class
//      Author: Ziming Li
//     Created: 2018-08-28
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_EXCEPTION_STATE_H
#define BLINKIT_JS_DUK_EXCEPTION_STATE_H

#pragma once

#include "platform/bindings/exception_state.h"

namespace BlinKit {

class DukExceptionState final : public blink::ExceptionState
{
public:
    DukExceptionState(duk_context *context, const char *propertyName, const char *interfaceName = nullptr);
private:
    void throwException(void) override;

    duk_context *m_context;

    const char *m_propertyName;
    const char *m_interfaceName;
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_EXCEPTION_STATE_H
