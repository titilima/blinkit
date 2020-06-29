// -------------------------------------------------
// BlinKit - BkHelper Library
// -------------------------------------------------
//   File Name: proxy_globals.h
// Description: ProxyGlobals Class
//      Author: Ziming Li
//     Created: 2020-06-03
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKHELPER_PROXY_GLOBALS_H
#define BLINKIT_BKHELPER_PROXY_GLOBALS_H

#pragma once

namespace BlinKit {

class ProxyGlobals
{
public:
    ~ProxyGlobals(void) = default;

    static void EnsureSSLInitialized(void);
private:
    ProxyGlobals(void) = default;

    static ProxyGlobals s_singleton;

    bool m_sslInitialized = false;
};

} // namespace BlinKit

#endif // BLINKIT_BKHELPER_PROXY_GLOBALS_H
