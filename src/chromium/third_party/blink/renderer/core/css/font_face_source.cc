// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: font_face_source.cc
// Description: FontFaceSource Class
//      Author: Ziming Li
//     Created: 2020-09-18
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/css/font_face_source.h"

#include "third_party/blink/renderer/core/css/font_face_set_document.h"
#include "third_party/blink/renderer/core/css/font_face_set_worker.h"

namespace blink {

FontFaceSet* FontFaceSource::fonts(Document& document) {
  return FontFaceSetDocument::From(document);
}

FontFaceSet* FontFaceSource::fonts(WorkerGlobalScope& worker) {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  return FontFaceSetWorker::From(worker);
#endif
}

}  // namespace blink
