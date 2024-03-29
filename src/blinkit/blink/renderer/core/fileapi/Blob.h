#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: Blob.h
// Description: Blob Class
//      Author: Ziming Li
//     Created: 2021-07-22
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2010 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef Blob_h
#define Blob_h

#include "blinkit/blink/renderer/core/dom/DOMArrayBuffer.h"
#include "blinkit/blink/renderer/core/dom/DOMArrayBufferView.h"
#if 0 // BKTODO:
#include "blinkit/blink/renderer/core/html/URLRegistry.h"
#include "blinkit/blink/renderer/core/imagebitmap/ImageBitmapSource.h"
#endif
#include "blinkit/blink/renderer/platform/blob/BlobData.h"
#include "blinkit/blink/renderer/wtf/PassRefPtr.h"
#include "blinkit/blink/renderer/wtf/RefCounted.h"
#include "blinkit/blink/renderer/wtf/text/WTFString.h"

namespace blink {

class BlobPropertyBag;
class ExceptionState;
class ExecutionContext;

class Blob : public GarbageCollectedFinalized<Blob>, public ScriptWrappable { // BKTODO: , public URLRegistrable, public ImageBitmapSource {
    DEFINE_WRAPPERTYPEINFO();
public:
    static Blob* create(ExceptionState&)
    {
        return new Blob(BlobDataHandle::create());
    }

    // BKTODO: static Blob* create(const HeapVector<ArrayBufferOrArrayBufferViewOrBlobOrString>&, const BlobPropertyBag&, ExceptionState&);

    static Blob* create(PassRefPtr<BlobDataHandle> blobDataHandle)
    {
        return new Blob(blobDataHandle);
    }

    ~Blob() override;

    virtual unsigned long long size() const { return m_blobDataHandle->size(); }
    virtual Blob* slice(long long start, long long end, const String& contentType, ExceptionState&) const;

    // To allow ExceptionState to be passed in last, manually enumerate the optional argument overloads.
    Blob* slice(ExceptionState& exceptionState) const
    {
        return slice(0, std::numeric_limits<long long>::max(), String(), exceptionState);
    }
    Blob* slice(long long start, ExceptionState& exceptionState) const
    {
        return slice(start, std::numeric_limits<long long>::max(), String(), exceptionState);
    }
    Blob* slice(long long start, long long end, ExceptionState& exceptionState) const
    {
        return slice(start, end, String(), exceptionState);
    }

    virtual void close(ExecutionContext*, ExceptionState&);

    String type() const { return m_blobDataHandle->type(); }
    String uuid() const { return m_blobDataHandle->uuid(); }
    PassRefPtr<BlobDataHandle> blobDataHandle() const { return m_blobDataHandle; }
    // True for all File instances, including the user-built ones.
    virtual bool isFile() const { return false; }
    // Only true for File instances that are backed by platform files.
    virtual bool hasBackingFile() const { return false; }
    bool hasBeenClosed() const { return m_hasBeenClosed; }

    // Used by the JavaScript Blob and File constructors.
    virtual void appendTo(BlobData&) const;

#if 0 // BKTODO:
    // URLRegistrable to support PublicURLs.
    URLRegistry& registry() const final;

    // ImageBitmapSource implementation
    bool isBlob() const override { return true; }
#endif

    DEFINE_INLINE_TRACE() { }

protected:
    explicit Blob(PassRefPtr<BlobDataHandle>);

    template<typename ItemType>
    static void populateBlobData(BlobData* blobData, const HeapVector<ItemType>& parts, bool normalizeLineEndingsToNative)
    {
        for (size_t i = 0; i < parts.size(); ++i) {
            const ItemType& item = parts[i];
            if (item.isArrayBuffer()) {
                RefPtr<DOMArrayBuffer> arrayBuffer = item.getAsArrayBuffer();
                blobData->appendBytes(arrayBuffer->data(), arrayBuffer->byteLength());
            } else if (item.isArrayBufferView()) {
                RefPtr<DOMArrayBufferView> arrayBufferView = item.getAsArrayBufferView();
                blobData->appendBytes(arrayBufferView->baseAddress(), arrayBufferView->byteLength());
            } else if (item.isBlob()) {
                item.getAsBlob()->appendTo(*blobData);
            } else if (item.isString()) {
                blobData->appendText(item.getAsString(), normalizeLineEndingsToNative);
            } else {
                ASSERT_NOT_REACHED();
            }
        }
    }

    static void clampSliceOffsets(long long size, long long& start, long long& end);
private:
    Blob();

    RefPtr<BlobDataHandle> m_blobDataHandle;
    bool m_hasBeenClosed;
};

} // namespace blink

#endif // Blob_h
