// -------------------------------------------------
// BlinKit - BkLogin Library
// -------------------------------------------------
//   File Name: socket_wrapper.cpp
// Description: Socket Wrapper Classes
//      Author: Ziming Li
//     Created: 2020-06-08
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "socket_wrapper.h"

#include <openssl/ssl.h>

namespace BlinKit {

class PlainSocketWrapper final : public SocketWrapper
{
public:
    PlainSocketWrapper(SOCKET s);
    ~PlainSocketWrapper(void) override;
private:
    SOCKET GetSocket(void) const override { return m_socket; }
    int Recv(char *buf, int bufSize) override;
    int Send(const char *buf, int bufSize) override;

    SOCKET m_socket;
};

PlainSocketWrapper::PlainSocketWrapper(SOCKET s) : m_socket(s)
{
}

PlainSocketWrapper::~PlainSocketWrapper(void)
{
    shutdown(m_socket, SD_BOTH);
    closesocket(m_socket);
}

int PlainSocketWrapper::Recv(char *buf, int bufSize)
{
    return recv(m_socket, buf, bufSize, 0);
}

int PlainSocketWrapper::Send(const char *buf, int bufSize)
{
    return send(m_socket, buf, bufSize, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SSLSocketWrapper final : public SocketWrapper
{
public:
    SSLSocketWrapper(const std::shared_ptr<SocketWrapper> &sw, SSL *ssl);
    ~SSLSocketWrapper(void) override;
private:
    SOCKET GetSocket(void) const override;
    int Recv(char *buf, int bufSize) override;
    int Send(const char *buf, int bufSize) override;

    std::shared_ptr<SocketWrapper> m_plainWrapper;
    SSL *m_ssl;
};

SSLSocketWrapper::SSLSocketWrapper(const std::shared_ptr<SocketWrapper> &sw, SSL *ssl) : m_plainWrapper(sw), m_ssl(ssl)
{
    SSL_set_fd(m_ssl, m_plainWrapper->GetSocket());
    if (-1 == SSL_accept(m_ssl))
        BKLOG("SSL_accept failed.");
}

SSLSocketWrapper::~SSLSocketWrapper(void)
{
    SSL_shutdown(m_ssl);
    SSL_free(m_ssl);
}

SOCKET SSLSocketWrapper::GetSocket(void) const
{
    NOTREACHED();
    return INVALID_SOCKET;
}

int SSLSocketWrapper::Recv(char *buf, int bufSize)
{
    return SSL_read(m_ssl, buf, bufSize);
}

int SSLSocketWrapper::Send(const char *buf, int bufSize)
{
    return SSL_write(m_ssl, buf, bufSize);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<SocketWrapper> SocketWrapper::PlainWrapper(SOCKET s)
{
    return std::make_shared<PlainSocketWrapper>(s);
}

std::shared_ptr<SocketWrapper> SocketWrapper::SSLWrapper(const std::shared_ptr<SocketWrapper> &sw, SSL *ssl)
{
    return std::make_shared<SSLSocketWrapper>(sw, ssl);
}

} // namespace BlinKit
