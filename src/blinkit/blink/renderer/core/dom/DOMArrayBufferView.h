#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: DOMArrayBufferView.h
// Description: DOMArrayBufferView Class
//      Author: Ziming Li
//     Created: 2021-07-19
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DOMArrayBufferView_h
#define DOMArrayBufferView_h

#include "blinkit/blink/renderer/core/dom/DOMArrayBuffer.h"
#include "blinkit/blink/renderer/core/dom/DOMSharedArrayBuffer.h"
#include "blinkit/blink/renderer/wtf/ArrayBufferView.h"
#include "blinkit/blink/renderer/wtf/RefCounted.h"

namespace blink {

class DOMArrayBufferView : public RefCounted<DOMArrayBufferView>, public ScriptWrappable
{
    DEFINE_WRAPPERTYPEINFO();
public:
    typedef WTF::ArrayBufferView::ViewType ViewType;
    static const ViewType TypeInt8 = WTF::ArrayBufferView::TypeInt8;
    static const ViewType TypeUint8 = WTF::ArrayBufferView::TypeUint8;
    static const ViewType TypeUint8Clamped = WTF::ArrayBufferView::TypeUint8Clamped;
    static const ViewType TypeInt16 = WTF::ArrayBufferView::TypeInt16;
    static const ViewType TypeUint16 = WTF::ArrayBufferView::TypeUint16;
    static const ViewType TypeInt32 = WTF::ArrayBufferView::TypeInt32;
    static const ViewType TypeUint32 = WTF::ArrayBufferView::TypeUint32;
    static const ViewType TypeFloat32 = WTF::ArrayBufferView::TypeFloat32;
    static const ViewType TypeFloat64 = WTF::ArrayBufferView::TypeFloat64;
    static const ViewType TypeDataView = WTF::ArrayBufferView::TypeDataView;

    virtual ~DOMArrayBufferView() { }

    PassRefPtr<DOMArrayBuffer> buffer() const
    {
        ASSERT(!isShared());
        if (!m_domArrayBuffer) {
            m_domArrayBuffer = DOMArrayBuffer::create(view()->buffer());
        }
        return static_pointer_cast<DOMArrayBuffer>(m_domArrayBuffer);
    }

    PassRefPtr<DOMSharedArrayBuffer> bufferShared() const
    {
        ASSERT(isShared());
        if (!m_domArrayBuffer) {
            m_domArrayBuffer = DOMSharedArrayBuffer::create(view()->buffer());
        }
        return static_pointer_cast<DOMSharedArrayBuffer>(m_domArrayBuffer);
    }

    PassRefPtr<DOMArrayBufferBase> bufferBase() const
    {
        if (isShared()) {
            return bufferShared();
        }
        return buffer();
    }

    const WTF::ArrayBufferView* view() const { return m_bufferView.get(); }
    WTF::ArrayBufferView* view() { return m_bufferView.get(); }

    ViewType type() const { return view()->type(); }
    const char* typeName() { return view()->typeName(); }
    void* baseAddress() const { return view()->baseAddress(); }
    unsigned byteOffset() const { return view()->byteOffset(); }
    unsigned byteLength() const { return view()->byteLength(); }
    void setNeuterable(bool flag) { return view()->setNeuterable(flag); }
    bool isShared() const { return view()->isShared(); }

protected:
    explicit DOMArrayBufferView(PassRefPtr<WTF::ArrayBufferView> bufferView)
        : m_bufferView(bufferView)
    {
        ASSERT(m_bufferView);
    }
    DOMArrayBufferView(PassRefPtr<WTF::ArrayBufferView> bufferView, PassRefPtr<DOMArrayBufferBase> domArrayBuffer)
        : m_bufferView(bufferView), m_domArrayBuffer(domArrayBuffer)
    {
        ASSERT(m_bufferView);
        ASSERT(m_domArrayBuffer);
        ASSERT(m_domArrayBuffer->buffer() == m_bufferView->buffer());
    }

private:
    RefPtr<WTF::ArrayBufferView> m_bufferView;
    mutable RefPtr<DOMArrayBufferBase> m_domArrayBuffer;
};

} // namespace blink

#endif // DOMArrayBufferView_h
