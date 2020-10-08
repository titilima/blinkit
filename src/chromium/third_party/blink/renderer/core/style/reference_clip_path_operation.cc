// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: reference_clip_path_operation.cc
// Description: ReferenceClipPathOperation Class
//      Author: Ziming Li
//     Created: 2020-10-08
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/style/reference_clip_path_operation.h"

namespace blink {

void ReferenceClipPathOperation::AddClient(SVGResourceClient& client) {
  ASSERT(false); // BKTODO:
#if 0
  if (resource_)
    resource_->AddClient(client);
#endif
}

void ReferenceClipPathOperation::RemoveClient(SVGResourceClient& client) {
  ASSERT(false); // BKTODO:
#if 0
  if (resource_)
    resource_->RemoveClient(client);
#endif
}

#if 0 // BKTODO:
SVGResource* ReferenceClipPathOperation::Resource() const {
  return resource_;
}
#endif

bool ReferenceClipPathOperation::operator==(const ClipPathOperation& o) const {
  if (!IsSameType(o))
    return false;
  const ReferenceClipPathOperation& other = ToReferenceClipPathOperation(o);
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return resource_ == other.resource_ && url_ == other.url_;
#endif
}

}  // namespace blink
