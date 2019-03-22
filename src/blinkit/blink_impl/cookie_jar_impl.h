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

namespace BlinKit {

class CookieJarImpl final : public blink::WebCookieJar
{
public:
    CookieJarImpl(void);

    std::string GetCookie(const std::string &URL) const;
private:
    // blink::WebCookieJar
    void setCookie(const blink::WebURL&, const blink::WebURL& firstPartyForCookies, const blink::WebString& cookie) override;
    blink::WebString cookies(const blink::WebURL&, const blink::WebURL& firstPartyForCookies) override;
    blink::WebString cookieRequestHeaderFieldValue(const blink::WebURL&, const blink::WebURL& firstPartyForCookies) override;

    mutable std::recursive_mutex m_lock;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_COOKIE_JAR_IMPL_H
