// -------------------------------------------------
// BlinKit - BkHelper Library
// -------------------------------------------------
//   File Name: socket_wrapper.h
// Description: SocketWrapper Class
//      Author: Ziming Li
//     Created: 2020-06-08
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKHELPER_SOCKET_WRAPPER_H
#define BLINKIT_BKHELPER_SOCKET_WRAPPER_H

#pragma once

#include <openssl/ossl_typ.h>
#include "bkcommon/bk_socket.h"

namespace BlinKit {

class SocketWrapper
{
public:
    static std::shared_ptr<SocketWrapper> PlainWrapper(SOCKET s);
    static std::shared_ptr<SocketWrapper> SSLWrapper(const std::shared_ptr<SocketWrapper> &sw, SSL *ssl);
    virtual ~SocketWrapper(void) = default;

    virtual SOCKET GetSocket(void) const = 0;
    virtual int Recv(char *buf, int bufSize) = 0;
    virtual int Send(const char *buf, int bufSize) = 0;
protected:
    SocketWrapper(void) = default;
};

} // namespace BlinKit

#endif // BLINKIT_BKHELPER_SOCKET_WRAPPER_H
