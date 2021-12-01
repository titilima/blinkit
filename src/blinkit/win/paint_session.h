#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: paint_session.h
// Description: PaintSession Class
//      Author: Ziming Li
//     Created: 2021-11-23
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_PAINT_SESSION_H
#define BLINKIT_PAINT_SESSION_H

#include "blinkit/blink/renderer/platform/geometry/IntRect.h"

namespace BlinKit {

class WebViewHostWindow;

class PaintSession
{
public:
    ~PaintSession(void);

    void Begin(WebViewHostWindow &host);
    void Flush(WebViewHostWindow &host);

    void AttachToScheduler(WebViewHostWindow &host);
    void DetachFromScheduler(void);
    bool IsAttached(void) const { return m_attached; }

    void UniteDamagedRect(const blink::IntRect &rect) {
        m_damagedRect.unite(rect);
    }
private:
    friend class ScopedPaintScheduler;

    bool m_attached = false;

    bool m_beginFrameCalled = false;
    blink::IntRect m_damagedRect;
};

} // namespace BlinKit

#endif // BLINKIT_PAINT_SESSION_H
