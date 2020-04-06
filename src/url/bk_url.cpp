// -------------------------------------------------
// BlinKit - url Library
// -------------------------------------------------
//   File Name: bk_url.cpp
// Description: BkURL Class
//      Author: Ziming Li
//     Created: 2019-09-14
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "bk_url.h"

#include "url/url_canon_stdstring.h"
#include "url/url_constants.h"
#include "url/url_util.h"

using namespace url;

namespace BlinKit {

BkURL BkURL::m_blank("about:blank");

BkURL::BkURL(const std::string &URLString) : m_string(URLString)
{
    ParseStandardURL(m_string.data(), m_string.length(), &m_parsed);
    m_isValid = m_parsed.scheme.is_valid();
}

int BkURL::EffectiveIntPort(void) const
{
    if (m_parsed.port.is_nonempty())
        return ParsePort(m_string.data(), m_parsed.port);
    if (SchemeIs(kHttpScheme))
        return 80;
    if (SchemeIs(kHttpsScheme))
        return 443;
    assert(false); // Not reached!
    return PORT_UNSPECIFIED;
}

std::string BkURL::PathForRequest(void) const
{
    if (m_parsed.ref.is_valid())
        return m_string.substr(m_parsed.path.begin, m_parsed.ref.begin - m_parsed.path.begin - 1);

    if (!m_parsed.path.is_valid())
        return std::string(1, '/');

    int pathLen = m_parsed.path.len;
    if (m_parsed.query.is_valid())
        pathLen = m_parsed.query.end() - m_parsed.path.begin;
    return m_string.substr(m_parsed.path.begin, pathLen);
}

BkURL BkURL::Resolve(const std::string &relative) const
{
    // Not allowed for invalid URLs.
    if (!m_isValid)
        return BkURL();

    BkURL result;
    url::StdStringCanonOutput output(&result.m_string);
    if (!url::ResolveRelative(m_string.data(), static_cast<int>(m_string.length()), m_parsed,
        relative.data(), static_cast<int>(relative.length()), nullptr, &output, &result.m_parsed))
    {
        // Error resolving, return an empty URL.
        return BkURL();
    }
    output.Complete();
    result.m_isValid = true;
    return result;
}

bool BkURL::SchemeIs(std::string_view scheme) const
{
    return ComponentStringView(m_parsed.scheme) == scheme;
}

bool BkURL::SchemeIsData(void) const
{
    return SchemeIs(kDataScheme);
}

bool BkURL::SchemeIsFile(void) const
{
    return SchemeIs(kFileScheme);
}

bool BkURL::SchemeIsHTTPOrHTTPS(void) const
{
    return SchemeIs(kHttpScheme) || SchemeIs(kHttpsScheme);
}

std::string BkURL::StrippedForUseAsHref(void) const
{
    if (m_parsed.username.is_nonempty() || m_parsed.password.is_nonempty())
    {
        assert(false); // BKTODO:
#if 0
        KURL href(*this);
        href.SetUser(String());
        href.SetPass(String());
        return href.GetString();
#endif
    }
    return AsString();
}

std::string BkURL::StrippedForUseAsReferrer(void) const
{
    if (!SchemeIsHTTPOrHTTPS())
        return std::string();

    if (m_parsed.username.is_nonempty() || m_parsed.password.is_nonempty() || m_parsed.ref.is_valid())
    {
        assert(false); // BKTODO:
#if 0
        KURL referrer(*this);
        referrer.SetUser(String());
        referrer.SetPass(String());
        referrer.RemoveFragmentIdentifier();
        return referrer.GetString();
#endif
    }
    return AsString();
}

BkURL BkURL::StripFragmentIdentifier(void) const
{
    if (m_parsed.ref.is_nonempty())
    {
        assert(false); // BKTODO:
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool EqualIgnoringFragmentIdentifier(const BkURL &a, const BkURL &b)
{
    // Compute the length of each URL without its ref. Note that the reference
    // begin (if it exists) points to the character *after* the '#', so we need
    // to subtract one.
    int al = a.m_string.length();
    if (a.m_parsed.ref.len >= 0)
        al = a.m_parsed.ref.begin - 1;

    int bl = b.m_string.length();
    if (b.m_parsed.ref.len >= 0)
        bl = b.m_parsed.ref.begin - 1;

    if (al != bl)
        return false;

    const std::string &as = a.m_string;
    const std::string &bs = b.m_string;
    for (int i = 0; i < al; ++i)
    {
        if (as.at(i) != bs.at(i))
            return false;
    }
    return true;
}

} // namespace BlinKit
