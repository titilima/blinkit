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

#include <unordered_map>
#include "bk_crawler.h"
#include "net/cookies/cookie_options.h"
#include "third_party/zed/include/zed/shared_mutex.hpp"

namespace net {
class CanonicalCookie;
}

class CookieJarImpl
{
public:
    CookieJarImpl(void);

    void Retain(void) { ++m_refCount; }
    void Release(void);

    void Clear(void);

    std::string Get(const char *URL, const net::CookieOptions *options = nullptr) const;
    bool Set(const char *setCookieHeader, const char *URL);

    void lock_shared(void) { m_mutex.lock_shared(); }
    void unlock_shared(void) { m_mutex.unlock_shared(); }
    void lock(void) { m_mutex.lock(); }
    void unlock(void) { m_mutex.unlock(); }
private:
    ~CookieJarImpl(void) = default;

    unsigned m_refCount = 1;
    net::CookieOptions m_options;
    zed::shared_mutex m_mutex;

    typedef std::unordered_map<std::string, std::unique_ptr<net::CanonicalCookie>> CookiesMap;
    std::unordered_map<std::string, CookiesMap> m_cookies;
};

#endif // BLINKIT_BLINKIT_COOKIE_JAR_IMPL_H
