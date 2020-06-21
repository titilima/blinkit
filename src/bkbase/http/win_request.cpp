// -------------------------------------------------
// BlinKit - BkBase Library
// -------------------------------------------------
//   File Name: win_request.cpp
// Description: WinRequest Class
//      Author: Ziming Li
//     Created: 2018-09-27
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include "win_request.h"

#include "base/strings/string_util.h"
#include "bkcommon/bk_strings.h"
#include "bkbase/http/http_response.h"
#include "url/gurl.h"
#include "url/url_constants.h"

namespace BlinKit {

WinRequest::WinRequest(const char *URL, const BkRequestClient &client)
    : RequestImpl(URL, client)
    , m_userAgent(Strings::DefaultUserAgent)
    , m_session(this)
{
    // Nothing
}

WinRequest::~WinRequest(void)
{
    if (nullptr != m_hEvent)
        CloseHandle(m_hEvent);
    if (nullptr != m_hEventCancel)
        CloseHandle(m_hEventCancel);
    if (nullptr != m_hThread)
        CloseHandle(m_hThread);

    m_request.Close();
    m_connection.Close();
    m_session.Close();

    if (nullptr != m_response)
        m_response->Release();
}

void WinRequest::Cancel(void)
{
    ASSERT(nullptr != m_hEventCancel);
    SetEvent(m_hEventCancel);
    RequestImpl::Release();
}

int WinRequest::Continue(ThreadWorker nextWorker, bool signal)
{
    m_nextWorker = nextWorker;
    if (signal)
        SetEvent(m_hEvent);
    return BK_ERR_SUCCESS;
}

WinRequest::ThreadWorker WinRequest::DetachThreadWorker(void)
{
    ThreadWorker worker = m_nextWorker;
    m_nextWorker = nullptr;
    return worker;
}

DWORD WinRequest::DoThreadWork(void)
{
    for (;;)
    {
        int r = WaitForIOPending();
        if (BK_ERR_SUCCESS != r)
        {
            m_response->SetErrorCode(r);
            break;
        }

        if (BK_ERR_SUCCESS != m_response->ErrorCode())
            break;

        ThreadWorker worker = DetachThreadWorker();
        if (nullptr == worker)
            break;

        r = (this->*worker)();
        if (BK_ERR_SUCCESS != r)
        {
            m_response->SetErrorCode(r);
            break;
        }
    }

    if (BK_ERR_SUCCESS != m_response->ErrorCode())
        m_client.RequestFailed(m_response->ErrorCode(), m_client.UserData);

    RequestImpl::Release();
    return EXIT_SUCCESS;
}

int WinRequest::EndRequest(void)
{
    bool signal = true;

    if (!m_request.End())
    {
        DWORD err = GetLastError();
        if (ERROR_IO_PENDING != err)
        {
            ASSERT(ERROR_IO_PENDING == err);
            return BK_ERR_UNKNOWN;
        }
        signal = false;
    }

    return Continue(&WinRequest::QueryRequest, signal);
}

ControllerImpl* WinRequest::GetController(void)
{
    ASSERT(nullptr == m_hEventCancel);
    m_hEventCancel = CreateEvent(nullptr, TRUE, FALSE, nullptr);

    return RequestImpl::GetController();
}

int WinRequest::OpenRequest(const std::string &URL)
{
    GURL u(URL);
    if (!u.SchemeIsHTTPOrHTTPS())
        return BK_ERR_URI;

    ASSERT(!m_connection.IsValid());
    m_connection = m_session.Connect(u.host(), u.EffectiveIntPort(), u.username(), u.password());
    if (!m_connection.IsValid())
        return BK_ERR_NETWORK;

    ASSERT(!m_request.IsValid());
    m_request = m_connection.OpenRequest(m_method, u.PathForRequest(), m_referer, u.SchemeIs(url::kHttpsScheme));
    if (!m_request.IsValid())
        return BK_ERR_NETWORK;

    return BK_ERR_SUCCESS;
}

bool WinRequest::OpenSession(void)
{
    std::string proxy;
    DWORD proxyType = INTERNET_OPEN_TYPE_PRECONFIG;
    switch (ProxyType())
    {
        case BK_PROXY_DIRECT:
            proxyType = INTERNET_OPEN_TYPE_DIRECT;
            break;
        case BK_PROXY_USER_SPECIFIED:
            proxyType = INTERNET_OPEN_TYPE_PROXY;
            proxy = Proxy();
            break;
        default:
            ASSERT(ProxyType() == BK_PROXY_SYSTEM_DEFAULT);
    }

    if (!m_session.Open(m_userAgent, proxyType, proxy))
        return false;

    m_session.SetStatusCallback(StatusCallback);
    m_session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, TimeoutInMs());
    return true;
}

