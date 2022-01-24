#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ScrollRecorder.h
// Description: ScrollRecorder Class
//      Author: Ziming Li
//     Created: 2022-01-23
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ScrollRecorder_h
#define ScrollRecorder_h

#include "blinkit/blink/renderer/core/paint/PaintPhase.h"
#include "blinkit/blink/renderer/platform/geometry/int_size.h"
#include "blinkit/blink/renderer/platform/graphics/paint/DisplayItem.h"

namespace blink {

class GraphicsContext;

// Emits display items which represent a region which is scrollable, so that it
// can be translated by the scroll offset.
class ScrollRecorder
{
public:
    ScrollRecorder(GraphicsContext&, const DisplayItemClient&, PaintPhase, const IntSize& currentOffset);
    ~ScrollRecorder();
private:
    const DisplayItemClient& m_client;
    DisplayItem::Type m_beginItemType;
    GraphicsContext& m_context;
};

} // namespace blink

#endif // ScrollRecorder_h
