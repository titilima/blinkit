// -------------------------------------------------
// BlinKit - BkLogin Library
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
    : RequestTaskBase(client, std::string_view(leadChars, leadSize))
{
}

void HTTPRequestTask::AdjustHeaders(BkHTTPHeaderMap &headers, LoginProxyImpl &loginProxy)
{
    RequestTaskBase::AdjustHeaders(headers, loginProxy);
    headers.Remove("Proxy-Connection");
}

ResponseTaskBase* HTTPRequestTask::CreateResponseTask(LoginProxyImpl &loginProxy)
{
    ASSERT(false); // BKTODO:
    return nullptr;
}

std::string HTTPRequestTask::GetURL(void) const
{
    ASSERT(false); // BKTODO:
    return std::string();
}

int HTTPRequestTask::Recv(char *buf, int bufSize) const
{
    return recv(m_socket, buf, bufSize, 0);
}

} // namespace BlinKit
