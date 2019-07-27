// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_lazy_event_listener.h
// Description: DukLazyEventListener Class
//      Author: Ziming Li
//     Created: 2019-07-24
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_LAZY_EVENT_LISTENER_H
#define BLINKIT_JS_DUK_LAZY_EVENT_LISTENER_H

#pragma once

#include "duk_event_listener.h"
#include "wtf/text/AtomicString.h"

namespace BlinKit {

class DukLazyEventListener : public DukEventListener
{
public:
    static PassRefPtr<DukLazyEventListener> Create(const AtomicString &functionName, const AtomicString &eventParameterName, const String &code)
    {
        return adoptRef(new DukLazyEventListener(functionName, eventParameterName, code));
    }
private:
    DukLazyEventListener(const AtomicString &functionName, const AtomicString &eventParameterName, const String &code);

    AtomicString m_functionName;
    AtomicString m_eventParameterName;
    String m_code;
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_LAZY_EVENT_LISTENER_H
