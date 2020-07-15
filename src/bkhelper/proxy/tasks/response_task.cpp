// -------------------------------------------------
// BlinKit - BkHelper Library
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
#include "bkcommon/bk_strings.h"
#include "bkcommon/buffer_impl.hpp"
#include "bkcommon/response_impl.h"
#include "bkhelper/proxy/proxy_impl.h"
#include "bkhelper/proxy/tasks/last_response_task.h"

namespace BlinKit {

ResponseTask::ResponseTask(const std::shared_ptr<SocketWrapper> &socketWrapper, ProxyImpl &proxy)
    : m_socketWrapper(socketWrapper), m_proxy(proxy)
{
}

ResponseTask::~ResponseTask(void)
{
    if (nullptr != m_response)
        m_response->Release();
}

void ResponseTask::AdjustHeaders(HttpHeaders &headers, ProxyImpl &proxy)
{
    headers["Connection"] = "close";

    auto it = headers.find(Strings::HttpHeader::ContentEncoding);
    if (std::end(headers) != it)
    {
        headers.erase(it);
        headers[Strings::HttpHeader::ContentLength] = std::to_string(m_response->ContentLength());
    }

    it = headers.find("Transfer-Encoding");
    if (std::end(headers) != it)
        headers.erase(it);
}

BkRequest ResponseTask::CreateRequest(const std::string &URL)
{
    BkRequestClient client = { 0 };
    client.SizeOfStruct = sizeof(BkRequestClient);
    client.UserData = this;
    client.RequestComplete = RequestCompleteImpl;
    client.RequestFailed = RequestFailedImpl;
    client.RequestRedirect = RequestRedirectImpl;
    return BkCreateRequest(URL.c_str(), &client);
}

ProxyTask* ResponseTask::Execute(ProxyImpl &proxy)
{
    if (!proxy.PreProcessRequestComplete(m_response))
        return new LastResponseTask(m_socketWrapper);

    ProcessResponse(proxy);
    return nullptr;
}

bool_t BKAPI ResponseTask::HeaderCallback(const char *k, const char *v, void *userData)
{
    HttpHeaders &headers = *(reinterpret_cast<HttpHeaders *>(userData));
    headers[k] = v;
    return true;
}

#if 0
bool ResponseTask::ProcessLoginOK(ProxyImpl &proxy)
{
    ASSERT(false); // BKTODO:
    std::string currentURL;
    m_response->GetData(BK_RESPONSE_CURRENT_URL, BkMakeBuffer(currentURL));
    if (!proxy.IsLoginSuccessful(currentURL))
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
#endif

void ResponseTask::ProcessRequestComplete(BkResponse response)
{
    ASSERT(nullptr == m_response);
    m_response = response->Retain();

    m_proxy.AddTask(this);
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

void ResponseTask::ProcessResponse(ProxyImpl &proxy)
{
    const std::string CRLF("\r\n");

    std::string httpVersion, statusText;
    m_response->GetData(BK_RESPONSE_HTTP_VERSION, BufferImpl::Wrap(httpVersion));
    m_response->GetData(BK_RESPONSE_STATUS_TEXT, BufferImpl::Wrap(statusText));

    std::string rawData = base::StringPrintf("HTTP/%s %d %s", httpVersion.c_str(), m_response->StatusCode(), statusText.c_str());
    rawData.append(CRLF);

    HttpHeaders headers;
    m_response->EnumerateHeaders(HeaderCallback, &headers);
    AdjustHeaders(headers, proxy);
    for (const auto &it : headers)
    {
        rawData.append(base::StringPrintf("%s: %s", it.first.c_str(), it.second.c_str()));
        rawData.append(CRLF);
    }

    size_t cookiesCount = m_response->CookiesCount();
    for (size_t i = 0; i < cookiesCount; ++i)
    {
        std::string cookie;
        m_response->GetCookie(i, BufferImpl::Wrap(cookie));
        rawData.append(base::StringPrintf("Set-Cookie: %s", cookie.c_str()));
        rawData.append(CRLF);
    }
    rawData.append(CRLF);

    rawData.append(reinterpret_cast<const char *>(m_response->Content()), m_response->ContentLength());

    m_socketWrapper->Send(rawData.data(), rawData.length());
}

} // namespace BlinKit
