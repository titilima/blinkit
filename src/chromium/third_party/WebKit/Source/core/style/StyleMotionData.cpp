// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: StyleMotionData.cpp
// Description: StyleMotionData Class
//      Author: Ziming Li
//     Created: 2019-05-24
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/style/StyleMotionData.h"

#include "core/style/PathStyleMotionPath.h"

namespace blink {

bool StyleMotionData::operator==(const StyleMotionData& o) const
{
    if (m_offset != o.m_offset || m_rotation != o.m_rotation || m_rotationType != o.m_rotationType)
        return false;

    if (!m_path || !o.m_path)
        return !m_path && !o.m_path;

    ASSERT(false); // BKTODO:
#if 0
    if (m_path->isPathStyleMotionPath() && o.m_path->isPathStyleMotionPath())
        return toPathStyleMotionPath(*m_path).equals(toPathStyleMotionPath(*o.m_path));
#endif

    return false;
}

} // namespace blink
