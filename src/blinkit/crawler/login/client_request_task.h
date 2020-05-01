// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: client_request_task.h
// Description: ClientRequestTask Class
//      Author: Ziming Li
//     Created: 2020-04-29
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_CLIENT_REQUEST_TASK_H
#define BLINKIT_BLINKIT_CLIENT_REQUEST_TASK_H

#pragma once

#include "blinkit/common/bk_socket.h"
#include "blinkit/common/bk_http_header_map.h"
#include "blinkit/crawler/login/login_task.h"

namespace BlinKit {

class ClientRequestTask final : public LoginTask
{
public:
    ClientRequestTask(SOCKET client, const char *lead, int leadSize);
private:
    void AdjustHeaders(LoginProxy &loginProxy);
    bool ParseRequestHeaders(const std::string_view &s);

    void Execute(LoginProxy &loginProxy) override;

    SOCKET m_client;
    std::string m_requestMethod;
    std::string m_requestURI;
    BlinKit::BkHTTPHeaderMap m_requestHeaders;
    std::string m_requestBody;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CLIENT_REQUEST_TASK_H
