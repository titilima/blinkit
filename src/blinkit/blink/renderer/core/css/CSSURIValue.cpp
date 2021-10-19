// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSURIValue.cpp
// Description: CSSURIValue Class
//      Author: Ziming Li
//     Created: 2021-10-19
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/css/CSSURIValue.h"

#include "core/css/CSSMarkup.h"
#include "wtf/text/WTFString.h"

namespace blink {

CSSURIValue::CSSURIValue(const String& str)
    : CSSValue(URIClass)
    , m_string(str) { }

String CSSURIValue::customCSSText() const
{
    return serializeURI(m_string);
}

} // namespace blink
