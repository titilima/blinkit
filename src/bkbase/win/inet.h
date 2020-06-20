// -------------------------------------------------
// BlinKit - BkBase Library
// -------------------------------------------------
//   File Name: inet.h
// Description: WinInet Wrappers
//      Author: Ziming Li
//     Created: 2018-09-28
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKBASE_INET_H
#define BLINKIT_BKBASE_INET_H

#pragma once

#include <Windows.h>
#include <WinInet.h>

namespace BlinKit {

class InetObject
{
public:
    bool IsValid(void) const { return nullptr != m_handle; }
    void Close(void);
    bool SetOption(DWORD option, ULONG value);
    void SetStatusCallback(INTERNET_STATUS_CALLBACK lpfnInternetCallback);
protected:
    InetObject(HINTERNET handle, DWORD_PTR context);

    HINTERNET m_handle;
    DWORD_PTR m_context;
};

class InetConnection;

class InetSession final : public InetObject
{
public:
    InetSession(PVOID context);

    bool Open(const std::string &agent, DWORD dwAccessType, const std::string &proxy);

    InetConnection Connect(const std::string &serverName, INTERNET_PORT nServerPort,
        const std::string &userName, const std::string &password);
};

class InetRequest;

class InetConnection final : public InetObject
{
    friend class InetSession;
public:
    InetConnection(void);
    InetConnection& operator=(InetConnection &&o);

    InetRequest OpenRequest(const std::string &verb, const std::string &objectName,
        const std::string &referer, bool isHTTPS);
private:
    InetConnection(HINTERNET handle, DWORD_PTR context);
};

class InetRequest final : public InetObject
{
    friend class InetConnection;
public:
    InetRequest(void);
    InetRequest& operator=(InetRequest &&o);

    bool Send(LPINTERNET_BUFFERSA lpBuffersIn);
    bool End(void);
    bool QueryInfo(DWORD dwInfoLevel, DWORD &info);
    bool QueryInfo(DWORD dwInfoLevel, std::string &info);

    bool Read(LPINTERNET_BUFFERSA lpBuffersOut);
    bool Write(LPCVOID lpBuffer, DWORD dwSizeToWrite, PDWORD pSizeWritten);
private:
    InetRequest(HINTERNET handle, DWORD_PTR context);
};

} // namespace BlinKit

#endif // BLINKIT_BKBASE_INET_H
