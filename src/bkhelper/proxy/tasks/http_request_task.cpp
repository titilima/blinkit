// -------------------------------------------------
// BlinKit - BkHelper Library
// -------------------------------------------------
//   File Name: http_request_task.cpp
// Description: HTTPRequestTask Class
//      Author: Ziming Li
//     Created: 2020-06-04
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "http_request_task.h"

namespace BlinKit {

HTTPRequestTask::HTTPRequestTask(SOCKET client, const char *leadChars, int leadSize)
    : RequestTaskBase(SocketWrapper::PlainWrapper(client), std::string_view(leadChars, leadSize))
{
}

void HTTPRequestTask::AdjustHeaders(HttpHeaders &headers, ProxyImpl &proxy)
{
    RequestTaskBase::AdjustHeaders(headers, proxy);

    auto it = headers.find("Proxy-Connection");
    if (std::end(headers) != it)
        headers.erase(it);
}

std::string HTTPRequestTask::GetURL(void) const
{
    return RequestURI();
}

} // namespace BlinKit
