// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: client_response_task.cpp
// Description: ClientResponseTask Class
//      Author: Ziming Li
//     Created: 2020-04-29
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "client_response_task.h"

#include "base/strings/stringprintf.h"
#include "blinkit/crawler/login_proxy.h"
#include "blinkit/http/response_impl.h"

namespace BlinKit {

ClientResponseTask::ClientResponseTask(ResponseImpl *response, SOCKET client)
    : m_client(client), m_response(response->shared_from_this())
{
    BKLOG("%s %d %s", response->CurrentURL().c_str(), response->StatusCode(), response->Reason().c_str());
    AdjustHeaders(m_response->MutableHeaders());
}

ClientResponseTask::~ClientResponseTask(void)
{
    closesocket(m_client);
}

void ClientResponseTask::AdjustHeaders(BkHTTPHeaderMap &headers)
{
    headers.Set("Connection", "close");
    headers.Remove("Transfer-Encoding");
}

std::string ClientResponseTask::BuildResponseData(void) const
{
    const std::string CRLF("\r\n");

    std::string ret = base::StringPrintf("HTTP/%s %d %s",
        m_response->HTTPVersion().c_str(),
        m_response->StatusCode(),
        m_response->Reason().c_str());
    ret.append(CRLF);

    for (const auto &it : m_response->Headers().GetRawMap())
    {
        ret.append(base::StringPrintf("%s: %s", it.first.c_str(), it.second.c_str()));
        ret.append(CRLF);
    }
    for (const std::string &cookie : m_response->Cookies())
    {
        ret.append(base::StringPrintf("Set-Cookie: %s", cookie.c_str()));
        ret.append(CRLF);
    }
    ret.append(CRLF);

    ret.append(m_response->BodyData(), m_response->BodyLength());
    return ret;
}

void ClientResponseTask::Execute(LoginProxy &loginProxy)
{
    SetCookies(loginProxy);

    std::string responseData = loginProxy.IsLoginSuccessful(m_response->CurrentURL())
        ? loginProxy.GetLoginSuccessfulResponseData()
        : BuildResponseData();
    send(m_client, responseData.data(), responseData.length(), 0);
}

void ClientResponseTask::SetCookies(LoginProxy &loginProxy)
{
    for (const std::string &cookie : m_response->Cookies())
        loginProxy.SetCookie(cookie);
}

} // namespace BlinKit
