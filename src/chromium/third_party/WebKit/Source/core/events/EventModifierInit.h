// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: EventModifierInit.h
// Description: EventModifierInit Class
//      Author: Ziming Li
//     Created: 2018-07-12
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_EVENT_MODIFIER_INIT_H
#define BLINKIT_BLINK_EVENT_MODIFIER_INIT_H

#pragma once

#include "core/events/UIEventInit.h"

namespace blink {

class EventModifierInit : public UIEventInit
{
public:
    bool hasAltKey(void) const { return true; }
    bool altKey(void) const { return m_altKey; }
    void setAltKey(bool value) { m_altKey = value; }

    bool hasCtrlKey(void) const { return true; }
    bool ctrlKey(void) const { return m_ctrlKey; }
    void setCtrlKey(bool value) { m_ctrlKey = value; }

    bool hasMetaKey(void) const { return true; }
    bool metaKey(void) const { return m_metaKey; }
    void setMetaKey(bool value) { m_metaKey = value; }

    bool hasShiftKey(void) const { return true; }
    bool shiftKey(void) const { return m_shiftKey; }
    void setShiftKey(bool value) { m_shiftKey = value; }

    bool modifierAltGraph(void) const { return m_modifierAltGraph; }
    void setModifierAltGraph(bool value) { m_modifierAltGraph = value; }

    bool modifierOS(void) const { return m_modifierOS; }
    void setModifierOS(bool value) { m_modifierOS = value; }

    bool modifierFn(void) const { return m_modifierFn; }
    void setModifierFn(bool value) { m_modifierFn = value; }

    bool modifierCapsLock(void) const { return m_modifierCapsLock; }
    void setModifierCapsLock(bool value) { m_modifierCapsLock = value; }

    bool modifierScrollLock(void) const { return m_modifierScrollLock; }
    void setModifierScrollLock(bool value) { m_modifierScrollLock = value; }

    bool modifierNumLock(void) const { return m_modifierNumLock; }
    void setModifierNumLock(bool value) { m_modifierNumLock = value; }

    bool modifierSymbol(void) const { return m_modifierSymbol; }
    void setModifierSymbol(bool value) { m_modifierSymbol = value; }
private:
    bool m_altKey = false;
    bool m_ctrlKey = false;
    bool m_metaKey = false;
    bool m_shiftKey = false;
    bool m_modifierAltGraph = false;
    bool m_modifierOS = false;
    bool m_modifierFn = false;
    bool m_modifierCapsLock = false;
    bool m_modifierScrollLock = false;
    bool m_modifierNumLock = false;
    bool m_modifierSymbol = false;
};

} // namespace blink

#endif // BLINKIT_BLINK_EVENT_MODIFIER_INIT_H
