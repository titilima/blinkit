// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: viewport_data.h
// Description: ViewportData Class
//      Author: Ziming Li
//     Created: 2020-09-22
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_VIEWPORT_DATA_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_VIEWPORT_DATA_H_

#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/core/page/display_cutout.h"
#include "third_party/blink/renderer/core/page/viewport_description.h"
#include "third_party/blink/renderer/platform/heap/handle.h"

namespace blink {

class Document;

class ViewportData {
 public:
  ViewportData(Document& document);
  void Shutdown();

  bool ShouldMergeWithLegacyDescription(ViewportDescription::Type) const;
  bool ShouldOverrideLegacyDescription(ViewportDescription::Type) const;
  CORE_EXPORT void SetViewportDescription(const ViewportDescription&);
  CORE_EXPORT ViewportDescription GetViewportDescription() const;
  Length ViewportDefaultMinWidth() const { return viewport_default_min_width_; }

  void UpdateViewportDescription();

  // When true this will force a kCover viewport fit value which will result in
  // the document expanding into the display cutout area.
  CORE_EXPORT void SetExpandIntoDisplayCutout(bool expand);
  CORE_EXPORT bool GetExpandIntoDisplayCutout() const {
    return force_expand_display_cutout_;
  }
#if 0 // BKTODO:
  mojom::ViewportFit GetCurrentViewportFitForTests() const {
    return viewport_fit_;
  }
#endif

 private:
  Member<Document> document_;

  ViewportDescription viewport_description_;
  ViewportDescription legacy_viewport_description_;
  Length viewport_default_min_width_;

  // Stores the current value viewport-fit value.
  ViewportFit viewport_fit_ = blink::ViewportFit::kAuto;
  bool force_expand_display_cutout_ = false;

#if 0 // BKTODO:
  mojom::blink::DisplayCutoutHostAssociatedPtr display_cutout_host_;
#endif
};

inline bool ViewportData::ShouldOverrideLegacyDescription(
    ViewportDescription::Type origin) const {
  // The different (legacy) meta tags have different priorities based on the
  // type regardless of which order they appear in the DOM. The priority is
  // given by the ViewportDescription::Type enum.
  return origin >= legacy_viewport_description_.type;
}

}  // namespace blink
#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_VIEWPORT_DATA_H_
