// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSContentDistributionValue.h
// Description: CSSContentDistributionValue Class
//      Author: Ziming Li
//     Created: 2021-10-18
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CSSContentDistributionValue_h
#define CSSContentDistributionValue_h

#include "core/css/CSSValue.h"
#include "core/css/CSSValuePool.h"

namespace blink {

class CSSContentDistributionValue : public CSSValue {
public:
    static GCRefPtr<CSSContentDistributionValue> create(CSSValueID distribution, CSSValueID position, CSSValueID overflow)
    {
        return BlinKit::GCWrapShared(new CSSContentDistributionValue(distribution, position, overflow));
    }
    ~CSSContentDistributionValue();

    GCRefPtr<CSSPrimitiveValue> distribution() const { return cssValuePool().createIdentifierValue(m_distribution); }

    GCRefPtr<CSSPrimitiveValue> position() const { return cssValuePool().createIdentifierValue(m_position); }

    GCRefPtr<CSSPrimitiveValue> overflow() const { return cssValuePool().createIdentifierValue(m_overflow); }

    String customCSSText() const;

    bool equals(const CSSContentDistributionValue&) const;

private:
    CSSContentDistributionValue(CSSValueID distribution, CSSValueID position, CSSValueID overflow);

    CSSValueID m_distribution;
    CSSValueID m_position;
    CSSValueID m_overflow;
};

DEFINE_CSS_VALUE_TYPE_CASTS(CSSContentDistributionValue, isContentDistributionValue());

} // namespace blink

#endif // CSSContentDistributionValue_h
