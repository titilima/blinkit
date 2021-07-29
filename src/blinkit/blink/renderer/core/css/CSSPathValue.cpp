// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSPathValue.cpp
// Description: CSSPathValue Class
//      Author: Ziming Li
//     Created: 2021-07-29
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/css/CSSPathValue.h"

#include "core/style/StylePath.h"
// BKTODO: #include "core/svg/SVGPathUtilities.h"

namespace blink {

#if 0 // BKTODO:
PassRefPtrWillBeRawPtr<CSSPathValue> CSSPathValue::create(PassRefPtr<SVGPathByteStream> pathByteStream, StylePath* cachedPath)
{
    return adoptRefWillBeNoop(new CSSPathValue(pathByteStream, cachedPath));
}
#endif

PassRefPtrWillBeRawPtr<CSSPathValue> CSSPathValue::create(const String& pathString)
{
    ASSERT(false); // BKTODO:
    return nullptr;
#if 0
    RefPtr<SVGPathByteStream> byteStream = SVGPathByteStream::create();
    buildByteStreamFromString(pathString, *byteStream);
    return CSSPathValue::create(byteStream.release());
#endif
}

#if 0 // BKTODO:
CSSPathValue::CSSPathValue(PassRefPtr<SVGPathByteStream> pathByteStream, StylePath* cachedPath)
    : CSSValue(PathClass)
    , m_pathByteStream(pathByteStream)
    , m_cachedPath(cachedPath)
{
    ASSERT(m_pathByteStream);
}
#endif

CSSPathValue::~CSSPathValue()
{
}

namespace {

PassRefPtrWillBeRawPtr<CSSPathValue> createPathValue()
{
    ASSERT(false); // BKTODO:
    return nullptr;
#if 0
    RefPtr<SVGPathByteStream> pathByteStream = SVGPathByteStream::create();
    // Need to be registered as LSan ignored, as it will be reachable and
    // separately referred to by emptyPathValue() callers.
    LEAK_SANITIZER_IGNORE_OBJECT(pathByteStream.get());
    return CSSPathValue::create(pathByteStream.release());
#endif
}

}

CSSPathValue* CSSPathValue::emptyPathValue()
{
    DEFINE_STATIC_LOCAL(RefPtrWillBePersistent<CSSPathValue>, empty, (createPathValue()));
    return empty.get();
}

StylePath* CSSPathValue::cachedPath()
{
    if (!m_cachedPath)
        ASSERT(false); // BKTODO: m_cachedPath = StylePath::create(m_pathByteStream);
    return m_cachedPath.get();
}

String CSSPathValue::customCSSText() const
{
    return "path('" + pathString() + "')";
}

bool CSSPathValue::equals(const CSSPathValue& other) const
{
    ASSERT(false); // BKTODO: return *m_pathByteStream == *other.m_pathByteStream;
    return false;
}

DEFINE_TRACE_AFTER_DISPATCH(CSSPathValue)
{
    CSSValue::traceAfterDispatch(visitor);
}

String CSSPathValue::pathString() const
{
    ASSERT(false); // BKTODO: return buildStringFromByteStream(*m_pathByteStream);
    return String();
}

} // namespace blink
