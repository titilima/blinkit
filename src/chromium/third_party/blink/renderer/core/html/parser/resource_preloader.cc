// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: resource_preloader.cc
// Description: ResourcePreloader Class
//      Author: Ziming Li
//     Created: 2019-10-27
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/html/parser/resource_preloader.h"

namespace blink {

void ResourcePreloader::TakeAndPreload(PreloadRequestStream& r) {
  PreloadRequestStream requests;
  requests.swap(r);

  for (PreloadRequestStream::iterator it = requests.begin();
       it != requests.end(); ++it)
    Preload(std::move(*it));
}

}  // namespace blink
