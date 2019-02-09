// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: script_event_listener.h
// Description: Event Listener Helpers for JS
//      Author: Ziming Li
//     Created: 2018-08-28
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_SCRIPT_EVENT_LISTENER_H
#define BLINKIT_JS_SCRIPT_EVENT_LISTENER_H

#pragma once

#include "core/events/EventListener.h"
#include "wtf/PassRefPtr.h"

namespace blink {

class LocalFrame;
class QualifiedName;

PassRefPtr<EventListener> createAttributeEventListener(Node *, const QualifiedName &, const AtomicString &value, const AtomicString &eventParameterName);

PassRefPtr<EventListener> createAttributeEventListener(LocalFrame *, const QualifiedName &, const AtomicString &value, const AtomicString &eventParameterName);

} // namespace blink

#endif // BLINKIT_JS_SCRIPT_EVENT_LISTENER_H
