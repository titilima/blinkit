// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSContentDistributionValue.cpp
// Description: CSSContentDistributionValue Class
//      Author: Ziming Li
//     Created: 2021-10-18
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/css/CSSContentDistributionValue.h"

#include "core/css/CSSValueList.h"
#include "wtf/text/StringBuilder.h"

namespace blink {

CSSContentDistributionValue::CSSContentDistributionValue(CSSValueID distribution, CSSValueID position, CSSValueID overflow)
    : CSSValue(CSSContentDistributionClass)
    , m_distribution(distribution)
    , m_position(position)
    , m_overflow(overflow)
{
}

CSSContentDistributionValue::~CSSContentDistributionValue()
{
}

String CSSContentDistributionValue::customCSSText() const
{
    GCRefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();

    if (m_distribution != CSSValueInvalid)
        list->append(distribution());
    if (m_position != CSSValueInvalid)
        list->append(position());
    if (m_overflow != CSSValueInvalid)
        list->append(overflow());

    return list->customCSSText();
}

bool CSSContentDistributionValue::equals(const CSSContentDistributionValue& other) const
{
    return m_distribution == other.m_distribution && m_position == other.m_position && m_overflow == other.m_overflow;
}

}
