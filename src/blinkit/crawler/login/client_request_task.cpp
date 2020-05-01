// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: client_request_task.cpp
// Description: ClientRequestTask Class
//      Author: Ziming Li
//     Created: 2020-04-29
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "client_request_task.h"

#include <regex>
#include "base/strings/string_split.h"
#include "base/strings/string_util.h"
#include "blinkit/crawler/login_proxy.h"
#include "blinkit/http/request_impl.h"

namespace BlinKit {

ClientRequestTask::ClientRequestTask(SOCKET client, const char *lead, int leadSize) : m_client(client)
{
    std::string rawData(lead, leadSize);

    char buf[4096];
    int size;
    size_t p;

    // Headers
    for (;;)
    {
        size = recv(m_client, buf, sizeof(buf), 0);
        rawData.append(buf, size);

        p = rawData.find("\r\n\r\n");
        if (std::string::npos != p)
            break;
    }

    if (!ParseRequestHeaders(std::string_view(rawData.data(), p)))
        return;

    std::string contentLength = m_requestHeaders.Get("Content-Length");
    if (contentLength.empty() || contentLength == std::string(1, '0'))
        return; // Need not to process body.

    const size_t bodySize = std::stoi(contentLength);
    m_requestBody = rawData.substr(p + 4);
    if (m_requestBody.size() < bodySize)
    {
        do {
            size = recv(m_client, buf, sizeof(buf), 0);
            m_requestBody.append(buf, size);
        } while (m_requestBody.size() < bodySize);
    }
    ASSERT(m_requestBody.size() == bodySize);
}

void ClientRequestTask::AdjustHeaders(LoginProxy &loginProxy)
{
    m_requestHeaders.Remove("Proxy-Connection");
    m_requestHeaders.Set("User-Agent", loginProxy.UserAgent());
}

void ClientRequestTask::Execute(LoginProxy &loginProxy)
{
    BkRequest req = loginProxy.CreateRequest(m_requestURI, m_client);

    req->SetMethod(m_requestMethod);

    AdjustHeaders(loginProxy);
    req->SetHeaders(m_requestHeaders);

    if (!m_requestBody.empty())
        req->SetBody(m_requestBody.data(), m_requestBody.size());

    req->Perform();
}

bool ClientRequestTask::ParseRequestHeaders(const std::string_view &s)
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
