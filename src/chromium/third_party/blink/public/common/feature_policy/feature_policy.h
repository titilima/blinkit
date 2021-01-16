// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: feature_policy.h
// Description: Feature Policy
//      Author: Ziming Li
//     Created: 2021-01-12
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_FEATURE_POLICY_H
#define BLINKIT_BLINK_FEATURE_POLICY_H

#pragma once

namespace mojom {

enum class FeaturePolicyFeature {
    kImageCompression,
    kLegacyImageFormats,
    kMaxDownscalingImage
};

} // namespace mojom

#endif // BLINKIT_BLINK_FEATURE_POLICY_H
