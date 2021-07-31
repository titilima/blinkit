/*
 * Copyright (C) 2012 Google Inc. All rights reserved.
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

#include "platform/graphics/DecodingImageGenerator.h"

#include "platform/PlatformInstrumentation.h"
#include "platform/SharedBuffer.h"
#include "platform/TraceEvent.h"
#include "platform/graphics/ImageFrameGenerator.h"
#include "platform/image-decoders/ImageDecoder.h"
#include "third_party/skia/include/core/SkData.h"

namespace blink {

DecodingImageGenerator::DecodingImageGenerator(PassRefPtr<ImageFrameGenerator> frameGenerator, const SkImageInfo& info, size_t index)
    : SkImageGenerator(info)
    , m_frameGenerator(frameGenerator)
    , m_frameIndex(index)
    , m_generationId(0)
    , m_canYUVDecode(false)
{
}

DecodingImageGenerator::~DecodingImageGenerator()
{
}

SkData* DecodingImageGenerator::onRefEncodedData()
{
    TRACE_EVENT0("blink", "DecodingImageGenerator::refEncodedData");

    return m_frameGenerator->refEncodedData();
}

bool DecodingImageGenerator::onGetPixels(const SkImageInfo& info, void* pixels, size_t rowBytes, SkPMColor table[], int* tableCount)
{
    TRACE_EVENT1("blink", "DecodingImageGenerator::getPixels", "frame index", static_cast<int>(m_frameIndex));

    // Implementation doesn't support scaling yet so make sure we're not given a different size.
    if (info.width() != getInfo().width() || info.height() != getInfo().height())
        return false;

    if (info.colorType() != getInfo().colorType()) {
        // blink::ImageFrame may have changed the owning SkBitmap to kOpaque_SkAlphaType after fully decoding the image frame,
        // so if we see a request for opaque, that is ok even if our initial alpha type was not opaque.
        return false;
    }

    PlatformInstrumentation::willDecodeLazyPixelRef(m_generationId);
    bool decoded = m_frameGenerator->decodeAndScale(m_frameIndex, getInfo(), pixels, rowBytes);
    PlatformInstrumentation::didDecodeLazyPixelRef();

    return decoded;
}

bool DecodingImageGenerator::onGetYUV8Planes(SkISize sizes[3], void* planes[3], size_t rowBytes[3], SkYUVColorSpace* colorSpace)
{
    if (!m_canYUVDecode)
        return false;

    bool requestingYUVSizes = !planes || !planes[0];

    TRACE_EVENT1("blink", "DecodingImageGenerator::getYUV8Planes", requestingYUVSizes ? "sizes" : "frame index", static_cast<int>(m_frameIndex));

    if (requestingYUVSizes)
        return m_frameGenerator->getYUVComponentSizes(sizes);

    if (colorSpace)
        *colorSpace = kJPEG_SkYUVColorSpace;

    PlatformInstrumentation::willDecodeLazyPixelRef(m_generationId);
    bool decoded = m_frameGenerator->decodeToYUV(m_frameIndex, sizes, planes, rowBytes);
    PlatformInstrumentation::didDecodeLazyPixelRef();

    return decoded;
}

SkImageGenerator* DecodingImageGenerator::create(SkData* data)
{
    RefPtr<SharedBuffer> buffer = SharedBuffer::create(data->bytes(), data->size());

    // We just need the size of the image, so we have to temporarily create an ImageDecoder. Since
    // we only need the size, it doesn't really matter about premul or not, or gamma settings.
    OwnPtr<ImageDecoder> decoder = ImageDecoder::create(*buffer.get(), ImageDecoder::AlphaPremultiplied, ImageDecoder::GammaAndColorProfileApplied);
    if (!decoder)
        return 0;

    decoder->setData(buffer.get(), true);
    if (!decoder->isSizeAvailable())
        return 0;

    const IntSize size = decoder->size();
    const SkImageInfo info = SkImageInfo::MakeN32Premul(size.width(), size.height());

    RefPtr<ImageFrameGenerator> frame = ImageFrameGenerator::create(SkISize::Make(size.width(), size.height()), buffer, true, false);
    if (!frame)
        return 0;

    return new DecodingImageGenerator(frame, info, 0);
}

} // namespace blink
