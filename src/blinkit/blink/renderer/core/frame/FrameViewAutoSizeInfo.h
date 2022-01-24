#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: FrameViewAutoSizeInfo.h
// Description: FrameViewAutoSizeInfo Class
//      Author: Ziming Li
//     Created: 2022-01-23
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FrameViewAutoSizeInfo_h
#define FrameViewAutoSizeInfo_h

#include "blinkit/blink/renderer/platform/geometry/int_size.h"
#include "blinkit/blink/renderer/platform/heap/Handle.h"
#include "blinkit/blink/renderer/wtf/Noncopyable.h"
#include "blinkit/blink/renderer/wtf/RefPtr.h"

namespace blink {

class FrameView;

class FrameViewAutoSizeInfo final : public NoBaseWillBeGarbageCollected<FrameViewAutoSizeInfo>
{
    WTF_MAKE_NONCOPYABLE(FrameViewAutoSizeInfo);
    USING_FAST_MALLOC_WILL_BE_REMOVED(FrameViewAutoSizeInfo);
    DECLARE_EMPTY_DESTRUCTOR_WILL_BE_REMOVED(FrameViewAutoSizeInfo);
public:
    static PassOwnPtrWillBeRawPtr<FrameViewAutoSizeInfo> create(FrameView* frameView)
    {
        return adoptPtrWillBeNoop(new FrameViewAutoSizeInfo(frameView));
    }

    void configureAutoSizeMode(const IntSize& minSize, const IntSize& maxSize);
    void autoSizeIfNeeded();

    DECLARE_TRACE();

private:
    explicit FrameViewAutoSizeInfo(FrameView*);

    RawPtrWillBeMember<FrameView> m_frameView;

    // The lower bound on the size when autosizing.
    IntSize m_minAutoSize;
    // The upper bound on the size when autosizing.
    IntSize m_maxAutoSize;

    bool m_inAutoSize;
    // True if autosize has been run since m_shouldAutoSize was set.
    bool m_didRunAutosize;
};

} // namespace blink

#endif // FrameViewAutoSizeInfo_h
