// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: ComputedTimingProperties.h
// Description: ComputedTimingProperties Class
//      Author: Ziming Li
//     Created: 2018-08-06
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_COMPUTED_TIMING_PROPERTIES_H
#define BLINKIT_BLINK_COMPUTED_TIMING_PROPERTIES_H

#pragma once

#include <optional>
#include "core/animation/KeyframeEffectOptions.h"

namespace blink {

class ComputedTimingProperties : public KeyframeEffectOptions
{
public:
    bool hasActiveDuration(void) const { return m_activeDuration.has_value(); }
    double activeDuration(void) const { return m_activeDuration.value(); }
    void setActiveDuration(double value) { m_activeDuration = value; }

    bool hasCurrentIteration(void) const { return m_currentIteration.has_value(); }
    unsigned currentIteration(void) const { return m_currentIteration.value(); }
    void setCurrentIteration(unsigned value) { m_currentIteration = value; }

    bool hasEndTime(void) const { return m_endTime.has_value(); }
    double endTime(void) const { return m_endTime.value(); }
    void setEndTime(double value) { m_endTime = value; }

    bool hasLocalTime(void) const { return m_localTime.has_value(); }
    double localTime(void) const { return m_localTime.value(); }
    void setLocalTime(double value) { m_localTime = value; }

    bool hasStartTime(void) const { return m_startTime.has_value(); }
    double startTime(void) const { return m_startTime.value(); }
    void setStartTime(double value) { m_startTime = value; }

    bool hasTimeFraction(void) const { return m_timeFraction.has_value(); }
    double timeFraction(void) const { return m_timeFraction.value(); }
    void setTimeFraction(double value) { m_timeFraction = value; }
private:
    std::optional<double> m_activeDuration;
    std::optional<unsigned> m_currentIteration;
    std::optional<double> m_endTime;
    std::optional<double> m_localTime;
    std::optional<double> m_startTime;
    std::optional<double> m_timeFraction;
};

} // namespace blink

#endif // BLINKIT_BLINK_COMPUTED_TIMING_PROPERTIES_H
