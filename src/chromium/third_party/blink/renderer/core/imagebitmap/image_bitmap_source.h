// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: image_bitmap_source.h
// Description: ImageBitmapSource Class
//      Author: Ziming Li
//     Created: 2021-01-08
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_IMAGEBITMAP_IMAGE_BITMAP_SOURCE_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_IMAGEBITMAP_IMAGE_BITMAP_SOURCE_H_

#include "base/optional.h"
#if 0 // BKTODO:
#include "third_party/blink/renderer/bindings/core/v8/script_promise.h"
#include "third_party/blink/renderer/bindings/core/v8/script_promise_resolver.h"
#endif
#include "third_party/blink/renderer/core/core_export.h"
// BKTODO: #include "third_party/blink/renderer/platform/bindings/script_state.h"
#include "third_party/blink/renderer/platform/geometry/int_rect.h"
#include "third_party/blink/renderer/platform/geometry/int_size.h"

namespace blink {

class ImageBitmap;
class ImageBitmapOptions;

class CORE_EXPORT ImageBitmapSource {
 public:
  virtual IntSize BitmapSourceSize() const { return IntSize(); }
#if 0 // BKTODO:
  virtual ScriptPromise CreateImageBitmap(ScriptState*,
                                          EventTarget&,
                                          base::Optional<IntRect>,
                                          const ImageBitmapOptions&);
#endif

  virtual bool IsBlob() const { return false; }

#if 0 // BKTODO:
  static ScriptPromise FulfillImageBitmap(ScriptState*, ImageBitmap*);
#endif

 protected:
  virtual ~ImageBitmapSource() = default;
};

}  // namespace blink

#endif
