// -------------------------------------------------
// BlinKit - BkLogin Library
// -------------------------------------------------
//   File Name: response_task.cpp
// Description: ResponseTask Class
//      Author: Ziming Li
//     Created: 2020-06-05
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "response_task.h"

#include "base/strings/stringprintf.h"
#include "blinkit/http/response_impl.h"
#include "blinkit/login/login_proxy_impl.h"

namespace BlinKit {

ResponseTask::ResponseTask(const std::shared_ptr<SocketWrapper> &socketWrapper, LoginProxyImpl &loginProxy)
    : m_socketWrapper(socketWrapper), m_loginProxy(loginProxy)
{
    BkRequestClient::SizeOfStruct = sizeof(BkRequestClient);
    BkRequestClient::UserData = this;
    BkRequestClient::RequestComplete = RequestCompleteImpl;
    BkRequestClient::RequestFailed = RequestFailedImpl;
    BkRequestClient::RequestRedirect = RequestRedirectImpl;
}

void ResponseTask::AdjustHeaders(BkHTTPHeaderMap &headers, LoginProxyImpl &loginProxy)
{
    headers.Set("Connection", "close");
    if (headers.Has("Content-Encoding"))
    {
        headers.Remove("Content-Encoding");
        headers.Set("Content-Length", std::to_string(m_response->BodyLength()));
    }
    headers.Remove("Transfer-Encoding");
}

LoginTask* ResponseTask::Execute(LoginProxyImpl &loginProxy)
{
    const std::vector<std::string> &cookies = m_response->Cookies();
    if (!cookies.empty())
    {
        const std::string &currentURL = m_response->CurrentURL();
        for (const std::string &cookie : m_response->Cookies())
            loginProxy.SetCookie(currentURL, cookie);
    }

    if (!ProcessLoginOK(loginProxy))
        ProcessResponse(loginProxy);

    return nullptr;
}

bool ResponseTask::ProcessLoginOK(LoginProxyImpl &loginProxy)
{
    if (!loginProxy.IsLoginSuccessful(m_response->CurrentURL()))
        return false;

    std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Content-Type: text/html\r\n\r\n"
        ;
    response += loginProxy.GetLoggedInHTML();
    m_socketWrapper->Send(response.data(), response.length());
    return true;
}

void ResponseTask::ProcessRequestComplete(BkResponse response)
{
    m_response = response->shared_from_this();
    m_loginProxy.AddTask(this);
}

void ResponseTask::ProcessRequestFailed(int errorCode)
{
    ASSERT(false); // BKTODO:
}

void BKAPI ResponseTask::RequestCompleteImpl(BkResponse response, void *userData)
{
    reinterpret_cast<ResponseTask *>(userData)->ProcessRequestComplete(response);
}

void BKAPI ResponseTask::RequestFailedImpl(int errorCode, void *userData)
{
    reinterpret_cast<ResponseTask *>(userData)->ProcessRequestFailed(errorCode);
}

void ResponseTask::ProcessResponse(LoginProxyImpl &loginProxy)
{
    const std::string CRLF("\r\n");

    std::string rawData = base::StringPrintf("HTTP/%s %d %s",
        m_response->HTTPVersion().c_str(),
        m_response->StatusCode(),
        m_response->Reason().c_str());
    rawData.append(CRLF);

    AdjustHeaders(m_response->MutableHeaders(), loginProxy);
    for (const auto &it : m_response->Headers().GetRawMap())
    {
        rawData.append(base::StringPrintf("%s: %s", it.first.c_str(), it.second.c_str()));
        rawData.append(CRLF);
    }
    for (const std::string &cookie : m_response->Cookies())
    {
        rawData.append(base::StringPrintf("Set-Cookie: %s", cookie.c_str()));
        rawData.append(CRLF);
    }
    rawData.append(CRLF);

    rawData.append(m_response->BodyData(), m_response->BodyLength());

    m_socketWrapper->Send(rawData.data(), rawData.length());
}

} // namespace BlinKit
