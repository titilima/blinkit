// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: PathStyleMotionPath.cpp
// Description: PathStyleMotionPath Class
//      Author: Ziming Li
//     Created: 2019-03-31
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/style/PathStyleMotionPath.h"

#include "core/svg/SVGPathUtilities.h"

namespace blink {

PathStyleMotionPath::PathStyleMotionPath(const String& pathString)
    : m_pathString(pathString.stripWhiteSpace())
{
#ifdef BLINKIT_CRAWLER_ONLY
    assert(false); // BKTODO: Not reached!
#else
    buildPathFromString(pathString, m_path);
    m_length = m_path.length();
#endif
}

bool PathStyleMotionPath::isClosed() const
{
    return m_pathString.endsWith("Z", TextCaseInsensitive);
}

} // namespace blink
