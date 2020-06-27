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

void CookieJarImpl::Clear(void)
{
    m_cookies.clear();
}

static bool DomainMatch(const std::string &domainInURL, const std::string &domainInCookie)
{
    if (domainInCookie.empty())
        return false;
    if (domainInCookie.front() != '.')
        return domainInURL == domainInCookie; // Full domain

    size_t l2 = domainInCookie.length();
    if (0 == domainInCookie.compare(1, l2 - 1, domainInURL))
        return true;

    size_t l1 = domainInURL.length();
    if (l1 <= l2)
        return false;

    return 0 == domainInURL.compare(l1 - l2, l2, domainInCookie);
}

std::string CookieJarImpl::Get(const char *URL) const
{
    std::string ret;

    GURL u(URL);
    for (const auto &it : m_cookies)
    {
        if (!DomainMatch(u.host(), it.first))
            continue;

        for (const auto &it2 : it.second)
        {
            CanonicalCookie *c = it2.second.get();
            if (!c->IncludeForRequestURL(u, m_options))
                continue;

            ret.append(c->Name());
            ret.push_back('=');
            ret.append(c->Value());
            ret.append("; ");
        }
    }

    if (!ret.empty())
    {
        size_t s = ret.length();
        ret.resize(s - 2); // Remove the last "; "
    }
    return ret;
}

bool CookieJarImpl::Set(const char *setCookieHeader, const char *URL)
{
    std::unique_ptr<CanonicalCookie> c(CanonicalCookie::Create(GURL(URL), setCookieHeader, base::Time::Now(), m_options));
    if (nullptr == c)
    {
        BKLOG("Parse cookie failed! Cookie line: %s", setCookieHeader);
        return false;
    }

    auto it = m_cookies.find(c->Domain());
    if (std::end(m_cookies) == it)
    {
        auto r = m_cookies.insert(std::make_pair(c->Domain(), CookiesMap()));
        if (!r.second)
        {
            ASSERT(r.second);
            return false;
        }

        it = r.first;
    }

    auto it2 = it->second.find(c->Name());
    if (std::end(it->second) == it2)
    {
        it->second[c->Name()] = std::move(c);
        return true;
    }

    it2->second = std::move(c);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT void BKAPI BkClearCookieJar(BkCookieJar cookieJar)
{
    cookieJar->Clear();
}

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
