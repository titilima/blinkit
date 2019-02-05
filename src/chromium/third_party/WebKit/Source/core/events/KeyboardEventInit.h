// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: KeyboardEventInit.h
// Description: KeyboardEventInit Class
//      Author: Ziming Li
//     Created: 2018-07-12
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_KEYBOARD_EVENT_INIT_H
#define BLINKIT_BLINK_KEYBOARD_EVENT_INIT_H

#pragma once

#include "core/events/EventModifierInit.h"
#include "wtf/text/WTFString.h"

namespace blink {

class KeyboardEventInit : public EventModifierInit
{
public:
    const String& keyIdentifier(void) const { return m_keyIdentifier; }
    const String& code(void) const { return m_code; }
    const String& key(void) const { return m_key; }
    unsigned location(void) const { return m_location; }
    bool repeat(void) const { return m_repeat; }
private:
    String m_keyIdentifier;
    String m_code;
    String m_key;
    unsigned m_location = 0;
    bool m_repeat = false;
};

} // namespace blink

#endif // BLINKIT_BLINK_KEYBOARD_EVENT_INIT_H
