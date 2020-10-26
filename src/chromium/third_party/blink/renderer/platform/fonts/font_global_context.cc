// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: font_global_context.cc
// Description: FontGlobalContext Class
//      Author: Ziming Li
//     Created: 2020-10-22
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/platform/fonts/font_global_context.h"

#include "third_party/blink/renderer/platform/fonts/font_cache.h"
// BKTODO: #include "third_party/blink/renderer/platform/fonts/font_unique_name_lookup.h"
#include "third_party/blink/renderer/platform/wtf/thread_specific.h"

namespace blink {

FontGlobalContext* FontGlobalContext::Get(CreateIfNeeded create_if_needed) {
  DEFINE_THREAD_SAFE_STATIC_LOCAL(ThreadSpecific<FontGlobalContext*>,
                                  font_persistent, ());
  if (!*font_persistent && create_if_needed == kCreate) {
    *font_persistent = new FontGlobalContext();
  }
  return *font_persistent;
}

FontGlobalContext::FontGlobalContext() : harfbuzz_font_funcs_(nullptr) {}

FontGlobalContext::~FontGlobalContext() = default;

FontUniqueNameLookup* FontGlobalContext::GetFontUniqueNameLookup() {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  if (!Get()->font_unique_name_lookup_) {
    Get()->font_unique_name_lookup_ =
        FontUniqueNameLookup::GetPlatformUniqueNameLookup();
  }
  return Get()->font_unique_name_lookup_.get();
#endif
}

void FontGlobalContext::ClearMemory() {
  if (!Get(kDoNotCreate))
    return;

  GetFontCache().Invalidate();
}

void FontGlobalContext::ClearForTesting() {
  FontGlobalContext* ctx = Get();
  ctx->font_cache_.Invalidate();
}

}  // namespace blink
