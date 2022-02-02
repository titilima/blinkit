// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: scoped_animation_session.cpp
// Description: ScopedAnimationSession Class
//      Author: Ziming Li
//     Created: 2021-12-09
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./scoped_animation_session.h"

#include "blinkit/ui/web_view_host.h"

namespace BlinKit {

struct SessionData {
    unsigned level = 0;
    bool fullPaint = false;
};

static std::unordered_map<WebViewHost *, SessionData> g_sessions;

ScopedAnimationSession::ScopedAnimationSession(WebViewHost &host) : m_host(&host)
{
    SessionData &data = g_sessions[m_host];
    if (1 == ++data.level)
        m_host->EnterAnimationSession();
}

ScopedAnimationSession::~ScopedAnimationSession(void)
{
    auto it = g_sessions.find(m_host);
    ASSERT(g_sessions.end() != it);

    SessionData &data = it->second;
    if (0 == --data.level)
    {
        bool fullPaint = data.fullPaint;
        g_sessions.erase(it);
        m_host->LeaveAnimationSession(fullPaint);
    }
}

void ScopedAnimationSession::SetFullPaint(void)
{
    auto it = g_sessions.find(m_host);
    ASSERT(g_sessions.end() != it);
    it->second.fullPaint = true;
}

}
