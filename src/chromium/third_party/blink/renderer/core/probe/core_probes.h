// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: core_probes.h
// Description: Placeholders for Probes
//      Author: Ziming Li
//     Created: 2020-08-30
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_CORE_PROBES_H
#define BLINKIT_BLINK_CORE_PROBES_H

#pragma once

#include "third_party/blink/renderer/core/animation/animation.h"

namespace blink {
namespace probe {

inline void animationPlayStateChanged(Document *, Animation *, Animation::AnimationPlayState, Animation::AnimationPlayState) {}
inline void didCreateAnimation(Document *, unsigned) {}
inline void didInvalidateStyleAttr(Node *) {}

} // namespace probe
} // namespace blink

#endif // BLINKIT_BLINK_CORE_PROBES_H
