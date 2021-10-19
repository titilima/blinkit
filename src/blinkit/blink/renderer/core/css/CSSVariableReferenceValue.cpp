// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSVariableReferenceValue.cpp
// Description: CSSVariableReferenceValue Class
//      Author: Ziming Li
//     Created: 2021-10-19
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/css/CSSVariableReferenceValue.h"

namespace blink {

String CSSVariableReferenceValue::customCSSText() const
{
    // We may want to consider caching this value.
    return m_data->tokenRange().serialize();
}

} // namespace blink
