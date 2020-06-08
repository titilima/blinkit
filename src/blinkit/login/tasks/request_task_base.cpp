// -------------------------------------------------
// BlinKit - BkLogin Library
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
#include "base/strings/string_split.h"
#include "base/strings/string_util.h"
#include "blinkit/http/request_impl.h"
#include "blinkit/login/login_proxy_impl.h"
#include "blinkit/login/tasks/https_request_task.h"
#include "blinkit/login/tasks/response_task.h"

namespace BlinKit {

RequestTaskBase::RequestTaskBase(const std::shared_ptr<SocketWrapper> &socketWrapper, const std::string_view &leadChars)
    : m_socketWrapper(socketWrapper), m_leadData(leadChars.data(), leadChars.length())
{
}

void RequestTaskBase::AdjustHeaders(BkHTTPHeaderMap &headers, LoginProxyImpl &loginProxy)
{
    headers.Set("User-Agent", loginProxy.UserAgent());
}

LoginTask* RequestTaskBase::DoRealRequest(LoginProxyImpl &loginProxy)
{
    ResponseTask *responseTask = new ResponseTask(m_socketWrapper, loginProxy);

    RequestImpl *req = loginProxy.CreateRequest(GetURL(), *responseTask);
    req->SetMethod(m_requestMethod);
    req->SetHeaders(m_requestHeaders);
    if (!m_requestBody.empty())
        req->SetBody(m_requestBody.data(), m_requestBody.size());
    if (BK_ERR_SUCCESS == req->Perform())
        return nullptr;

    ASSERT(false); // BKTODO:
    return nullptr;
}

LoginTask* RequestTaskBase::Execute(LoginProxyImpl &loginProxy)
{
    if (!ParseRequest())
        return nullptr;

    AdjustHeaders(m_requestHeaders, loginProxy);
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

        SSL *ssl = loginProxy.NewSSL(domain);
        ASSERT(nullptr != ssl);
        std::shared_ptr<SocketWrapper> sslWrapper = SocketWrapper::SSLWrapper(m_socketWrapper, ssl);
        return new HTTPSRequestTask(sslWrapper, domain, port, loginProxy);
    }

    return DoRealRequest(loginProxy);
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

    std::string contentLength = m_requestHeaders.Get("Content-Length");
    if (contentLength.empty() || contentLength == std::string(1, '0'))
        return true; // Need not to process body.

    const size_t bodySize = std::stoi(contentLength);
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
        m_requestHeaders.Set(k, v);
    }
    return true;
}

} // namespace BlinKit
