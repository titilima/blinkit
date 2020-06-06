// -------------------------------------------------
// BlinKit - BkLogin Library
// -------------------------------------------------
//   File Name: https_request_task.cpp
// Description: HTTPSRequestTask Class
//      Author: Ziming Li
//     Created: 2020-06-04
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "https_request_task.h"

#include <openssl/ssl.h>
#include "blinkit/login/login_proxy_impl.h"
#include "blinkit/login/tasks/https_response_task.h"

namespace BlinKit {

HTTPSRequestTask::HTTPSRequestTask(SOCKET client, const std::string &domain, const std::string &port, LoginProxyImpl &loginProxy)
    : RequestTaskBase(client)
    , m_domain(domain), m_port(port)
    , m_ssl(loginProxy.NewSSL(domain))
{
    if (nullptr != m_ssl)
    {
        SSL_set_fd(m_ssl, m_socket);
        SSL_accept(m_ssl); // BKTODO: Check failure.
    }
}

HTTPSRequestTask::~HTTPSRequestTask(void)
{
    if (nullptr != m_ssl)
    {
        SSL_shutdown(m_ssl);
        SSL_free(m_ssl);
    }
}

ResponseTaskBase* HTTPSRequestTask::CreateResponseTask(LoginProxyImpl &loginProxy)
{
    return new HTTPSResponseTask(DetachSocket(), DetachSSL(), loginProxy);
}

SSL* HTTPSRequestTask::DetachSSL(void)
{
    SSL *ret = m_ssl;
    m_ssl = nullptr;
    return ret;
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

int HTTPSRequestTask::Recv(char *buf, int bufSize) const
{
    return SSL_read(m_ssl, buf, bufSize);
}

} // namespace BlinKit
