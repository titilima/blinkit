// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSPathValue.h
// Description: CSSPathValue Class
//      Author: Ziming Li
//     Created: 2021-07-19
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CSSPathValue_h
#define CSSPathValue_h

#include "core/css/CSSValue.h"
// BKTODO: #include "core/svg/SVGPathByteStream.h"
#include "wtf/PassRefPtr.h"
#include "wtf/RefPtr.h"

namespace blink {

class StylePath;

class CSSPathValue : public CSSValue {
public:
    // BKTODO: static PassRefPtrWillBeRawPtr<CSSPathValue> create(PassRefPtr<SVGPathByteStream>, StylePath* = nullptr);
    static PassRefPtrWillBeRawPtr<CSSPathValue> create(const String&);
    ~CSSPathValue();

    static CSSPathValue* emptyPathValue();

    StylePath* cachedPath();
    String customCSSText() const;

    bool equals(const CSSPathValue&) const;

    DECLARE_TRACE_AFTER_DISPATCH();

    // BKTODO: const SVGPathByteStream& byteStream() const { return *m_pathByteStream; }
    String pathString() const;

private:
#if 0 // BKTODO:
    CSSPathValue(PassRefPtr<SVGPathByteStream>, StylePath*);

    RefPtr<SVGPathByteStream> m_pathByteStream;
#endif
    RefPtr<StylePath> m_cachedPath;
};

DEFINE_CSS_VALUE_TYPE_CASTS(CSSPathValue, isPathValue());

} // namespace blink

#endif // CSSPathValue_h
