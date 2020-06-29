// -------------------------------------------------
// BlinKit - BkHelper Library
// -------------------------------------------------
//   File Name: last_response_task.cpp
// Description: LastResponseTask Class
//      Author: Ziming Li
//     Created: 2020-06-29
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "last_response_task.h"

#include "bkhelper/proxy/proxy_impl.h"

namespace BlinKit {

LastResponseTask::LastResponseTask(const std::shared_ptr<SocketWrapper> &socketWrapper) : m_socketWrapper(socketWrapper)
{
}

ProxyTask* LastResponseTask::Execute(ProxyImpl &proxy)
{
    std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Content-Type: text/html; charset=utf-8\r\n\r\n"
        ;
    response += proxy.GetExitBody();
    m_socketWrapper->Send(response.data(), response.length());
    return nullptr;
}

} // namespace BlinKit
