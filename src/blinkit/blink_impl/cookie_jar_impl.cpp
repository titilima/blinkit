// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: cookie_jar_impl.cpp
// Description: CookieJarImpl Class
//      Author: Ziming Li
//     Created: 2019-03-22
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "cookie_jar_impl.h"

using namespace blink;

namespace BlinKit {

CookieJarImpl::CookieJarImpl(void)
{
    // Nothing
}

WebString CookieJarImpl::cookieRequestHeaderFieldValue(const WebURL &URL, const WebURL &firstPartyForCookies)
{
    assert(false); // BKTODO:
    return WebString();
}

WebString CookieJarImpl::cookies(const WebURL &URL, const WebURL &firstPartyForCookies)
{
    assert(false); // BKTODO:
    return WebString();
}

std::string CookieJarImpl::GetCookie(const std::string &URL) const
{
    assert(false); // BKTODO:
    return std::string();
}

void CookieJarImpl::setCookie(const WebURL &URL, const WebURL &firstPartyForCookies, const WebString &cookie)
{
    assert(false); // BKTODO:
}

} // namespace BlinKit
