// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: TransitionEventInit.h
// Description: TransitionEventInit Class
//      Author: Ziming Li
//     Created: 2018-08-02
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_TRANSITION_EVENT_INIT_H
#define BLINKIT_BLINKIT_TRANSITION_EVENT_INIT_H

#pragma once

#include "blinkit/blink/renderer/core/events/EventInit.h"
#include "blinkit/blink/renderer/wtf/text/WTFString.h"

namespace blink {

class TransitionEventInit : public EventInit
{
public:
    bool hasElapsedTime(void) const { return true; }
    double elapsedTime(void) const { return m_elapsedTime; }
    void setElapsedTime(double value) { m_elapsedTime = value; }

    bool hasPropertyName(void) const { return !m_propertyName.isNull(); }
    const String& propertyName(void) const { return m_propertyName; }
    void setPropertyName(const String &value) { m_propertyName = value; }

    bool hasPseudoElement(void) const { return !m_pseudoElement.isNull(); }
    const String& pseudoElement(void) const { return m_pseudoElement; }
    void setPseudoElement(const String &value) { m_pseudoElement = value; }
private:
    double m_elapsedTime = 0.0;
    String m_propertyName{ "" };
    String m_pseudoElement{ "" };
};

} // namespace blink

#endif // BLINKIT_BLINKIT_TRANSITION_EVENT_INIT_H
