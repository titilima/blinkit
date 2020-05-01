// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: bk_socket.h
// Description: Cross Platform Stuff for Socket
//      Author: Ziming Li
//     Created: 2020-04-29
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_COMMON_SOCKET_H
#define BLINKIT_COMMON_SOCKET_H

#pragma once

#if defined(OS_WIN)
#   include <WinSock2.h>
#   include <WS2tcpip.h>
#elif defined(OS_POSIX)
#   include <sys/socket.h>
#   include <arpa/inet.h>
#   include <netinet/in.h>
#else
#   error Not supported!
#endif

#ifdef OS_POSIX

typedef int SOCKET;

enum {
    INVALID_SOCKET = -1,
    SOCKET_ERROR   = -1,
};

inline int closesocket(SOCKET s) { return close(s); }

#endif // OS_POSIX

#endif // BLINKIT_COMMON_SOCKET_H
