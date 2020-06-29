// -------------------------------------------------
// BlinKit - BkHelper Library
// -------------------------------------------------
//   File Name: https_request_task.cpp
// Description: HTTPSRequestTask Class
//      Author: Ziming Li
//     Created: 2020-06-04
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "https_request_task.h"

namespace BlinKit {

HTTPSRequestTask::HTTPSRequestTask(const std::shared_ptr<SocketWrapper> &sslWrapper, const std::string &domain, const std::string &port, ProxyImpl &)
    : RequestTaskBase(sslWrapper)
    , m_domain(domain), m_port(port)
{
}

std::string HTTPSRequestTask::GetURL(void) const
{
    std::string ret("https://");
    ret.append(m_domain);
    if (m_port != "443")
        ret.append(m_port);
    ret.append(RequestURI());
    return ret;
}

} // namespace BlinKit
