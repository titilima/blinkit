// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: document_load_timing.h
// Description: DocumentLoadTiming Class
//      Author: Ziming Li
//     Created: 2020-12-04
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DOCUMENT_LOAD_TIMING_H
#define BLINKIT_BLINK_DOCUMENT_LOAD_TIMING_H

#pragma once

#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/time.h"

namespace blink {

class DocumentLoadTiming final
{
    DISALLOW_NEW();
public:
    explicit DocumentLoadTiming(void) = default;

    TimeTicks ReferenceMonotonicTime(void) const { return m_referenceMonotonicTime; }

    void SetNavigationStart(TimeTicks navigationStart);
private:
    void EnsureReferenceTimesSet(void);

    TimeTicks m_referenceMonotonicTime;
};

} // namespace blink

#endif // BLINKIT_BLINK_DOCUMENT_LOAD_TIMING_H