int WinRequest::Perform(void)
{
    int r;
    do {
        if (m_session.IsValid())
        {
            r = BK_ERR_FORBIDDEN;
            break;
        }

        if (!OpenSession())
        {
            r = BK_ERR_NETWORK;
            break;
        }

        r = OpenRequest(m_URL);
        if (BK_ERR_SUCCESS != r)
            break;

        StartWorkThread();
        return Continue(&WinRequest::SendRequest, true);
    } while (false);
    delete this;
    return r;
}

int WinRequest::QueryRequest(void)
{
    std::string rawHeaders;
    if (!m_request.QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, rawHeaders))
        return BK_ERR_UNKNOWN;

    m_response->ParseHeaders(rawHeaders);

    DWORD contentLength = 0;
    if (m_request.QueryInfo(HTTP_QUERY_CONTENT_LENGTH, contentLength))
        m_response->PrepareBody(contentLength);

    m_request.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, TimeoutInMs());
    return Continue(&WinRequest::ReceiveData, true);
}

int WinRequest::ReceiveData(void)
{
    BYTE buf[1024];
    INTERNET_BUFFERSA ib = { 0 };
    ib.dwStructSize = sizeof(ib);
    ib.lpvBuffer = buf;
    ib.dwBufferLength = sizeof(buf);

    bool done = false;
    while (!done)
    {
        if (!m_request.Read(&ib))
        {
            DWORD err = GetLastError();
            if (ERROR_IO_PENDING != err)
            {
                ASSERT(ERROR_IO_PENDING == err);
                return BK_ERR_NETWORK;
            }

            int r = WaitForIOPending();
            if (BK_ERR_SUCCESS != r)
                return r;
            if (BK_ERR_SUCCESS != m_response->ErrorCode())
                return m_response->ErrorCode();
        }

        if (ib.dwBufferLength > 0)
            m_response->AppendData(buf, ib.dwBufferLength);
        else
            done = true;
    }

    return Continue(&WinRequest::RequestComplete, true);
}

int WinRequest::RequestComplete(void)
{
    std::string contentEncoding;
    if (BK_ERR_SUCCESS == m_response->GetHeader(Strings::HttpHeader::ContentEncoding, BkMakeBuffer(contentEncoding)))
    {
        if (base::EqualsCaseInsensitiveASCII(contentEncoding, "gzip"))
            m_response->GZipInflate();
    }

    ThreadWorker nextWorker = nullptr;
    switch (m_response->StatusCode())
    {
        case 301: case 302:
        {
            std::string previousURL = m_response->ResolveRedirection();
            if (nullptr == m_client.RequestRedirect || m_client.RequestRedirect(m_response, m_client.UserData))
            {
                m_referer = previousURL;
                nextWorker = &WinRequest::RequestRedirect;
                break;
            }
        }
        [[fallthrough]];

        default:
            m_client.RequestComplete(m_response, m_client.UserData);
    }
    return Continue(nextWorker, true);
}

int WinRequest::RequestRedirect(void)
{
    m_request.Close();
    m_connection.Close();
    m_response->ResetForRedirection();

    int r = OpenRequest(m_response->CurrentURL());
    if (BK_ERR_SUCCESS != r)
        return r;

    return Continue(&WinRequest::SendRequest, true);
}

int WinRequest::SendBody(void)
{
    bool signal = true;

    DWORD dwWritten = 0;
    ASSERT(!m_body.empty());
    if (!m_request.Write(m_body.data(), m_body.size(), &dwWritten))
    {
        DWORD err = GetLastError();
        if (ERROR_IO_PENDING != err)
        {
            ASSERT(ERROR_IO_PENDING == err);
            return BK_ERR_NETWORK;
        }

        signal = false;
    }

    return Continue(&WinRequest::EndRequest, signal);
}

