// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSCustomIdentValue.h
// Description: CSSCustomIdentValue Class
//      Author: Ziming Li
//     Created: 2021-10-19
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CSSCustomIdentValue_h
#define CSSCustomIdentValue_h

#include "core/CSSPropertyNames.h"
#include "core/css/CSSValue.h"

namespace blink {

class CSSCustomIdentValue : public CSSValue {
public:
    static GCRefPtr<CSSCustomIdentValue> create(const String& str)
    {
        return BlinKit::GCWrapShared(new CSSCustomIdentValue(str));
    }

    // TODO(sashab, timloh): Remove this and lazily parse the CSSPropertyID in isKnownPropertyID().
    static GCRefPtr<CSSCustomIdentValue> create(CSSPropertyID id)
    {
        return BlinKit::GCWrapShared(new CSSCustomIdentValue(id));
    }

    String value() const { ASSERT(!isKnownPropertyID()); return m_string; }
    bool isKnownPropertyID() const { return m_propertyId != CSSPropertyInvalid; }
    CSSPropertyID valueAsPropertyID() const { ASSERT(isKnownPropertyID()); return m_propertyId; }

    String customCSSText() const;

    bool equals(const CSSCustomIdentValue& other) const
    {
        return isKnownPropertyID() ? m_propertyId == other.m_propertyId : m_string == other.m_string;
    }

private:
    CSSCustomIdentValue(const String&);
    CSSCustomIdentValue(CSSPropertyID);

    // TODO(sashab): Change this to an AtomicString.
    String m_string;
    CSSPropertyID m_propertyId;
};

DEFINE_CSS_VALUE_TYPE_CASTS(CSSCustomIdentValue, isCustomIdentValue());

} // namespace blink

#endif // CSSCustomIdentValue_h
