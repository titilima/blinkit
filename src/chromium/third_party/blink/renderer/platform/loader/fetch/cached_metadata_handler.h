// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: cached_metadata_handler.h
// Description: CachedMetadataHandler Class
//      Author: Ziming Li
//     Created: 2019-12-25
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BLINKIT_BLINK_CACHED_METADATA_HANDLER_H
#define BLINKIT_BLINK_CACHED_METADATA_HANDLER_H

#pragma once

#include "third_party/blink/renderer/platform/heap/handle.h"

namespace blink {

// Handler class for caching operations.
class CachedMetadataHandler : public GarbageCollectedFinalized<CachedMetadataHandler>
{
public:
    virtual ~CachedMetadataHandler(void) = default;
protected:
    CachedMetadataHandler(void) = default;
};

} // namespace blink

#endif // BLINKIT_BLINK_CACHED_METADATA_HANDLER_H
