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

#include "public/platform/WebURL.h"

using namespace blink;
using namespace net;

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

void CookieJarImpl::AddCookieEntry(const std::string &URL, const std::string &cookie)
{
    AutoLock lock(m_lock);

    CanonicalCookie *c = CanonicalCookie::Create(GURL(URL), cookie, base::Time::Now(), m_options);
    if (nullptr != c)
        m_cookies.push_back(std::unique_ptr<CanonicalCookie>(c));
    else
        BKLOG("Parse cookie failed! Cookie line: %s", cookie.c_str());
}

WebString CookieJarImpl::cookieRequestHeaderFieldValue(const WebURL &URL, const WebURL &firstPartyForCookies)
{
    assert(false); // BKTODO:
    return WebString();
}

WebString CookieJarImpl::cookies(const WebURL &URL, const WebURL &firstPartyForCookies)
{
    std::string cookies = GetCookies(URL.string().utf8());
    return WebString::fromUTF8(cookies);
}

std::string CookieJarImpl::GetCookies(const std::string &URL) const
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
    AddCookieEntry(URL.string().utf8(), cookie.utf8());
}

} // namespace BlinKit
