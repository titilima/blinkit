// -------------------------------------------------
// BlinKit - BkHelper Library
// -------------------------------------------------
//   File Name: proxy_globals.cpp
// Description: ProxyGlobals Class
//      Author: Ziming Li
//     Created: 2020-06-03
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "proxy_globals.h"

#include <openssl/ssl.h>

namespace BlinKit {

ProxyGlobals ProxyGlobals::s_singleton;

void ProxyGlobals::EnsureSSLInitialized(void)
{
    if (!s_singleton.m_sslInitialized)
    {
        SSL_library_init();
        OpenSSL_add_all_algorithms();
        SSL_load_error_strings();
        s_singleton.m_sslInitialized = true;
    }
}

} // namespace BlinKit
