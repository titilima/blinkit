// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: script_event_listener.cpp
// Description: Event Listener Helpers for JS
//      Author: Ziming Li
//     Created: 2019-03-08
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "script_event_listener.h"

#include "core/dom/QualifiedName.h"

#include "bindings/duk_lazy_event_listener.h"

using namespace BlinKit;

namespace blink {

PassRefPtr<EventListener> createAttributeEventListener(Node *node, const QualifiedName &name, const AtomicString &value, const AtomicString &eventParameterName)
{
    assert(nullptr != node);
    if (value.isNull())
        return nullptr;

    return DukLazyEventListener::Create(name.localName(), eventParameterName, value);
}

PassRefPtr<EventListener> createAttributeEventListener(LocalFrame *frame, const QualifiedName &name, const AtomicString &value, const AtomicString &eventParameterName)
{
    assert(false); // BKTODO:
    return nullptr;
}

} // namespace blink
