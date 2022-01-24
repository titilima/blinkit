// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: PaintArtifact.cpp
// Description: PaintArtifact Class
//      Author: Ziming Li
//     Created: 2022-01-23
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "./PaintArtifact.h"

#include "blinkit/blink/renderer/platform/TraceEvent.h"
#include "blinkit/blink/renderer/platform/geometry/int_rect.h"

namespace blink {

PaintArtifact::PaintArtifact()
    : m_displayItemList(0)
{
}

PaintArtifact::~PaintArtifact()
{
}

void PaintArtifact::reset()
{
    m_displayItemList.clear();
    m_paintChunks.clear();
}

size_t PaintArtifact::approximateUnsharedMemoryUsage() const
{
    return sizeof(*this) + m_displayItemList.memoryUsageInBytes()
        + m_paintChunks.capacity() * sizeof(m_paintChunks[0]);
}

void PaintArtifact::replay(GraphicsContext& graphicsContext) const
{
    TRACE_EVENT0("blink,benchmark", "PaintArtifact::replay");
    for (const DisplayItem& displayItem : m_displayItemList)
        displayItem.replay(graphicsContext);
}

void PaintArtifact::appendToWebDisplayItemList(WebDisplayItemList* list) const
{
    TRACE_EVENT0("blink,benchmark", "PaintArtifact::appendToWebDisplayItemList");
#if ENABLE(ASSERT)
    m_displayItemList.assertDisplayItemClientsAreAlive();
#endif
    for (const DisplayItem& displayItem : m_displayItemList) {
        // TODO(wkorman): Pass the actual visual rect for the display item.
        displayItem.appendToWebDisplayItemList(IntRect(), list);
    }
}

} // namespace blink
