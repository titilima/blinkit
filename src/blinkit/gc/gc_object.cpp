// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: gc_object.cpp
// Description: GCObject Class
//      Author: Ziming Li
//     Created: 2021-08-10
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./gc_def.h"

#include "blinkit/blink/renderer/wtf/MainThread.h"

namespace BlinKit {

unsigned GCObject::DecRef(void)
{
    ASSERT(isMainThread());
    return --m_refCnt;
}

void GCObject::IncRef(void)
{
    ASSERT(isMainThread());
    ++m_refCnt;
}

void GCObject::Release(void)
{
    if (0 == DecRef())
        ASSERT(false); // BKTODO: Perform GC.
}

} // namespace BlinKit
