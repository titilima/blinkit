// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_lazy_event_listener.cpp
// Description: DukLazyEventListener Class
//      Author: Ziming Li
//     Created: 2019-07-24
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_lazy_event_listener.h"

namespace BlinKit {

DukLazyEventListener::DukLazyEventListener(const AtomicString &functionName, const AtomicString &eventParameterName, const String &code)
    : m_functionName(functionName)
    , m_eventParameterName(eventParameterName)
    , m_code(code)
{
    // Nothing
}

} // namespace BlinKit
