// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: resource_load_priority.h
// Description: ResourceLoadPriority Enum
//      Author: Ziming Li
//     Created: 2019-11-28
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_RESOURCE_LOAD_PRIORITY_H
#define BLINKIT_BLINK_RESOURCE_LOAD_PRIORITY_H

#pragma once

namespace blink {

enum class ResourceLoadPriority {
    kUnresolved = -1,
    kVeryLow,
    kLow,
    kMedium,
    kHigh,
    kVeryHigh,
    kLowest = kVeryLow,
    kHighest = kVeryHigh,
};

} // namespace blink

#endif // BLINKIT_BLINK_RESOURCE_LOAD_PRIORITY_H
