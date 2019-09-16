// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: dom_window.cpp
// Description: DOMWindow Class
//      Author: Ziming Li
//     Created: 2019-09-16
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "dom_window.h"

namespace blink {

DOMWindow::DOMWindow(Frame &frame) : m_frame(&frame)
{
}

DOMWindow::~DOMWindow(void)
{
    // The frame must be disconnected before finalization.
    assert(!m_frame);
}

}  // namespace blink
