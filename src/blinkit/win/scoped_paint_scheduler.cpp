// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: scoped_paint_scheduler.cpp
// Description: ScopedPaintScheduler Class
//      Author: Ziming Li
//     Created: 2021-11-29
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./scoped_paint_scheduler.h"

#include "blinkit/win/paint_session.h"
#include "third_party/zed/include/zed/container_utilites.hpp"

namespace BlinKit {

ScopedPaintScheduler* ScopedPaintScheduler::m_current = nullptr;

ScopedPaintScheduler::ScopedPaintScheduler(void)
{
    ASSERT(nullptr == m_current);
    m_current = this;
}

ScopedPaintScheduler::~ScopedPaintScheduler(void)
{
    ASSERT(this == m_current);

    for (auto &[paintSession, host] : m_sessions)
    {
        paintSession->Flush(*host);
        paintSession->m_attached = false;
    }

    m_current = nullptr;
}

void ScopedPaintScheduler::Attach(PaintSession &session, WinWebViewHost &host)
{
    ASSERT(!zed::key_exists(m_sessions, &session));
    m_sessions.emplace(&session, &host);
    session.m_attached = true;
}

void ScopedPaintScheduler::Detach(PaintSession &session)
{
    ASSERT(zed::key_exists(m_sessions, &session));
    m_sessions.erase(&session);
    session.m_attached = false;
}

} // namespace BlinKit
