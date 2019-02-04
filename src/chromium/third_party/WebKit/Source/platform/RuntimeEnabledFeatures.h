// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: RuntimeEnabledFeatures.h
// Description: RuntimeEnabledFeatures Class
//      Author: Ziming Li
//     Created: 2019-01-22
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_RUNTIME_ENABLED_FEATURES_H
#define BLINKIT_BLINK_RUNTIME_ENABLED_FEATURES_H

#pragma once

#include "wtf/Allocator.h"

#define BLINKIT_ENABLE_FEATURE(name)    static bool name ## Enabled(void) { return true;  }
#define BLINKIT_DISABLE_FEATURE(name)   static bool name ## Enabled(void) { return false; }

#define BLINK_DEFINE_STABLE_FEATURE BLINKIT_ENABLE_FEATURE

/**
 * Experimental & test features are disabled by default.
 * To enable these features, change the macros below to BLINKIT_ENABLE_FEATURE.
 */
#define BLINK_DEFINE_EXPERIMENTAL_FEATURE   BLINKIT_DISABLE_FEATURE
#define BLINK_DEFINE_TEST_FEATURE           BLINKIT_DISABLE_FEATURE

class RuntimeEnabledFeatures final {
    STATIC_ONLY(RuntimeEnabledFeatures);
public:
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(fractionalScrollOffsets)
};

#endif // BLINKIT_BLINK_RUNTIME_ENABLED_FEATURES_H
