// -------------------------------------------------
// BlinKit - BkHelper Library
// -------------------------------------------------
//   File Name: login_globals.h
// Description: LoginGlobals Class
//      Author: Ziming Li
//     Created: 2020-06-03
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKHELPER_LOGIN_GLOBALS_H
#define BLINKIT_BKHELPER_LOGIN_GLOBALS_H

#pragma once

namespace BlinKit {

class LoginGlobals
{
public:
    ~LoginGlobals(void);

    static void EnsureSSLInitialized(void);
private:
    LoginGlobals(void) = default;

    static LoginGlobals s_singleton;

    bool m_sslInitialized = false;
};

} // namespace BlinKit

#endif // BLINKIT_BKHELPER_LOGIN_GLOBALS_H
