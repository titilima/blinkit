// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: local_frame_view.h
// Description: LocalFrameView Class
//      Author: Ziming Li
//     Created: 2020-09-20
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_LOCAL_FRAME_VIEW_H
#define BLINKIT_BLINK_LOCAL_FRAME_VIEW_H

#pragma once

#include "third_party/blink/renderer/core/frame/frame_view.h"
#include "third_party/blink/renderer/platform/geometry/layout_size.h"

namespace blink {

class LocalFrameView final : public FrameView
{
public:
    void IncrementLayoutObjectCount(void) {} // Just a placeholder
};

} // namespace blink

#endif // BLINKIT_BLINK_LOCAL_FRAME_VIEW_H
