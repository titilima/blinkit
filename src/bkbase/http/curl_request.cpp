// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: curl_request.cpp
// Description: CURLRequest Class
//      Author: Ziming Li
//     Created: 2019-09-10
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "curl_request.h"

#include "base/strings/string_util.h"
#include "bkbase/http/http_response.h"
#include "url/gurl.h"

namespace BlinKit {

static const long OPT_TRUE = true;
static const long OPT_FALSE = false;

CURLRequest::CURLRequest(const char *URL, const BkRequestClient &client)
    : RequestImpl(URL, client)
    , m_curl(curl_easy_init())
{
    curl_easy_setopt(m_curl, CURLOPT_URL, URL);
    curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, OPT_FALSE);
    curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, OPT_FALSE);
}

CURLRequest::~CURLRequest(void)
{
    if (nullptr != m_headersList)
        curl_slist_free_all(m_headersList);
    curl_easy_cleanup(m_curl);
}

void CURLRequest::Cancel(void)
{
    ASSERT(false); // BKTODO:
}

static size_t HeaderCallback(char *buffer, size_t, size_t nitems, void *userData)
{
    std::string *headers = reinterpret_cast<std::string *>(userData);
    headers->append(buffer, nitems);
    return nitems;
}

void* CURLRequest::DoThreadWork(void)
{
    std::string headers;
    curl_easy_setopt(m_curl, CURLOPT_HEADERDATA, &headers);
    curl_easy_setopt(m_curl, CURLOPT_HEADERFUNCTION, HeaderCallback);

    CURLcode code = curl_easy_perform(m_curl);
    if (CURLE_OK == code)
    {
        m_response->ParseHeaders(headers);
        m_response->InflateBodyIfNecessary();
        m_client.RequestComplete(m_response, m_client.UserData);
    }
    else
    {
        BKLOG("ERROR: curl_easy_perform failed, code = %d.", code);
        m_client.RequestFailed(BK_ERR_NETWORK, m_client.UserData);
    }

    RequestImpl::Release();
    return nullptr;
}

ControllerImpl* CURLRequest::GetController(void)
{
    ASSERT(false); // BKTODO:
    return nullptr;
}

int CURLRequest::Perform(void)
{
    int err = BK_ERR_UNKNOWN;
    do {
        // 1. Check URL.
        GURL u(m_URL);
        if (!u.SchemeIsHTTPOrHTTPS())
        {
            err = BK_ERR_URI;
            break;
        }

        // 2. Adjust method.
        if (base::EqualsCaseInsensitiveASCII(m_method, "POST"))
            curl_easy_setopt(m_curl, CURLOPT_POST, OPT_TRUE);

        // 3. Process headers.
        for (const auto &it : m_headers.GetRawMap())
        {
            CURLoption opt = TranslateOption(it.first);
            if (CURLOPT_HTTPHEADER == opt)
            {
                std::string header(it.first);
                header.append(": ");
                header.append(it.second);
                m_headersList = curl_slist_append(m_headersList, header.c_str());
            }
            else
            {
                curl_easy_setopt(m_curl, opt, it.second.c_str());
            }
        }
        if (nullptr != m_headersList)
            curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_headersList);

        // 4. Fill body.
        if (!m_body.empty())
        {
            curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, m_body.size());
            curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, m_body.data());
        }

        // 5. Apply other options.
        ASSERT(ProxyType() == BK_PROXY_SYSTEM_DEFAULT); // BKTODO:
        const long timeout = TimeoutInMs();
        curl_easy_setopt(m_curl, CURLOPT_TIMEOUT_MS, timeout);

        // 6. Initialize response & start worker thread.
        m_response = new HttpResponse(m_URL);
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, m_response);
        curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        if (0 == pthread_create(&m_thread, nullptr, ThreadProc, this))
            return BK_ERR_SUCCESS;
    } while (false);

    ASSERT(BK_ERR_SUCCESS == err);
    delete this;
    return err;
}

void* CURLRequest::ThreadProc(void *arg)
{
    CURLRequest *This = reinterpret_cast<CURLRequest *>(arg);
    return This->DoThreadWork();
}

CURLoption CURLRequest::TranslateOption(const std::string &name)
{
    if (base::EqualsCaseInsensitiveASCII(name, "Cookie"))
        return CURLOPT_COOKIE;
    if (base::EqualsCaseInsensitiveASCII(name, "Referer"))
        return CURLOPT_REFERER;
    if (base::EqualsCaseInsensitiveASCII(name, "User-Agent"))
        return CURLOPT_USERAGENT;
    return CURLOPT_HTTPHEADER;
}

size_t CURLRequest::WriteCallback(char *ptr, size_t, size_t nmemb, void *userData)
{
    HttpResponse *response = reinterpret_cast<HttpResponse *>(userData);
    response->AppendData(ptr, nmemb);
    return nmemb;
}

} // namespace BlinKit

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RequestImpl* RequestImpl::CreateInstance(const char *URL, const BkRequestClient &client)
{
    return new BlinKit::CURLRequest(URL, client);
}
