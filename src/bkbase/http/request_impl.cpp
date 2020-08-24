// -------------------------------------------------
// BlinKit - BkBase Library
// -------------------------------------------------
//   File Name: request_impl.cpp
// Description: RequestImpl Class
//      Author: Ziming Li
//     Created: 2018-09-27
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include "request_impl.h"

#include <atomic>
#include <mutex>
#include "base/strings/string_util.h"
#include "bkbase/http/http_constants.h"
#include "bkbase/http/http_response.h"
#include "bkcommon/bk_strings.h"
#include "bkcommon/controller_impl.h"

using namespace BlinKit;

static const long OPT_TRUE = true;
static const long OPT_FALSE = false;

class RequestImpl::Controller final : public ControllerImpl
{
public:
    void AddRef(void) { ++m_refCnt; }
    bool Cancelled(void) const { return m_cancelled; }

    void lock(void) { m_mutex.lock(); }
    void unlock(void) { m_mutex.unlock(); }

    // ControllerImpl
    int Release(void) override
    {
        if (0 == --m_refCnt)
            delete this;
        return BK_ERR_SUCCESS;
    }
private:
    void DoCancel(void)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cancelled = true;
    }

    // ControllerImpl
    int ContinueWorking(void) override
    {
        NOTREACHED();
        return BK_ERR_FORBIDDEN;
    }
    int CancelWork(void) override
    {
        DoCancel(); 
        return Release();
    }

    std::atomic<unsigned> m_refCnt{ 1 };
    std::mutex m_mutex;
    bool m_cancelled = false;
};

RequestImpl::RequestImpl(const char *URL, const BkRequestClient &client)
    : m_URL(URL), m_client(client), m_controller(new Controller)
    , m_method("GET"), m_timeoutInMs(HttpConstants::DefaultTimeoutInMs)
{
    m_userHeaders.Set("Accept", "*/*");
}

RequestImpl::~RequestImpl(void)
{
    m_controller->Release();

    if (nullptr != m_headersList)
        curl_slist_free_all(m_headersList);
    if (nullptr != m_curl)
        curl_easy_cleanup(m_curl);
    if (nullptr != m_response)
        m_response->Release();
}

void RequestImpl::DoThreadWork(void)
{
    std::string headers;
    curl_easy_setopt(m_curl, CURLOPT_HEADERDATA, &headers);
    curl_easy_setopt(m_curl, CURLOPT_HEADERFUNCTION, HeaderCallback);

    CURLcode code = curl_easy_perform(m_curl);
    if (CURLE_OK == code)
    {
        m_response->ParseHeaders(headers);
        m_response->InflateBodyIfNecessary();

        std::unique_lock<Controller> lock(*m_controller);
        if (!m_controller->Cancelled())
            m_client.RequestComplete(m_response, m_client.UserData);
    }
    else
    {
        BKLOG("ERROR: curl_easy_perform failed, code = %d.", code);

        std::unique_lock<Controller> lock(*m_controller);
        if (!m_controller->Cancelled())
            m_client.RequestFailed(BK_ERR_NETWORK, m_client.UserData);
    }

    delete this;
}

std::optional<CURLProxy> RequestImpl::GetProxyForCURL(void) const
{
    switch (m_proxyType)
    {
        case BK_PROXY_SYSTEM_DEFAULT:
        case BK_PROXY_DIRECT:
            break;
        case BK_PROXY_HTTP:
            return std::make_pair(CURLPROXY_HTTP, m_proxy);
        case BK_PROXY_SOCKS:
            return std::make_pair(CURLPROXY_SOCKS4, m_proxy);
        case BK_PROXY_SOCKS5:
            return std::make_pair(CURLPROXY_SOCKS5, m_proxy);
        default:
            NOTREACHED();
    }
    return std::nullopt;
}

size_t RequestImpl::HeaderCallback(char *buffer, size_t, size_t nitems, void *userData)
{
    std::string *headers = reinterpret_cast<std::string *>(userData);
    headers->append(buffer, nitems);
    return nitems;
}

