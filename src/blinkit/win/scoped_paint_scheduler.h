#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: scoped_paint_scheduler.h
// Description: ScopedPaintScheduler Class
//      Author: Ziming Li
//     Created: 2021-11-29
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SCOPED_PAINT_SCHEDULER_H
#define BLINKIT_SCOPED_PAINT_SCHEDULER_H

#include "blinkit/blink/renderer/wtf/Allocator.h"

namespace BlinKit {

class PaintSession;
class WinWebViewHost;

class ScopedPaintScheduler
{
    STACK_ALLOCATED();
public:
    static ScopedPaintScheduler* Current(void) { return m_current; }

    void Attach(PaintSession &session, WinWebViewHost &host);
    void Detach(PaintSession &session);
private:
    friend class MessageLoop;
    ScopedPaintScheduler(void);
    ~ScopedPaintScheduler(void);

    static ScopedPaintScheduler *m_current;
    std::unordered_map<PaintSession *, WinWebViewHost *> m_sessions;
};

} // namespace BlinKit

#endif // BLINKIT_SCOPED_PAINT_SCHEDULER_H
