// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: font_selector_client.h
// Description: FontSelectorClient Class
//      Author: Ziming Li
//     Created: 2020-08-02
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_PLATFORM_FONTS_FONT_SELECTOR_CLIENT_H_
#define THIRD_PARTY_BLINK_RENDERER_PLATFORM_FONTS_FONT_SELECTOR_CLIENT_H_

#include "third_party/blink/renderer/platform/heap/handle.h"

namespace blink {

class FontSelector;

class FontSelectorClient : public GarbageCollectedMixin {
 public:
  virtual ~FontSelectorClient() = default;

  virtual void FontsNeedUpdate(FontSelector*) = 0;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_PLATFORM_FONTS_FONT_SELECTOR_CLIENT_H_
