// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: document_load_timing.cpp
// Description: DocumentLoadTiming Class
//      Author: Ziming Li
//     Created: 2020-12-04
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "document_load_timing.h"

namespace blink {

void DocumentLoadTiming::EnsureReferenceTimesSet(void)
{
    if (m_referenceMonotonicTime.is_null())
        m_referenceMonotonicTime = CurrentTimeTicks();
}

void DocumentLoadTiming::SetNavigationStart(TimeTicks navigationStart)
{
    // |m_referenceMonotonicTime| and |m_referenceWallTime| represent
    // navigationStart. We must set these to the current time if they haven't
    // been set yet in order to have a valid reference time in both units.
    EnsureReferenceTimesSet();

    // The reference times are adjusted based on the embedder's navigationStart.
    ASSERT(!m_referenceMonotonicTime.is_null());
    m_referenceMonotonicTime = navigationStart;
}

} // namespace blink
