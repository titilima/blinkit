// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSColorValue.h
// Description: CSSColorValue Class
//      Author: Ziming Li
//     Created: 2021-10-18
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CSSColorValue_h
#define CSSColorValue_h

#include "core/css/CSSValue.h"
#include "platform/graphics/Color.h"

namespace blink {

// Represents the non-keyword subset of <color>.
class CSSColorValue : public CSSValue {
public:
    static GCRefPtr<CSSColorValue> create(Color color)
    {
        return BlinKit::GCWrapShared(new CSSColorValue(color));
    }

    String customCSSText() const
    {
        return m_color.serializedAsCSSComponentValue();
    }

    Color value() const { return m_color; }

    bool equals(const CSSColorValue& other) const
    {
        return m_color == other.m_color;
    }

private:
    CSSColorValue(Color color)
        : CSSValue(ColorClass)
        , m_color(color) { }

    Color m_color;
};

DEFINE_CSS_VALUE_TYPE_CASTS(CSSColorValue, isColorValue());

} // namespace blink

#endif // CSSColorValue_h
