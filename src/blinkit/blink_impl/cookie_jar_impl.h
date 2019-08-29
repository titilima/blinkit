// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: cookie_jar_impl.h
// Description: CookieJarImpl Class
//      Author: Ziming Li
//     Created: 2019-03-22
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_COOKIE_JAR_IMPL_H
#define BLINKIT_BLINKIT_COOKIE_JAR_IMPL_H

#pragma once

#include <mutex>
#include "public/platform/WebCookieJar.h"
#include "net/cookies/cookie_options.h"

namespace net {
class CanonicalCookie;
}

namespace BlinKit {

class CookieJarImpl final : public blink::WebCookieJar
{
public:
    CookieJarImpl(void);
    ~CookieJarImpl(void);

    void AddCookieEntry(const std::string &URL, const std::string &cookie);
    std::string GetCookie(const std::string &URL) const;
private:
    // blink::WebCookieJar
    void setCookie(const blink::WebURL&, const blink::WebURL& firstPartyForCookies, const blink::WebString& cookie) override;
    blink::WebString cookies(const blink::WebURL&, const blink::WebURL& firstPartyForCookies) override;
    blink::WebString cookieRequestHeaderFieldValue(const blink::WebURL&, const blink::WebURL& firstPartyForCookies) override;

    mutable std::recursive_mutex m_lock;
    net::CookieOptions m_options;
    std::vector<std::unique_ptr<net::CanonicalCookie>> m_cookies;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_COOKIE_JAR_IMPL_H
