// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSUnsetValue.h
// Description: CSSUnsetValue Class
//      Author: Ziming Li
//     Created: 2021-10-20
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CSSUnsetValue_h
#define CSSUnsetValue_h

#include "core/css/CSSValue.h"

namespace blink {

class CSSUnsetValue : public CSSValue {
public:
    static GCRefPtr<CSSUnsetValue> create()
    {
        return BlinKit::GCWrapShared(new CSSUnsetValue);
    }

    String customCSSText() const;

    bool equals(const CSSUnsetValue&) const { return true; }

private:
    CSSUnsetValue()
        : CSSValue(UnsetClass)
    {
    }
};

DEFINE_CSS_VALUE_TYPE_CASTS(CSSUnsetValue, isUnsetValue());

} // namespace blink

#endif // CSSUnsetValue_h
