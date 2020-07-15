// -------------------------------------------------
// BlinKit - BkHelper Library
// -------------------------------------------------
//   File Name: request_task_base.cpp
// Description: RequestTaskBase Class
//      Author: Ziming Li
//     Created: 2020-06-04
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "request_task_base.h"

#include <regex>
#include "bk_http.h"
#include "base/strings/string_split.h"
#include "base/strings/string_util.h"
#include "bkcommon/bk_strings.h"
#include "bkhelper/proxy/proxy_impl.h"
#include "bkhelper/proxy/tasks/https_request_task.h"
#include "bkhelper/proxy/tasks/last_response_task.h"
#include "bkhelper/proxy/tasks/response_task.h"

namespace BlinKit {

RequestTaskBase::RequestTaskBase(const std::shared_ptr<SocketWrapper> &socketWrapper, const std::string_view &leadChars)
    : m_socketWrapper(socketWrapper), m_leadData(leadChars.data(), leadChars.length())
{
}

ProxyTask* RequestTaskBase::DoRealRequest(ProxyImpl &proxy)
{
    ResponseTask *responseTask = new ResponseTask(m_socketWrapper, proxy);

    BkRequest req = responseTask->CreateRequest(GetURL());
    BkSetRequestMethod(req, m_requestMethod.c_str());
    for (const auto &kv : m_requestHeaders)
        BkSetRequestHeader(req, kv.first.c_str(), kv.second.c_str());
    if (!m_requestBody.empty())
        BkSetRequestBody(req, m_requestBody.data(), m_requestBody.size());
    if (BK_ERR_SUCCESS == BkPerformRequest(req, nullptr))
        return nullptr;

    ASSERT(false); // BKTODO:
    return nullptr;
}

ProxyTask* RequestTaskBase::Execute(ProxyImpl &proxy)
{
    if (!ParseRequest())
        return nullptr;

    AdjustHeaders(m_requestHeaders, proxy);
    if (m_requestMethod == "CONNECT")
    {
        static const char ConnectionEstablished[] = "HTTP/1.1 200 Connection Established\r\n\r\n";
        send(m_socketWrapper->GetSocket(), ConnectionEstablished, std::size(ConnectionEstablished) - 1, 0);

        std::string domain, port;
        size_t p = m_requestURI.find(':');
        if (std::string::npos != p)
        {
            domain = m_requestURI.substr(0, p);
            port = m_requestURI.substr(p + 1);
        }
        else
        {
            domain = m_requestURI;
        }

        SSL *ssl = proxy.NewSSL(domain);
        ASSERT(nullptr != ssl);
        std::shared_ptr<SocketWrapper> sslWrapper = SocketWrapper::SSLWrapper(m_socketWrapper, ssl);
        return new HTTPSRequestTask(sslWrapper, domain, port, proxy);
    }

    if (!proxy.PreProcessRequestPerform(this))
        return new LastResponseTask(m_socketWrapper);

    return DoRealRequest(proxy);
}

int RequestTaskBase::GetData(int data, BkBuffer *dst)
{
    std::string ret;
    switch (data)
    {
        case BK_PROXY_REQUEST_URL:
            ret = GetURL();
            break;
        default:
            NOTREACHED();
            return BK_ERR_NOT_FOUND;
    }
    BkSetBufferData(dst, ret.data(), ret.length());
    return BK_ERR_SUCCESS;
}

int RequestTaskBase::GetHeader(const char *name, BkBuffer *dst)
{
    auto it = m_requestHeaders.find(name);
    if (std::end(m_requestHeaders) == it)
        return BK_ERR_NOT_FOUND;

    BkSetBufferData(dst, it->second.data(), it->second.length());
    return BK_ERR_SUCCESS;
}

bool RequestTaskBase::ParseRequest(void)
{
    std::string rawData(m_leadData);

    char buf[4096];
    int size;
    size_t p;

    // Headers
    for (;;)
    {
        size = m_socketWrapper->Recv(buf, sizeof(buf));
        if (size < 0)
            return false;

        rawData.append(buf, size);

        p = rawData.find("\r\n\r\n");
        if (std::string::npos != p)
            break;
    }

    if (!ParseHeaders(std::string_view(rawData.data(), p)))
        return false;

    auto it = m_requestHeaders.find(Strings::HttpHeader::ContentLength);
    if (std::end(m_requestHeaders) == it || it->second == std::string(1, '0'))
        return true; // Need not to process body.

    const size_t bodySize = std::stoi(it->second);
    m_requestBody = rawData.substr(p + 4);
    if (m_requestBody.size() < bodySize)
    {
        do {
            size = m_socketWrapper->Recv(buf, sizeof(buf));
            if (size < 0)
                return false;

            m_requestBody.append(buf, size);
        } while (m_requestBody.size() < bodySize);
    }
    ASSERT(m_requestBody.size() == bodySize);
    return true;
}

bool RequestTaskBase::ParseHeaders(const std::string_view &s)
{
    size_t p = s.find("\r\n");
    if (std::string_view::npos == p)
    {
        ASSERT(std::string_view::npos != p);
        return false;
    }

    std::string_view firstLine = s.substr(0, p);
    std::regex pattern(R"(([A-Z]+)\s+(.*?)\s+HTTP\/\d\.\d)");
    std::match_results<std::string_view::const_iterator> match;
    if (!std::regex_search(firstLine.begin(), firstLine.end(), match, pattern))
    {
        ASSERT(false); // Invalid header!
        return false;
    }

    m_requestMethod = match.str(1);
    m_requestURI = match.str(2);
    BKLOG("%s %s", m_requestMethod.c_str(), m_requestURI.c_str());

    base::StringPairs headers;
    if (!base::SplitStringIntoKeyValuePairs(s.substr(p + 2), ':', '\n', &headers))
    {
        ASSERT(false); // Invalid header!
        return false;
    }

    for (const auto &kv : headers)
    {
        std::string k, v;
        base::TrimWhitespaceASCII(kv.first, base::TRIM_ALL, &k);
        base::TrimWhitespaceASCII(kv.second, base::TRIM_ALL, &v);
        m_requestHeaders[k] = v;
    }
    return true;
}

void RequestTaskBase::SetHeader(const char *name, const char *value)
{
    if (nullptr == value)
    {
        auto it = m_requestHeaders.find(name);
        if (std::end(m_requestHeaders) != it)
            m_requestHeaders.erase(it);
    }
    else
    {
        m_requestHeaders[name] = value;
    }
}

} // namespace BlinKit
