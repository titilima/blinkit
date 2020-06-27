// -------------------------------------------------
// BlinKit - BkLogin Library
// -------------------------------------------------
//   File Name: https_request_task.h
// Description: HTTPSRequestTask Class
//      Author: Ziming Li
//     Created: 2020-06-04
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKHELPER_HTTPS_REQUEST_TASK_H
#define BLINKIT_BKHELPER_HTTPS_REQUEST_TASK_H

#pragma once

#include <openssl/ossl_typ.h>
#include "bkhelper/proxy/tasks/request_task_base.h"

namespace BlinKit {

class HTTPSRequestTask final : public RequestTaskBase
{
public:
    HTTPSRequestTask(const std::shared_ptr<SocketWrapper> &sslWrapper, const std::string &domain, const std::string &port, LoginProxyImpl &loginProxy);
private:
    std::string GetURL(void) const override;

    const std::string m_domain, m_port;
};

} // namespace BlinKit

#endif // BLINKIT_BKHELPER_HTTPS_REQUEST_TASK_H
