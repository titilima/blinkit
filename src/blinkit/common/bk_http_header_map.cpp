// -------------------------------------------------
// BlinKit - BkCommon Library
// -------------------------------------------------
//   File Name: bk_http_header_map.cpp
// Description: BkHTTPHeaderMap Class
//      Author: Ziming Li
//     Created: 2019-10-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "bk_http_header_map.h"

#include "base/strings/string_util.h"

namespace BlinKit {

static const char CRLF[] = "\r\n";

std::string BkHTTPHeaderMap::CanonizeHeaderName(const std::string &header)
{
    static const char* specialNames[] = { "ETag" };
    for (const char *specialName : specialNames)
    {
        if (base::EqualsCaseInsensitiveASCII(header, specialName))
            return specialName;
    }

    std::string ret(header);
    bool upperNext = true;
    for (char &ch : ret)
    {
        if (upperNext)
        {
            ch = base::ToUpperASCII(ch);
            upperNext = false;
        }
        else
        {
            ch = base::ToLowerASCII(ch);
        }

        if ('-' == ch)
            upperNext = true;
    }
    return ret;
}

std::string BkHTTPHeaderMap::Get(const std::string &name) const
{
    std::string canonizedName = CanonizeHeaderName(name);
    auto it = m_headers.find(canonizedName);
    if (std::end(m_headers) == it)
        return std::string();
    return it->second;
}

std::string BkHTTPHeaderMap::GetAllForRequest(void) const
{
    std::string ret;
    for (const auto &it : m_headers)
    {
        ret.append(it.first);
        ret.append(": ");
        ret.append(it.second);
        ret.append(CRLF);
    }
    return ret;
}

void BkHTTPHeaderMap::Set(const std::string &name, const std::string &val)
{
    assert(std::string::npos == name.find_first_of(CRLF));
    assert(std::string::npos == val.find_first_of(CRLF));
    std::string canonizedName = CanonizeHeaderName(name);
    m_headers[canonizedName] = val;
}

} // namespace BlinKit
