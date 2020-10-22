// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: offscreen_font_selector.h
// Description: OffscreenFontSelector Class
//      Author: Ziming Li
//     Created: 2020-10-22
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_CSS_OFFSCREEN_FONT_SELECTOR_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_CSS_OFFSCREEN_FONT_SELECTOR_H_

#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/core/css/font_face_cache.h"
#include "third_party/blink/renderer/platform/fonts/font_selector.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/forward.h"
#include "third_party/blink/renderer/platform/wtf/hash_map.h"
#include "third_party/blink/renderer/platform/wtf/hash_set.h"

namespace blink {

class ExecutionContext;
class FontDescription;

class CORE_EXPORT OffscreenFontSelector : public FontSelector {
 public:
  static OffscreenFontSelector* Create(ExecutionContext* context) {
    return new OffscreenFontSelector(context);
  }
  ~OffscreenFontSelector() override;

  unsigned Version() const override { return 1; }

  void ReportNotDefGlyph() const override;

  scoped_refptr<FontData> GetFontData(const FontDescription&,
                                      const AtomicString&) override;
  void WillUseFontData(const FontDescription&,
                       const AtomicString& family,
                       const String& text) override;
  void WillUseRange(const FontDescription&,
                    const AtomicString& family_name,
                    const FontDataForRangeSet&) override;

  void RegisterForInvalidationCallbacks(FontSelectorClient*) override;
  void UnregisterForInvalidationCallbacks(FontSelectorClient*) override;

  void FontCacheInvalidated() override;
  void FontFaceInvalidated() override;

  FontFaceCache* GetFontFaceCache() override { return &font_face_cache_; }

  bool IsPlatformFamilyMatchAvailable(
      const FontDescription&,
      const AtomicString& passed_family) override;

  ExecutionContext* GetExecutionContext() const override {
    return execution_context_;
  }

 protected:
  explicit OffscreenFontSelector(ExecutionContext*);

  void DispatchInvalidationCallbacks();

 private:
  FontFaceCache font_face_cache_;

  Member<ExecutionContext> execution_context_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_CSS_OFFSCREEN_FONT_SELECTOR_H_
