// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: client_response_task.h
// Description: ClientResponseTask Class
//      Author: Ziming Li
//     Created: 2020-04-29
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_CLIENT_RESPONSE_TASK_H
#define BLINKIT_BLINKIT_CLIENT_RESPONSE_TASK_H

#pragma once

#include "blinkit/common/bk_socket.h"
#include "blinkit/crawler/login/login_task.h"

class ResponseImpl;

namespace BlinKit {

class BkHTTPHeaderMap;

class ClientResponseTask final : public LoginTask
{
public:
    ClientResponseTask(ResponseImpl *response, SOCKET client);
    ~ClientResponseTask(void) override;
private:
    void AdjustHeaders(BkHTTPHeaderMap &headers);
    void SetCookies(LoginProxy &loginProxy);
    std::string BuildResponseData(void) const;

    void Execute(LoginProxy &loginProxy) override;

    SOCKET m_client;
    std::shared_ptr<ResponseImpl> m_response;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CLIENT_RESPONSE_TASK_H
