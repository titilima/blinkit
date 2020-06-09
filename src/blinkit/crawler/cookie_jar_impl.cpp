// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: cookie_jar_impl.cpp
// Description: CookieJarImpl Class
//      Author: Ziming Li
//     Created: 2020-06-08
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "cookie_jar_impl.h"

#include "net/cookies/canonical_cookie.h"

using namespace net;

CookieJarImpl::CookieJarImpl(void)
{
    m_options.set_include_httponly();
}

CookieJarImpl::~CookieJarImpl(void) = default;

std::string CookieJarImpl::Get(const char *URL) const
{
    std::string ret;

    GURL u(URL);
    for (const auto &cookie : m_cookies)
    {
        if (!cookie->IncludeForRequestURL(u, m_options))
            continue;

        ret.append(cookie->Name());
        ret.push_back('=');
        ret.append(cookie->Value());
        ret.append("; ");
    }

    if (!ret.empty())
    {
        size_t s = ret.length();
        ret.resize(s - 2); // For the last "; "
    }
    return ret;
}

bool CookieJarImpl::Set(const char *setCookieHeader, const char *URL)
{
    CanonicalCookie *c = CanonicalCookie::Create(GURL(URL), setCookieHeader, base::Time::Now(), m_options);
    if (nullptr != c)
    {
        m_cookies.push_back(std::unique_ptr<CanonicalCookie>(c));
        return true;
    }
    BKLOG("Parse cookie failed! Cookie line: %s", setCookieHeader);
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT BkCookieJar BKAPI BkCreateCookieJar(void)
{
    return new CookieJarImpl;
}

BKEXPORT void BKAPI BkDestroyCookieJar(BkCookieJar cookieJar)
{
    delete cookieJar;
}

BKEXPORT void BKAPI BkGetCookie(BkCookieJar cookieJar, const char *URL, BkBuffer *dst)
{
    std::string ret = cookieJar->Get(URL);
    if (!ret.empty())
        BkSetBufferData(dst, ret.data(), ret.length());
}

BKEXPORT bool BKAPI BkSetCookie(BkCookieJar cookieJar, const char *setCookieHeader, const char *URL)
{
    return cookieJar->Set(setCookieHeader, URL);
}

} // extern "C"
