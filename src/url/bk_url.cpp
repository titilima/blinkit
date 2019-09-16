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

#include "url/url_constants.h"

using namespace url;

namespace BlinKit {

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
    assert(false); // BKTODO:
    return result;
}

bool BkURL::SchemeIs(std::string_view scheme) const
{
    return ComponentStringView(m_parsed.scheme) == scheme;
}

bool BkURL::SchemeIsHTTPOrHTTPS(void) const
{
    return SchemeIs(kHttpScheme) || SchemeIs(kHttpsScheme);
}

} // namespace BlinKit
