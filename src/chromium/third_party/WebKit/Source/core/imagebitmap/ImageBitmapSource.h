// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: ImageBitmapSource.h
// Description: ImageBitmapSource Class
//      Author: Ziming Li
//     Created: 2019-02-05
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ImageBitmapSource_h
#define ImageBitmapSource_h

#include "core/CoreExport.h"
#include "core/dom/ExceptionCode.h"
#include "platform/geometry/IntSize.h"

namespace blink {

class ImageBitmap;

class CORE_EXPORT ImageBitmapSource {
public:
    virtual IntSize bitmapSourceSize() const { return IntSize(); }

    virtual bool isBlob() const { return false; }
protected:
    virtual ~ImageBitmapSource() {}
};

} // namespace blink

#endif
