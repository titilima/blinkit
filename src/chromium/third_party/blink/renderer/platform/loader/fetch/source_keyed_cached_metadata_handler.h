// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: source_keyed_cached_metadata_handler.h
// Description: SourceKeyedCachedMetadataHandler Class
//      Author: Ziming Li
//     Created: 2019-12-25
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BLINKIT_BLINK_SOURCE_KEYED_CACHED_METADATA_HANDLER_H
#define BLINKIT_BLINK_SOURCE_KEYED_CACHED_METADATA_HANDLER_H

#pragma once

#include "third_party/blink/renderer/platform/loader/fetch/cached_metadata_handler.h"

namespace blink {

// An implementation of CachedMetadataHandler which can hold multiple
// CachedMetadata entries. These entries are keyed by a cryptograph hash of the
// source code which produced them.
//
// This is used to store cached metadata for multiple inline scripts on a single
// HTML document's resource.
class SourceKeyedCachedMetadataHandler final : public CachedMetadataHandler
{
public:
private:
};

} // namespace blink

#endif // BLINKIT_BLINK_SOURCE_KEYED_CACHED_METADATA_HANDLER_H
