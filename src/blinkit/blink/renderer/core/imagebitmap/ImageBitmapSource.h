// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ImageBitmapSource.h
// Description: ImageBitmapSource Class
//      Author: Ziming Li
//     Created: 2021-07-30
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
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
    // BKTODO: virtual ScriptPromise createImageBitmap(ScriptState*, EventTarget&, int sx, int sy, int sw, int sh, ExceptionState&);

    virtual bool isBlob() const { return false; }

    // BKTODO: static ScriptPromise fulfillImageBitmap(ScriptState*, PassRefPtrWillBeRawPtr<ImageBitmap>);
protected:
    virtual ~ImageBitmapSource() {}
};

} // namespace blink

#endif
