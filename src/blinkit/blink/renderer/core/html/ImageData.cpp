// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ImageData.cpp
// Description: ImageData Class
//      Author: Ziming Li
//     Created: 2021-08-01
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2008 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "./ImageData.h"

#include "blinkit/blink/renderer/bindings/core/exception_state.h"
#include "blinkit/blink/renderer/core/dom/ExceptionCode.h"
#include "blinkit/blink/renderer/core/frame/ImageBitmap.h"
#include "blinkit/blink/renderer/platform/RuntimeEnabledFeatures.h"

namespace blink {

ImageData* ImageData::create(const IntSize& size)
{
    Checked<int, RecordOverflow> dataSize = 4;
    dataSize *= size.width();
    dataSize *= size.height();
    if (dataSize.hasOverflowed() || dataSize.unsafeGet() < 0)
        return nullptr;

    ASSERT(false); // BKTODO:
    return nullptr;
#if 0
    RefPtr<DOMUint8ClampedArray> byteArray =
        DOMUint8ClampedArray::createOrNull(dataSize.unsafeGet());
    if (!byteArray)
        return nullptr;

    return new ImageData(size, byteArray.release());
#endif
}

#if 0 // BKTODO:
ImageData* ImageData::create(const IntSize& size, PassRefPtr<DOMUint8ClampedArray> byteArray)
{
    Checked<int, RecordOverflow> dataSize = 4;
    dataSize *= size.width();
    dataSize *= size.height();
    if (dataSize.hasOverflowed())
        return nullptr;

    if (dataSize.unsafeGet() < 0
        || static_cast<unsigned>(dataSize.unsafeGet()) > byteArray->length())
        return nullptr;

    return new ImageData(size, byteArray);
}
#endif

ImageData* ImageData::create(unsigned width, unsigned height, ExceptionState& exceptionState)
{
    if (!width || !height) {
        exceptionState.throwDOMException(IndexSizeError, String::format("The source %s is zero or not a number.", width ? "height" : "width"));
        return nullptr;
    }

    Checked<unsigned, RecordOverflow> dataSize = 4;
    dataSize *= width;
    dataSize *= height;
    if (dataSize.hasOverflowed()
        || static_cast<int>(width) < 0
        || static_cast<int>(height) < 0) {
        exceptionState.throwDOMException(IndexSizeError, "The requested image size exceeds the supported range.");
        return nullptr;
    }

    ASSERT(false); // BKTODO:
    return nullptr;
#if 0
    RefPtr<DOMUint8ClampedArray> byteArray =
        DOMUint8ClampedArray::createOrNull(dataSize.unsafeGet());
    if (!byteArray) {
        exceptionState.throwDOMException(V8GeneralError, "Out of memory at ImageData creation");
        return nullptr;
    }

    return new ImageData(IntSize(width, height), byteArray.release());
#endif
}

#if 0 // BKTODO:
bool ImageData::validateConstructorArguments(DOMUint8ClampedArray* data, unsigned width, unsigned& lengthInPixels, ExceptionState& exceptionState)
{
    if (!width) {
        exceptionState.throwDOMException(IndexSizeError, "The source width is zero or not a number.");
        return false;
    }
    ASSERT(data);
    unsigned length = data->length();
    if (!length) {
        exceptionState.throwDOMException(IndexSizeError, "The input data has a zero byte length.");
        return false;
    }
    if (length % 4) {
        exceptionState.throwDOMException(IndexSizeError, "The input data byte length is not a multiple of 4.");
        return false;
    }
    length /= 4;
    if (length % width) {
        exceptionState.throwDOMException(IndexSizeError, "The input data byte length is not a multiple of (4 * width).");
        return false;
    }
    lengthInPixels = length;
    return true;
}

ImageData* ImageData::create(DOMUint8ClampedArray* data, unsigned width, ExceptionState& exceptionState)
{
    unsigned lengthInPixels = 0;
    if (!validateConstructorArguments(data, width, lengthInPixels, exceptionState)) {
        ASSERT(exceptionState.hadException());
        return nullptr;
    }
    ASSERT(lengthInPixels && width);
    unsigned height = lengthInPixels / width;
    return new ImageData(IntSize(width, height), data);
}

ImageData* ImageData::create(DOMUint8ClampedArray* data, unsigned width, unsigned height, ExceptionState& exceptionState)
{
    unsigned lengthInPixels = 0;
    if (!validateConstructorArguments(data, width, lengthInPixels, exceptionState)) {
        ASSERT(exceptionState.hadException());
        return nullptr;
    }
    ASSERT(lengthInPixels && width);
    if (height != lengthInPixels / width) {
        exceptionState.throwDOMException(IndexSizeError, "The input data byte length is not equal to (4 * width * height).");
        return nullptr;
    }
    return new ImageData(IntSize(width, height), data);
}

ScriptPromise ImageData::createImageBitmap(ScriptState* scriptState, EventTarget& eventTarget, int sx, int sy, int sw, int sh, ExceptionState& exceptionState)
{
    if (!sw || !sh) {
        exceptionState.throwDOMException(IndexSizeError, String::format("The source %s provided is 0.", sw ? "height" : "width"));
        return ScriptPromise();
    }
    if (data()->bufferBase()->isNeutered()) {
        exceptionState.throwDOMException(InvalidStateError, "The source data has been neutered.");
        return ScriptPromise();
    }
    return ImageBitmapSource::fulfillImageBitmap(scriptState, ImageBitmap::create(this, IntRect(sx, sy, sw, sh)));
}

ImageData::ImageData(const IntSize& size, PassRefPtr<DOMUint8ClampedArray> byteArray)
    : m_size(size)
    , m_data(byteArray)
{
    ASSERT(size.width() >= 0 && size.height() >= 0);
    ASSERT_WITH_SECURITY_IMPLICATION(static_cast<unsigned>(size.width() * size.height() * 4) <= m_data->length());
}
#endif

void ImageData::dispose()
{
    ASSERT(false); // BKTODO: m_data.clear();
}

} // namespace blink
