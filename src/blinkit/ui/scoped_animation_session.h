#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: scoped_animation_session.h
// Description: ScopedAnimationSession Class
//      Author: Ziming Li
//     Created: 2021-12-09
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SCOPED_ANIMATION_SESSION_H
#define BLINKIT_SCOPED_ANIMATION_SESSION_H

namespace BlinKit {

class WebViewHost;

class ScopedAnimationSession
{
public:
    ScopedAnimationSession(WebViewHost &host);
    ~ScopedAnimationSession(void);
private:
    WebViewHost *m_host;
};

} // namespace BlinKit

#endif // BLINKIT_SCOPED_ANIMATION_SESSION_H