int RequestImpl::Perform(void)
{
    int err = BK_ERR_UNKNOWN;
    do {
        // 1. Check URL.
        if (!m_URL.SchemeIsHTTPOrHTTPS())
        {
            err = BK_ERR_URI;
            break;
        }

        // 2. Prepare request.
        ASSERT(nullptr == m_curl);
        m_curl = curl_easy_init();
        if (nullptr == m_curl)
            break;

        curl_easy_setopt(m_curl, CURLOPT_URL, m_URL.spec().c_str());
        curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, OPT_FALSE);
        curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, OPT_FALSE);

        // 3. Adjust method.
        if (base::EqualsCaseInsensitiveASCII(m_method, "POST"))
        {
            m_headersList = curl_slist_append(m_headersList, "Expect:"); // Disable "HTTP 100" for response.
            curl_easy_setopt(m_curl, CURLOPT_POST, OPT_TRUE);
        }

        // 4. Process standard headers.
        for (const auto &it : m_standardHeaders)
            curl_easy_setopt(m_curl, it.first, it.second.c_str());

        // 5. Process user headers.
        if (nullptr != m_headersList)
        {
            curl_slist_free_all(m_headersList);
            m_headersList = nullptr;
        }
        for (const auto &it : m_userHeaders.GetRawMap())
        {
            std::string header(it.first);
            header.append(": ");
            header.append(it.second);
            m_headersList = curl_slist_append(m_headersList, header.c_str());
        }
        if (nullptr != m_headersList)
        {
            curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_headersList);
        }

        // 6. Fill body.
        if (!m_body.empty())
        {
            curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, m_body.size());
            curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, m_body.data());
        }

        // 7. Apply proxy.
        std::optional<CURLProxy> proxy = GetProxyForCURL();
        if (proxy.has_value())
        {
            curl_easy_setopt(m_curl, CURLOPT_PROXYTYPE, proxy->first);
            curl_easy_setopt(m_curl, CURLOPT_PROXY, proxy->second.c_str());
            if (m_URL.SchemeIs(url::kHttpsScheme))
            {
                curl_easy_setopt(m_curl, CURLOPT_HTTPPROXYTUNNEL, OPT_TRUE);
                curl_easy_setopt(m_curl, CURLOPT_SUPPRESS_CONNECT_HEADERS, OPT_TRUE);
            }
            // BKTODO: curl_easy_setopt(m_curl, CURLOPT_PROXYUSERPWD, "<usr>:<pwd>");
        }

        // 8. Apply other options.
        const long timeout = m_timeoutInMs;
        curl_easy_setopt(m_curl, CURLOPT_TIMEOUT_MS, timeout);

        // 9. Initialize response & start worker thread.
        m_response = new HttpResponse(m_URL.spec());
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, m_response);
        curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        if (StartWorkThread())
            return BK_ERR_SUCCESS;
    } while (false);

    ASSERT(BK_ERR_SUCCESS == err);
    delete this;
    return err;
}

void RequestImpl::SetBody(const void *data, size_t dataLength)
{
    m_body.resize(dataLength);
    memcpy(m_body.data(), data, dataLength);
}

void RequestImpl::SetHeader(const char *name, const char *value)
{
    CURLoption opt = TranslateOption(name);
    if (CURLOPT_HTTPHEADER != opt)
        m_standardHeaders[opt] = value;
    else
        m_userHeaders.Set(name, value);
}

void RequestImpl::SetProxy(int type, const char *proxy)
{
    switch (type)
    {
        case BK_PROXY_SYSTEM_DEFAULT:
        case BK_PROXY_DIRECT:
            break;
        case BK_PROXY_HTTP:
        case BK_PROXY_SOCKS:
        case BK_PROXY_SOCKS5:
            m_proxy.assign(proxy);
            break;
        default:
            NOTREACHED();
            type = BK_PROXY_DIRECT;
    }
    m_proxyType = type;
}

CURLoption RequestImpl::TranslateOption(const char *name)
{
    if (base::EqualsCaseInsensitiveASCII(name, Strings::HttpHeader::Cookie))
        return CURLOPT_COOKIE;
    if (base::EqualsCaseInsensitiveASCII(name, Strings::HttpHeader::Referer))
        return CURLOPT_REFERER;
    if (base::EqualsCaseInsensitiveASCII(name, Strings::HttpHeader::UserAgent))
        return CURLOPT_USERAGENT;
    return CURLOPT_HTTPHEADER;
}

size_t RequestImpl::WriteCallback(char *ptr, size_t, size_t nmemb, void *userData)
{
    HttpResponse *response = reinterpret_cast<HttpResponse *>(userData);
    response->AppendData(ptr, nmemb);
    return nmemb;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT BkRequest BKAPI BkCreateRequest(const char *URL, BkRequestClient *client)
{
    return RequestImpl::CreateInstance(URL, *client);
}

BKEXPORT int BKAPI BkPerformRequest(BkRequest request, BkWorkController *controller)
{
    if (nullptr != controller)
    {
        RequestImpl::Controller *c = request->GetController();
        c->AddRef();
        *controller = c;
    }
    return request->Perform();
}

BKEXPORT void BKAPI BkSetRequestBody(BkRequest request, const void *data, size_t dataLength)
{
    request->SetBody(data, dataLength);
}

BKEXPORT void BKAPI BkSetRequestHeader(BkRequest request, const char *name, const char *value)
{
    request->SetHeader(name, value);
}

BKEXPORT void BKAPI BkSetRequestMethod(BkRequest request, const char *method)
{
    request->SetMethod(method);
}

BKEXPORT void BKAPI BkSetRequestProxy(BkRequest request, int type, const char *proxy)
{
    request->SetProxy(type, proxy);
}

BKEXPORT void BKAPI BkSetRequestTimeout(BkRequest request, unsigned timeout)
{
    request->SetTimeout(timeout);
}

} // extern "C"
