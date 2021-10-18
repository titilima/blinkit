// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSStringValue.h
// Description: CSSStringValue Class
//      Author: Ziming Li
//     Created: 2021-10-18
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CSSStringValue_h
#define CSSStringValue_h

#include "core/css/CSSValue.h"

namespace blink {

class CSSStringValue : public CSSValue {
public:
    static GCRefPtr<CSSStringValue> create(const String& str)
    {
        return BlinKit::GCWrapShared(new CSSStringValue(str));
    }

    String value() const { return m_string; }

    String customCSSText() const;

    bool equals(const CSSStringValue& other) const
    {
        return m_string == other.m_string;
    }

private:
    CSSStringValue(const String&);

    String m_string;
};

DEFINE_CSS_VALUE_TYPE_CASTS(CSSStringValue, isStringValue());

} // namespace blink

#endif // CSSStringValue_h
