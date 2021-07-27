// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: StylePath.cpp
// Description: StylePath Class
//      Author: Ziming Li
//     Created: 2021-07-27
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/style/StylePath.h"

#include "core/css/CSSPathValue.h"
#if 0 // BKTODO:
#include "core/svg/SVGPathByteStream.h"
#include "core/svg/SVGPathUtilities.h"
#endif

namespace blink {

#if 0 // BKTODO:
StylePath::StylePath(PassRefPtr<SVGPathByteStream> pathByteStream)
    : m_byteStream(pathByteStream)
{
    ASSERT(m_byteStream);
    buildPathFromByteStream(*m_byteStream, m_path);
}
#endif

StylePath::~StylePath()
{
}

#if 0 // BKTODO:
PassRefPtr<StylePath> StylePath::create(PassRefPtr<SVGPathByteStream> pathByteStream)
{
    return adoptRef(new StylePath(pathByteStream));
}
#endif

StylePath* StylePath::emptyPath()
{
    ASSERT(false); // BKTODO:
    return nullptr;
#if 0
    DEFINE_STATIC_REF(StylePath, emptyPath, StylePath::create(SVGPathByteStream::create()));
    return emptyPath;
#endif
}

#if 0 // BKTODO:
const SVGPathByteStream& StylePath::byteStream() const
{
    return *m_byteStream;
}
#endif

PassRefPtrWillBeRawPtr<CSSValue> StylePath::computedCSSValue() const
{
    ASSERT(false); // BKTODO: return CSSPathValue::create(m_byteStream, const_cast<StylePath*>(this));
    return nullptr;
}

bool StylePath::equals(const StylePath& other) const
{
    ASSERT(false); // BKTODO: return *m_byteStream == *other.m_byteStream;
    return false;
}

} // namespace blink
