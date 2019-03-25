// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: inet.cpp
// Description: WinInet Wrappers
//      Author: Ziming Li
//     Created: 2018-09-28
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include "inet.h"

namespace BlinKit {

InetObject::InetObject(HINTERNET handle, DWORD_PTR context) : m_handle(handle), m_context(context)
{
    // Nothing
}

void InetObject::Close(void)
{
    if (nullptr != m_handle)
    {
        InternetCloseHandle(m_handle);
        m_handle = nullptr;
    }
}

bool InetObject::SetOption(DWORD option, ULONG value)
{
    return InternetSetOptionA(m_handle, option, &value, sizeof(ULONG));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

InetSession::InetSession(PVOID context) : InetObject(nullptr, reinterpret_cast<DWORD_PTR>(context))
{
    // Nothing
}

void InetSession::Close(void)
{
    SetStatusCallback(nullptr);
    InetObject::Close();
}

InetConnection InetSession::Connect(
    const std::string &serverName, INTERNET_PORT nServerPort,
    const std::string &userName, const std::string &password)
{
    HINTERNET h = InternetConnectA(m_handle, serverName.c_str(), nServerPort, userName.c_str(), password.c_str(),
        INTERNET_SERVICE_HTTP, 0, m_context);
    return InetConnection(h, m_context);
}

bool InetSession::Open(const std::string &agent, DWORD dwAccessType, const std::string &proxy)
{
    m_handle = InternetOpenA(agent.c_str(), dwAccessType, proxy.c_str(), nullptr, INTERNET_FLAG_ASYNC);
    return nullptr != m_handle;
}

void InetSession::SetStatusCallback(INTERNET_STATUS_CALLBACK lpfnInternetCallback)
{
    InternetSetStatusCallbackA(m_handle, lpfnInternetCallback);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

InetConnection::InetConnection(void) : InetObject(nullptr, 0)
{
    // Nothing
}

InetConnection::InetConnection(HINTERNET handle, DWORD_PTR context) : InetObject(handle, context)
{
    // Nothing
}

InetConnection& InetConnection::operator=(InetConnection &&o)
{
    m_handle = o.m_handle;
    o.m_handle = nullptr;
    m_context = o.m_context;
    return *this;
}

InetRequest InetConnection::OpenRequest(
    const std::string &verb,
    const std::string &objectName, const std::string &referer,
    bool isHTTPS)
{
    DWORD flags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_NO_COOKIES
        | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_NO_UI;
    if (isHTTPS)
        flags |= INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;

    HINTERNET h = HttpOpenRequestA(m_handle, verb.c_str(), objectName.c_str(), "HTTP/1.1", referer.c_str(),
        nullptr, flags, m_context);
    return InetRequest(h, m_context);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

InetRequest::InetRequest(void) : InetObject(nullptr, 0)
{
    // Nothing
}

InetRequest::InetRequest(HINTERNET handle, DWORD_PTR context) : InetObject(handle, context)
{
    // Nothing
}

InetRequest& InetRequest::operator=(InetRequest &&o)
{
    m_handle = o.m_handle;
    o.m_handle = nullptr;
    m_context = o.m_context;
    return *this;
}

bool InetRequest::End(void)
{
    return HttpEndRequestA(m_handle, nullptr, 0, m_context);
}

bool InetRequest::QueryInfo(DWORD dwInfoLevel, DWORD &info)
{
    DWORD l = sizeof(DWORD);
#ifdef _DEBUG
    HttpQueryInfoA(m_handle, dwInfoLevel | HTTP_QUERY_FLAG_NUMBER, nullptr, &l, nullptr);
    assert(sizeof(DWORD) == l || ERROR_HTTP_HEADER_NOT_FOUND == GetLastError());
#endif
    return HttpQueryInfoA(m_handle, dwInfoLevel | HTTP_QUERY_FLAG_NUMBER, &info, &l, nullptr);
}

bool InetRequest::QueryInfo(DWORD dwInfoLevel, std::string &info)
{
    DWORD l = 0;
    if (!HttpQueryInfoA(m_handle, dwInfoLevel, nullptr, &l, nullptr))
    {
        DWORD err = GetLastError();
        if (ERROR_HTTP_HEADER_NOT_FOUND == err)
            return false;

        if (ERROR_INSUFFICIENT_BUFFER != err)
        {
            assert(ERROR_INSUFFICIENT_BUFFER == err);
            return false;
        }
    }

    info.resize(l - 1);
    return HttpQueryInfoA(m_handle, dwInfoLevel, const_cast<PSTR>(info.c_str()), &l, nullptr);
}

bool InetRequest::Read(LPINTERNET_BUFFERSA lpBuffersOut)
{
    return InternetReadFileExA(m_handle, lpBuffersOut, IRF_ASYNC, m_context);
}

bool InetRequest::Send(LPINTERNET_BUFFERSA lpBuffersIn)
{
    return HttpSendRequestExA(m_handle, lpBuffersIn, nullptr, HSR_ASYNC, m_context);
}

bool InetRequest::Write(LPCVOID lpBuffer, DWORD dwSizeToWrite, PDWORD pSizeWritten)
{
    return InternetWriteFile(m_handle, lpBuffer, dwSizeToWrite, pSizeWritten);
}

} // namespace BlinKit
