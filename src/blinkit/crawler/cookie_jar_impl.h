// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: cookie_jar_impl.h
// Description: CookieJarImpl Class
//      Author: Ziming Li
//     Created: 2020-06-08
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_COOKIE_JAR_IMPL_H
#define BLINKIT_BLINKIT_COOKIE_JAR_IMPL_H

#pragma once

#include "bk_crawler.h"
#include "net/cookies/cookie_options.h"

namespace net {
class CanonicalCookie;
}

class CookieJarImpl
{
public:
    CookieJarImpl(void);
    ~CookieJarImpl(void);

    void Clear(void);

    std::string Get(const char *URL) const;
    bool Set(const char *setCookieHeader, const char *URL);
private:
    net::CookieOptions m_options;
    std::vector<std::unique_ptr<net::CanonicalCookie>> m_cookies;
};

#endif // BLINKIT_BLINKIT_COOKIE_JAR_IMPL_H