int WinRequest::SendRequest(void)
{
    std::string allHeaders = m_headers.GetAllForRequest();

    INTERNET_BUFFERSA buf = { 0 };
    buf.dwStructSize = sizeof(INTERNET_BUFFERSA);
    buf.lpcszHeader = allHeaders.data();
    buf.dwHeadersLength = allHeaders.length();
    buf.dwBufferTotal = m_body.size();
    m_request.SetOption(INTERNET_OPTION_SEND_TIMEOUT, TimeoutInMs());
    m_request.SetOption(INTERNET_OPTION_SECURITY_FLAGS, SECURITY_FLAG_IGNORE_REVOCATION);
    if (!m_request.Send(&buf))
    {
        DWORD err = GetLastError();
        if (ERROR_IO_PENDING != err)
        {
            ASSERT(ERROR_IO_PENDING == err);
            return BK_ERR_NETWORK;
        }

        int r = WaitForIOPending();
        if (BK_ERR_SUCCESS != r)
            return r;
        if (BK_ERR_SUCCESS != m_response->ErrorCode())
            return m_response->ErrorCode();
    }

    ThreadWorker nextWorker = m_body.empty() ? &WinRequest::EndRequest : &WinRequest::SendBody;
    return Continue(nextWorker, true);

}

void WinRequest::SetHeader(const char *name, const char *value)
{
    if (base::EqualsCaseInsensitiveASCII(name, "User-Agent"))
        m_userAgent = value;
    else if (base::EqualsCaseInsensitiveASCII(name, "Referer"))
        m_referer = value;
    else
        RequestImpl::SetHeader(name, value);
}

void WinRequest::StartWorkThread(void)
{
    m_response = new HttpResponse(m_URL);
    m_hEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    m_hThread = CreateThread(nullptr, 0, ThreadProc, this, 0, nullptr);
}

void CALLBACK WinRequest::StatusCallback(
    HINTERNET hInternet,
    DWORD_PTR dwContext,
    DWORD dwInternetStatus,
    PVOID lpvStatusInformation, DWORD dwStatusInformationLength)
{
    WinRequest *This = reinterpret_cast<WinRequest *>(dwContext);
    This->StatusCallback(hInternet, dwInternetStatus, lpvStatusInformation, dwStatusInformationLength);
}

void WinRequest::StatusCallback(
    HINTERNET hInternet,
    DWORD dwInternetStatus,
    PVOID lpvStatusInformation, DWORD dwStatusInformationLength)
{
    switch (dwInternetStatus)
    {
        case INTERNET_STATUS_CONNECTING_TO_SERVER:
            BKLOG("Connecting to %s ...", reinterpret_cast<PCSTR>(lpvStatusInformation));
            break;
        case INTERNET_STATUS_DETECTING_PROXY:
            BKLOG("Detecting proxy...");
            break;
        case INTERNET_STATUS_REDIRECT:
        {
            std::string URL(reinterpret_cast<const char *>(lpvStatusInformation), dwStatusInformationLength - 1);
            m_response->SetCurrentURL(URL);
            BKLOG("Redirect to: %s.", URL.c_str());
            break;
        }
        case INTERNET_STATUS_REQUEST_COMPLETE:
        {
            LPINTERNET_ASYNC_RESULT r = reinterpret_cast<LPINTERNET_ASYNC_RESULT>(lpvStatusInformation);
            if (ERROR_SUCCESS != r->dwError)
            {
                BKLOG("Request complete, error = %d.", r->dwError);
                m_response->SetErrorCode(BK_ERR_NETWORK);
            }
            else
            {
                BKLOG("Request complete.");
            }
            SetEvent(m_hEvent);
            break;
        }
    }
}

DWORD WINAPI WinRequest::ThreadProc(PVOID param)
{
    return reinterpret_cast<WinRequest *>(param)->DoThreadWork();
}

int WinRequest::WaitForIOPending(void)
{
    const HANDLE events[2] = { m_hEvent, m_hEventCancel };
    DWORD count = nullptr != m_hEventCancel ? 2 : 1;

    DWORD dwWait = WaitForMultipleObjects(count, events, FALSE, INFINITE);
    if (WAIT_OBJECT_0 + 1 == dwWait)
        return BK_ERR_CANCELLED;

    return BK_ERR_SUCCESS;
}

} // namespace BlinKit

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RequestImpl* RequestImpl::CreateInstance(const char *URL, const BkRequestClient &client)
{
    return new BlinKit::WinRequest(URL, client);
}
