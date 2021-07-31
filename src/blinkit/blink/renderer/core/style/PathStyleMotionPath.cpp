// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: PathStyleMotionPath.cpp
// Description: PathStyleMotionPath Class
//      Author: Ziming Li
//     Created: 2021-07-31
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/style/PathStyleMotionPath.h"

// BKTODO: #include "core/svg/SVGPathUtilities.h"

namespace blink {

PathStyleMotionPath::PathStyleMotionPath(const String& pathString)
    : m_pathString(pathString.stripWhiteSpace())
{
    ASSERT(false); // BKTODO: buildPathFromString(pathString, m_path);
    m_length = m_path.length();
}

bool PathStyleMotionPath::isClosed() const
{
    return m_pathString.endsWith("Z", TextCaseInsensitive);
}

} // namespace blink
