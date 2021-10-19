// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSReflectValue.h
// Description: CSSReflectValue Class
//      Author: Ziming Li
//     Created: 2021-10-19
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2008 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef CSSReflectValue_h
#define CSSReflectValue_h

#include "core/css/CSSValue.h"

namespace blink {

class CSSPrimitiveValue;

class CSSReflectValue : public CSSValue {
public:
    static GCRefPtr<CSSReflectValue> create(const GCRefPtr<CSSPrimitiveValue> &direction,
        const GCRefPtr<CSSPrimitiveValue> &offset, const GCRefPtr<CSSValue> &mask)
    {
        return BlinKit::GCWrapShared(new CSSReflectValue(direction, offset, mask));
    }

    CSSPrimitiveValue* direction() const { return m_direction.get(); }
    CSSPrimitiveValue* offset() const { return m_offset.get(); }
    CSSValue* mask() const { return m_mask.get(); }

    String customCSSText() const;

    bool equals(const CSSReflectValue&) const;

    DECLARE_TRACE_AFTER_DISPATCH();

private:
    CSSReflectValue(const GCRefPtr<CSSPrimitiveValue> &direction, const GCRefPtr<CSSPrimitiveValue> &offset, const GCRefPtr<CSSValue> &mask)
        : CSSValue(ReflectClass)
        , m_direction(direction)
        , m_offset(offset)
        , m_mask(mask)
    {
    }

    GCRefPtr<CSSPrimitiveValue> m_direction;
    GCRefPtr<CSSPrimitiveValue> m_offset;
    GCRefPtr<CSSValue> m_mask;
};

DEFINE_CSS_VALUE_TYPE_CASTS(CSSReflectValue, isReflectValue());

} // namespace blink

#endif // CSSReflectValue_h
