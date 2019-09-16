// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: frame_load_request.cpp
// Description: FrameLoadRequest Class
//      Author: Ziming Li
//     Created: 2019-09-14
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "frame_load_request.h"

namespace blink {

FrameLoadRequest::FrameLoadRequest(Document *originDocument, const ResourceRequest &resourceRequest)
    : m_originDocument(originDocument), m_resourceRequest(resourceRequest)
{
}

}  // namespace blink
