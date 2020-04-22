// -------------------------------------------------
// BlinKit - url Library
// -------------------------------------------------
//   File Name: bk_url.h
// Description: BkURL Class
//      Author: Ziming Li
//     Created: 2019-09-14
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_URL_BK_URL_H
#define BLINKIT_URL_BK_URL_H

#pragma once

#include <string_view>
#include "url/third_party/mozilla/url_parse.h"

namespace BlinKit {

class BkURL
{
public:
    BkURL(void) = default;
    explicit BkURL(const std::string &URLString);

    static const BkURL& Blank(void) { return m_blank; }

    bool IsEmpty(void) const { return m_string.empty(); }
    bool IsValid(void) const { return m_isValid; }
    const std::string& AsString(void) const { return m_string; }

    std::string Scheme(void) const { return ComponentString(m_parsed.scheme); }
    bool SchemeIs(std::string_view scheme) const;
    bool SchemeIsHTTPOrHTTPS(void) const;
    bool SchemeIsData(void) const;
    bool SchemeIsFile(void) const;

    std::string Host(void) const { return ComponentString(m_parsed.host); }
    std::string Username(void) const { return ComponentString(m_parsed.username); }
    std::string Password(void) const { return ComponentString(m_parsed.password); }
    int EffectiveIntPort(void) const;
    std::string Path(void) const { return ComponentString(m_parsed.path); }
    std::string PathForRequest(void) const;
    std::string Query(void) const { return ComponentString(m_parsed.query); }
    bool HasRef(void) const { return m_parsed.ref.is_nonempty(); }

    BkURL Resolve(const std::string &relative) const;

    BkURL StripFragmentIdentifier(void) const;
    std::string StrippedForUseAsHref(void) const;
    std::string StrippedForUseAsReferrer(void) const;
private:
    friend bool EqualIgnoringFragmentIdentifier(const BkURL &a, const BkURL &b);

    std::string ComponentString(const url::Component &comp) const {
        return comp.is_nonempty() ? std::string(m_string.data() + comp.begin, comp.len) : std::string();
    }
    std::string_view ComponentStringView(const url::Component &comp) const {
        return comp.is_nonempty() ? std::string_view(m_string.data() + comp.begin, comp.len) : std::string();
    }

    bool m_isValid = false;
    std::string m_string;
    url::Parsed m_parsed;

    static BkURL m_blank;
};

bool EqualIgnoringFragmentIdentifier(const BkURL &a, const BkURL &b);

inline bool operator==(const BkURL &a, const BkURL &b) { return a.AsString() == b.AsString(); }

} // namespace BlinKit

#endif // BLINKIT_URL_BK_URL_H
