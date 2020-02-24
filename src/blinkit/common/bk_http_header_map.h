// -------------------------------------------------
// BlinKit - BkCommon Library
// -------------------------------------------------
//   File Name: bk_http_header_map.h
// Description: BkHTTPHeaderMap Class
//      Author: Ziming Li
//     Created: 2019-10-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKCOMMON_BK_HTTP_HEADER_MAP_H
#define BLINKIT_BKCOMMON_BK_HTTP_HEADER_MAP_H

#pragma once

#include <string>
#include <unordered_map>

namespace BlinKit {

class BkHTTPHeaderMap
{
public:
    void Clear(void) { m_headers.clear(); }

    std::string Get(const std::string &name) const;
    void Set(const std::string &name, const std::string &val);

    void Remove(const std::string &name);

    std::string GetAllForRequest(void) const;
private:
    static std::string CanonizeHeaderName(const std::string &header);

    std::unordered_map<std::string, std::string> m_headers;
};

} // namespace BlinKit

#endif // BLINKIT_BKCOMMON_BK_HTTP_HEADER_MAP_H
