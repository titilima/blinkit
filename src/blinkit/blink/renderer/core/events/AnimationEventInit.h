// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: AnimationEventInit.h
// Description: AnimationEventInit Class
//      Author: Ziming Li
//     Created: 2018-08-02
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_ANIMATION_EVENT_INIT_H
#define BLINKIT_BLINKIT_ANIMATION_EVENT_INIT_H

#pragma once

#include "blinkit/blink/renderer/core/events/EventInit.h"
#include "blinkit/blink/renderer/wtf/text/WTFString.h"

namespace blink {

class AnimationEventInit : public EventInit
{
public:
    bool hasAnimationName(void) const { return !m_animationName.isNull(); }
    const String& animationName(void) const { return m_animationName; }
    void setAnimationName(const String &value) { m_animationName = value; }

    bool hasElapsedTime(void) const { return true; }
    double elapsedTime(void) const { return m_elapsedTime; }
    void setElapsedTime(double value) { m_elapsedTime = value; }
private:
    String m_animationName;
    double m_elapsedTime = 0.0;
};

} // namespace blink

#endif // BLINKIT_BLINKIT_ANIMATION_EVENT_INIT_H
