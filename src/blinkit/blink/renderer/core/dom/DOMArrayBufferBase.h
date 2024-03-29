#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: DOMArrayBufferBase.h
// Description: DOMArrayBufferBase Class
//      Author: Ziming Li
//     Created: 2021-07-19
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DOMArrayBufferBase_h
#define DOMArrayBufferBase_h

#include "blinkit/blink/renderer/bindings/core/script_wrappable.h"
#include "blinkit/blink/renderer/wtf/ArrayBuffer.h"
#include "blinkit/blink/renderer/wtf/RefCounted.h"

namespace blink {

class DOMArrayBufferBase : public RefCounted<DOMArrayBufferBase>, public ScriptWrappable
{
public:
    virtual ~DOMArrayBufferBase() { }

    const WTF::ArrayBuffer* buffer() const { return m_buffer.get(); }
    WTF::ArrayBuffer* buffer() { return m_buffer.get(); }

    const void* data() const { return buffer()->data(); }
    void* data() { return buffer()->data(); }
    unsigned byteLength() const { return buffer()->byteLength(); }
    bool transfer(WTF::ArrayBufferContents& result) { return buffer()->transfer(result); }
    bool shareContentsWith(WTF::ArrayBufferContents& result) { return buffer()->shareContentsWith(result); }
    bool isNeutered() const { return buffer()->isNeutered(); }
    bool isShared() const { return buffer()->isShared(); }

protected:
    explicit DOMArrayBufferBase(PassRefPtr<WTF::ArrayBuffer> buffer)
        : m_buffer(buffer)
    {
        ASSERT(m_buffer);
    }

    RefPtr<WTF::ArrayBuffer> m_buffer;
};

} // namespace blink

#endif // DOMArrayBuffer_h
