// -------------------------------------------------
// BlinKit - BkLogin Library
// -------------------------------------------------
//   File Name: https_response_task.h
// Description: HTTPSResponseTask Class
//      Author: Ziming Li
//     Created: 2020-06-05
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKLOGIN_HTTPS_RESPONSE_TASK_H
#define BLINKIT_BKLOGIN_HTTPS_RESPONSE_TASK_H

#pragma once

#include <openssl/ossl_typ.h>
#include "blinkit/login/tasks/response_task_base.h"

namespace BlinKit {

class HTTPSResponseTask final : public ResponseTaskBase
{
public:
    HTTPSResponseTask(SOCKET s, SSL *ssl, LoginProxyImpl &loginProxy);
    ~HTTPSResponseTask(void) override;
private:
    int Send(const char *buf, int bufSize) override;

    SSL *m_ssl;
};

} // namespace BlinKit

#endif // BLINKIT_BKLOGIN_HTTPS_RESPONSE_TASK_H
