// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: KeyframeEffectOptions.h
// Description: KeyframeEffectOptions Class
//      Author: Ziming Li
//     Created: 2018-08-06
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_KEYFRAME_EFFECT_OPTIONS_H
#define BLINKIT_BLINKIT_KEYFRAME_EFFECT_OPTIONS_H

#pragma once

#include "blinkit/blink/renderer/wtf/text/WTFString.h"

namespace blink {

class KeyframeEffectOptions
{
public:
    bool hasDelay(void) const { return true; }
    double delay(void) const { return m_delay; }
    void setDelay(double value) { m_delay = value; }

    bool hasDirection(void) const { return !m_direction.isNull(); }
    const String& direction(void) const { return m_direction; }
    void setDirection(const String &value) { m_direction = value; }

    bool hasEasing(void) const { return !m_easing.isNull(); }
    const String& easing(void) const { return m_easing; }
    void setEasing(const String &value) { m_easing = value; }

    bool hasEndDelay(void) const { return true; }
    double endDelay(void) const { return m_endDelay; }
    void setEndDelay(double value) { m_endDelay = value; }

    bool hasFill(void) const { return !m_fill.isNull(); }
    const String& fill(void) const { return m_fill; }
    void setFill(const String &value) { m_fill = value; }

    bool hasIterationStart(void) const { return true; }
    double iterationStart(void) const { return m_iterationStart; }
    void setIterationStart(double value) { m_iterationStart = value; }

    bool hasIterations(void) const { return true; }
    double iterations(void) const { return m_iterations; }
    void setIterations(double value) { m_iterations = value; }

    bool hasPlaybackRate(void) const { return true; }
    double playbackRate(void) const { return m_playbackRate; }
    void setPlaybackRate(double value) { m_playbackRate = value; }
private:
    double m_delay = 0.0;
    String m_direction{ "normal" };
    String m_easing{ "linear" };
    double m_endDelay = 0.0;
    String m_fill{ "auto" };
    double m_iterationStart = 0.0;
    double m_iterations = 1.0;
    double m_playbackRate = 1.0;
};

} // namespace blink

#endif // BLINKIT_BLINKIT_KEYFRAME_EFFECT_OPTIONS_H
