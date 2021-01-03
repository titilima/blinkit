// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: resize_observer_data_map.h
// Description: ResizeObserverDataMap
//      Author: Ziming Li
//     Created: 2021-01-03
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_RESIZE_OBSERVER_DATA_MAP_H
#define BLINKIT_BLINK_RESIZE_OBSERVER_DATA_MAP_H

#pragma once

#ifdef BLINKIT_CRAWLER_ONLY
#   error This file could not be used as a part of crawler-only module.
#endif

#include <unordered_map>
#include "third_party/blink/renderer/platform/heap/member.h"

namespace blink {

class ResizeObservation;
class ResizeObserver;

using ResizeObserverDataMap = std::unordered_map<ResizeObserver *, Member<ResizeObservation>>;

} // namespace blink

#endif // BLINKIT_BLINK_RESIZE_OBSERVER_DATA_MAP_H
