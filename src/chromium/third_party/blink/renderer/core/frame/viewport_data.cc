// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: viewport_data.cc
// Description: ViewportData Class
//      Author: Ziming Li
//     Created: 2020-09-29
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/frame/viewport_data.h"

// BKTODO: #include "third_party/blink/public/common/associated_interfaces/associated_interface_provider.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_frame_client.h"
#include "third_party/blink/renderer/core/frame/settings.h"
#include "third_party/blink/renderer/core/page/chrome_client.h"
#include "third_party/blink/renderer/core/page/page.h"

namespace blink {

ViewportData::ViewportData(Document& document) : document_(document) {}

void ViewportData::Trace(Visitor* visitor) {
  visitor->Trace(document_);
}

void ViewportData::Shutdown() {
  // TODO(https://crbug.com/800641): Use InterfaceInvalidator once it works with
  // associated interfaces.
  ASSERT(false); // BKTODO:
#if 0
  display_cutout_host_.reset();
#endif
}

bool ViewportData::ShouldMergeWithLegacyDescription(
    ViewportDescription::Type origin) const {
  return Settings::ViewportMetaMergeContentQuirk &&
         legacy_viewport_description_.IsMetaViewportType() &&
         legacy_viewport_description_.type == origin;
}

void ViewportData::SetViewportDescription(
    const ViewportDescription& viewport_description) {
  if (viewport_description.IsLegacyViewportType()) {
    if (viewport_description == legacy_viewport_description_)
      return;
    legacy_viewport_description_ = viewport_description;
  } else {
    if (viewport_description == viewport_description_)
      return;
    viewport_description_ = viewport_description;

    // The UA-defined min-width is considered specifically by Android WebView
    // quirks mode.
    if (!viewport_description.IsSpecifiedByAuthor())
      viewport_default_min_width_ = viewport_description.min_width;
  }

  UpdateViewportDescription();
}

ViewportDescription ViewportData::GetViewportDescription() const {
  ViewportDescription applied_viewport_description = viewport_description_;
  bool viewport_meta_enabled = Settings::ViewportMetaEnabled;
  if (legacy_viewport_description_.type !=
          ViewportDescription::kUserAgentStyleSheet &&
      viewport_meta_enabled)
    applied_viewport_description = legacy_viewport_description_;
  if (ShouldOverrideLegacyDescription(viewport_description_.type))
    applied_viewport_description = viewport_description_;

  return applied_viewport_description;
}

void ViewportData::UpdateViewportDescription() {
  if (!document_->GetFrame())
    return;

  // If the viewport_fit has changed we should send this to the browser. We
  // use the legacy viewport description which contains the viewport_fit
  // defined from the layout meta tag.
  ViewportFit current_viewport_fit =
      GetViewportDescription().GetViewportFit();

  // If we are forcing to expand into the display cutout then we should override
  // the viewport fit value.
  if (force_expand_display_cutout_)
    current_viewport_fit = ViewportFit::kCoverForcedByUserAgent;

  if (viewport_fit_ != current_viewport_fit) {
    ASSERT(false); // BKTODO:
#if 0
    if (AssociatedInterfaceProvider* provider =
            document_->GetFrame()
                ->Client()
                ->GetRemoteNavigationAssociatedInterfaces()) {
      // Bind the mojo interface.
      if (!display_cutout_host_.is_bound()) {
        provider->GetInterface(&display_cutout_host_);
        DCHECK(display_cutout_host_.is_bound());
      }

      // Even though we bind the mojo interface above there still may be cases
      // where this will fail (e.g. unit tests).
      display_cutout_host_->NotifyViewportFitChanged(current_viewport_fit);
    }
#endif

    viewport_fit_ = current_viewport_fit;
  }

  document_->GetPage()->GetChromeClient().DispatchViewportPropertiesDidChange(GetViewportDescription());
}

void ViewportData::SetExpandIntoDisplayCutout(bool expand) {
  if (force_expand_display_cutout_ == expand)
    return;

  force_expand_display_cutout_ = expand;
  UpdateViewportDescription();
}

}  // namespace blink
