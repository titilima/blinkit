// -------------------------------------------------
// BlinKit - cc Library
// -------------------------------------------------
//   File Name: image_provider.cc
// Description: ImageProvider Class
//      Author: Ziming Li
//     Created: 2020-12-21
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "cc/paint/image_provider.h"

namespace cc {

#if 0 // BKTODO:
ImageProvider::ScopedDecodedDrawImage::ScopedDecodedDrawImage() = default;

ImageProvider::ScopedDecodedDrawImage::ScopedDecodedDrawImage(
    DecodedDrawImage image)
    : image_(std::move(image)) {}

ImageProvider::ScopedDecodedDrawImage::ScopedDecodedDrawImage(
    DecodedDrawImage image,
    DestructionCallback callback)
    : image_(std::move(image)), destruction_callback_(std::move(callback)) {}

ImageProvider::ScopedDecodedDrawImage::ScopedDecodedDrawImage(
    ScopedDecodedDrawImage&& other) {
  image_ = std::move(other.image_);
  destruction_callback_ = std::move(other.destruction_callback_);
}

ImageProvider::ScopedDecodedDrawImage& ImageProvider::ScopedDecodedDrawImage::
operator=(ScopedDecodedDrawImage&& other) {
  DestroyDecode();

  image_ = std::move(other.image_);
  destruction_callback_ = std::move(other.destruction_callback_);
  return *this;
}

ImageProvider::ScopedDecodedDrawImage::~ScopedDecodedDrawImage() {
  DestroyDecode();
}

void ImageProvider::ScopedDecodedDrawImage::DestroyDecode() {
  if (!destruction_callback_.is_null())
    std::move(destruction_callback_).Run();
}
#endif

}  // namespace cc
