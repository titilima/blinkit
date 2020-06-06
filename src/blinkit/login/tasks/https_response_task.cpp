// -------------------------------------------------
// BlinKit - BkLogin Library
// -------------------------------------------------
//   File Name: https_response_task.cpp
// Description: HTTPSResponseTask Class
//      Author: Ziming Li
//     Created: 2020-06-05
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "https_response_task.h"

#include <openssl/ssl.h>

namespace BlinKit {

HTTPSResponseTask::HTTPSResponseTask(SOCKET s, SSL *ssl, LoginProxyImpl &loginProxy)
    : ResponseTaskBase(s, loginProxy), m_ssl(ssl)
{
}

HTTPSResponseTask::~HTTPSResponseTask(void)
{
    if (nullptr != m_ssl)
    {
        SSL_shutdown(m_ssl);
        SSL_free(m_ssl);
    }
}

int HTTPSResponseTask::Send(const char *buf, int bufSize)
{
    return SSL_write(m_ssl, buf, bufSize);
}

} // namespace BlinKit
