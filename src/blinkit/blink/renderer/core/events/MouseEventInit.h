// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: MouseEventInit.h
// Description: MouseEventInit Class
//      Author: Ziming Li
//     Created: 2018-07-12
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_MOUSE_EVENT_INIT_H
#define BLINKIT_BLINKIT_MOUSE_EVENT_INIT_H

#pragma once

#include "blinkit/blink/renderer/core/events/EventModifierInit.h"
#include "blinkit/blink/renderer/core/events/EventTarget.h"

namespace blink {

class MouseEventInit : public EventModifierInit
{
public:
    bool hasButton(void) const { return true; }
    int button(void) const { return m_button; }
    void setButton(int value) { m_button = value; }

    bool hasButtons(void) const { return true; }
    unsigned buttons(void) const { return m_buttons; }
    void setButtons(unsigned value) { m_buttons = value; }

    bool hasClientX(void) const { return true; }
    int clientX(void) const { return m_clientX; }
    void setClientX(int value) { m_clientX = value; }

    bool hasClientY(void) const { return true; }
    int clientY(void) const { return m_clientY; }
    void setClientY(int value) { m_clientY = value; }

    bool hasMovementX(void) const { return true; }
    int movementX(void) const { return m_movementX; }
    void setMovementX(int value) { m_movementX = value; }

    bool hasMovementY(void) const { return true; }
    int movementY(void) const { return m_movementY; }
    void setMovementY(int value) { m_movementY = value; }

    bool hasRelatedTarget(void) const { return m_relatedTarget; }
    PassRefPtrWillBeRawPtr<EventTarget> relatedTarget(void) const { return m_relatedTarget; }
    void setRelatedTarget(PassRefPtrWillBeRawPtr<EventTarget> value) { m_relatedTarget = value; }
    void setRelatedTargetToNull(void) { m_relatedTarget = RefPtrWillBeMember<EventTarget>(); }

    bool hasScreenX(void) const { return true; }
    int screenX(void) const { return m_screenX; }
    void setScreenX(int value) { m_screenX = value; }

    bool hasScreenY(void) const { return true; }
    int screenY(void) const { return m_screenY; }
    void setScreenY(int value) { m_screenY = value; }
private:
    int m_button = 0;
    unsigned m_buttons = 0;
    int m_clientX = 0, m_clientY = 0;
    int m_movementX = 0, m_movementY = 0;
    RefPtrWillBeMember<EventTarget> m_relatedTarget;
    int m_screenX = 0, m_screenY = 0;
};

} // namespace blink

#endif // BLINKIT_BLINKIT_MOUSE_EVENT_INIT_H
