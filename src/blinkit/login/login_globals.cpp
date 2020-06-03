// -------------------------------------------------
// BlinKit - BkLogin Library
// -------------------------------------------------
//   File Name: login_globals.cpp
// Description: LoginGlobals Class
//      Author: Ziming Li
//     Created: 2020-06-03
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "login_globals.h"

#include <openssl/ssl.h>

namespace BlinKit {

LoginGlobals LoginGlobals::s_singleton;

LoginGlobals::~LoginGlobals(void)
{
}

void LoginGlobals::EnsureSSLInitialized(void)
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
