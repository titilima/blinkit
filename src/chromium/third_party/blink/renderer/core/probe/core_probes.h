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

namespace blink {
namespace probe {
inline void nothingToDo(void) {}
} // namespace probe
} // namespace blink

#define AsyncTaskScheduled(...)         nothingToDo()
#define activeStyleSheetsUpdated(...)   nothingToDo()
#define animationPlayStateChanged(...)  nothingToDo()
#define didChangeViewport(...)          nothingToDo()
#define didCreateAnimation(...)         nothingToDo()
#define didInvalidateStyleAttr(...)     nothingToDo()
#define didPaint(...)                   nothingToDo()
#define didPerformSlotDistribution(...) nothingToDo()
#define fontsUpdated(...)               nothingToDo()
#define layerTreeDidChange(...)         nothingToDo()
#define pseudoElementDestroyed(...)     nothingToDo()

#endif // BLINKIT_BLINK_CORE_PROBES_H
