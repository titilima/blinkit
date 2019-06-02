// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_exception_state.cpp
// Description: DukExceptionState Class
//      Author: Ziming Li
//     Created: 2018-08-28
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include "duk_exception_state.h"

#include "core/dom/ExceptionCode.h"

using namespace blink;

namespace BlinKit {

DukExceptionState::DukExceptionState(duk_context *context, const char *propertyName, const char *interfaceName)
    : m_context(context), m_propertyName(propertyName), m_interfaceName(interfaceName)
{
    // Nothing
}

void DukExceptionState::throwException(void)
{
    const std::string message = m_message.to_string();
    switch (m_exceptionCode)
    {
        case SyntaxError:
            duk_syntax_error(m_context, "%s", message.c_str());
            break;
        default:
            assert(false); // Not reached!
    }    
}

} // namespace BlinKit
