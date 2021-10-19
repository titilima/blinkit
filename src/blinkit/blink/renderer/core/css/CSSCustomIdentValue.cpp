// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSCustomIdentValue.cpp
// Description: CSSCustomIdentValue Class
//      Author: Ziming Li
//     Created: 2021-10-19
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/css/CSSCustomIdentValue.h"

#include "core/css/CSSMarkup.h"
#include "wtf/text/WTFString.h"

namespace blink {

CSSCustomIdentValue::CSSCustomIdentValue(const String& str)
    : CSSValue(CustomIdentClass)
    , m_string(str)
    , m_propertyId(CSSPropertyInvalid) { }

CSSCustomIdentValue::CSSCustomIdentValue(CSSPropertyID id)
    : CSSValue(CustomIdentClass)
    , m_string()
    , m_propertyId(id)
{
    ASSERT(isKnownPropertyID());
}

String CSSCustomIdentValue::customCSSText() const
{
    if (isKnownPropertyID())
        return getPropertyNameAtomicString(m_propertyId);
    return quoteCSSStringIfNeeded(m_string);
}

} // namespace blink
