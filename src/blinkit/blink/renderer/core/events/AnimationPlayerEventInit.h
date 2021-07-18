// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: AnimationPlayerEventInit.h
// Description: AnimationPlayerEventInit Class
//      Author: Ziming Li
//     Created: 2018-08-02
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_ANIMATION_PLAYER_EVENT_INIT_H
#define BLINKIT_BLINKIT_ANIMATION_PLAYER_EVENT_INIT_H

#pragma once

#include <optional>
#include "blinkit/blink/renderer/core/events/EventInit.h"

namespace blink {

class AnimationPlayerEventInit : public EventInit
{
public:
    bool hasCurrentTime(void) const { return m_currentTime.has_value(); }
    double currentTime(void) const { return m_currentTime.value(); }
    void setCurrentTime(double value) { m_currentTime = value; }

    bool hasTimelineTime(void) const { return m_timelineTime.has_value(); }
    double timelineTime(void) const { return m_timelineTime.value(); }
    void setTimelineTime(double value) { m_timelineTime = value; }
private:
    std::optional<double> m_currentTime, m_timelineTime;
};

} // namespace blink

#endif // BLINKIT_BLINKIT_ANIMATION_PLAYER_EVENT_INIT_H
