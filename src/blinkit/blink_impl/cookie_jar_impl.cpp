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

#include "net/cookies/canonical_cookie.h"
#include "url/gurl.h"

using namespace blink;

typedef std::lock_guard<std::recursive_mutex> AutoLock;

namespace BlinKit {

CookieJarImpl::CookieJarImpl(void)
{
    m_options.set_include_httponly();
}

CookieJarImpl::~CookieJarImpl(void)
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
    AutoLock l(m_lock);

    GURL u(URL);
    std::string ret;
    for (const auto &cookie : m_cookies)
    {
        if (!cookie->IncludeForRequestURL(u, m_options))
            continue;

        if (!ret.empty())
            ret.append("; ");
        ret.append(cookie->Name());
        ret.push_back('=');
        ret.append(cookie->Value());
    }

    return ret;
}

void CookieJarImpl::setCookie(const WebURL &URL, const WebURL &firstPartyForCookies, const WebString &cookie)
{
    assert(false); // BKTODO:
}

} // namespace BlinKit
