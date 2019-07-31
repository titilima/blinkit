// -------------------------------------------------
// BlinKit - BlinKit Library
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
#include "url/gurl.h"

#include "app/app_constants.h"
#include "http/response_impl.h"

namespace BlinKit {

WinRequest::WinRequest(const char *URL, BkRequestClient &client)
    : RequestImpl(URL, client)
    , m_userAgent(AppConstants::DefaultUserAgent)
    , m_session(this)
{
    // Nothing
}

WinRequest::~WinRequest(void)
{
    assert(nullptr == m_hEventCancel);
    assert(nullptr == m_hThread);

    m_request.Close();
    m_connection.Close();
    m_session.Close();

    if (nullptr != m_hEvent)
        CloseHandle(m_hEvent);
}

void WinRequest::Cancel(void)
{
    assert(nullptr != m_hEventCancel);
    SetEvent(m_hEventCancel);
    RequestImpl::Release();
}

int WinRequest::Continue(ThreadWorker nextWorker)
{
    m_nextWorker = nextWorker;
    SetEvent(m_hEvent);
    return BkError::Success;
}

WinRequest::ThreadWorker WinRequest::DetachThreadWorker(void)
{
    ThreadWorker worker = m_nextWorker;
    m_nextWorker = nullptr;
    return worker;
}

DWORD WinRequest::DoThreadWork(void)
{
    const HANDLE events[2] = { m_hEvent, m_hEventCancel };
    DWORD count = nullptr != m_hEventCancel ? 2 : 1;

    for (;;)
    {
        DWORD dwWait = WaitForMultipleObjects(count, events, FALSE, INFINITE);
        if (WAIT_OBJECT_0 + 1 == dwWait)
            m_response->SetErrorCode(BkError::Cancelled);

        if (BkError::Success != m_response->ErrorCode())
            break;

        ThreadWorker worker = DetachThreadWorker();
        if (nullptr == worker)
            break;

        int r = (this->*worker)();
        if (BkError::Success != r)
        {
            m_response->SetErrorCode(r);
            break;
        }
    }

    if (nullptr != m_hEventCancel)
    {
        CloseHandle(m_hEventCancel);
        m_hEventCancel = nullptr;
    }

    if (BkError::Success != m_response->ErrorCode())
        m_client.RequestFailed(m_response->ErrorCode());

    CloseHandle(m_hThread);
    m_hThread = nullptr;

    RequestImpl::Release();
    return EXIT_SUCCESS;
}

int WinRequest::EndRequest(void)
{
    m_nextWorker = &WinRequest::QueryRequest;
    if (m_request.End())
    {
        SetEvent(m_hEvent);
    }
    else
    {
        DWORD err = GetLastError();
        if (ERROR_IO_PENDING != err)
        {
            assert(ERROR_IO_PENDING == err);
            return BkError::UnknownError;
        }
    }
    return BkError::Success;
}

bool WinRequest::OpenSession(void)
{
    std::string proxy;
    DWORD proxyType = INTERNET_OPEN_TYPE_DIRECT;
    if (m_client.UseProxy())
    {
        m_client.GetProxy(BkMakeBuffer(proxy).Wrap());
        if (proxy.empty())
            proxyType = INTERNET_OPEN_TYPE_PRECONFIG;
        else
            proxyType = INTERNET_OPEN_TYPE_PROXY;
    }

    if (!m_session.Open(m_userAgent, proxyType, proxy))
        return false;

    m_session.SetStatusCallback(StatusCallback);
    m_session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, TimeoutInMs());
    return true;
}

int BKAPI WinRequest::Perform(void)
{
    if (m_session.IsValid())
        return BkError::Forbidden;

    if (!OpenSession())
        return BkError::NetworkError;

    GURL URL(m_URL);
    if (!URL.SchemeIsHTTPOrHTTPS())
        return BkError::URIError;

    m_connection = m_session.Connect(URL.host(), URL.EffectiveIntPort(), URL.username(), URL.password());
    if (!m_connection.IsValid())
        return BkError::NetworkError;

    m_request = m_connection.OpenRequest(m_method, URL.PathForRequest(), m_referer, URL.SchemeIs("https"));
    if (!m_request.IsValid())
        return BkError::NetworkError;

    assert(m_allHeaders.empty());
    m_allHeaders = GetAllHeaders();

    INTERNET_BUFFERSA buf = { 0 };
    buf.dwStructSize = sizeof(INTERNET_BUFFERSA);
    buf.lpcszHeader = m_allHeaders.data();
    buf.dwHeadersLength = m_allHeaders.length();
    buf.dwBufferTotal = m_body.size();
    m_request.SetOption(INTERNET_OPTION_SEND_TIMEOUT, TimeoutInMs());
    m_request.SetOption(INTERNET_OPTION_SECURITY_FLAGS, SECURITY_FLAG_IGNORE_REVOCATION);
    if (m_request.Send(&buf))
    {
        SetEvent(m_hEvent);
    }
    else
    {
        DWORD err = GetLastError();
        if (ERROR_IO_PENDING != err)
        {
            assert(ERROR_IO_PENDING == err);
            delete this;
            return BkError::NetworkError;
        }
    }

    if (m_body.empty())
        m_nextWorker = &WinRequest::EndRequest;
    else
        m_nextWorker = &WinRequest::SendData;
    StartWorkThread();
    return BkError::Success;
}

int WinRequest::QueryRequest(void)
{
    std::string rawHeaders;
    if (!m_request.QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, rawHeaders))
        return BkError::UnknownError;

    m_response->ParseHeaders(rawHeaders);

    DWORD contentLength = 0;
    if (m_request.QueryInfo(HTTP_QUERY_CONTENT_LENGTH, contentLength))
        m_response->PrepareBody(contentLength);

    m_request.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, TimeoutInMs());
    return Continue(&WinRequest::ReceiveData);
}

int WinRequest::ReceiveData(void)
{
    const HANDLE events[2] = { m_hEvent, m_hEventCancel };
    DWORD count = nullptr != m_hEventCancel ? 2 : 1;
 
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
                assert(ERROR_IO_PENDING == err);
                return BkError::NetworkError;
            }

            DWORD dwWait = WaitForMultipleObjects(count, events, FALSE, INFINITE);
            if (WAIT_OBJECT_0 + 1 == dwWait)
                return BkError::Cancelled;
        }

        if (ib.dwBufferLength > 0)
            m_response->AppendData(buf, ib.dwBufferLength);
        else
            done = true;
    }

    return Continue(&WinRequest::RequestComplete);
}

int WinRequest::RequestComplete(void)
{
    std::string contentEncoding;
    if (BkError::Success == m_response->GetHeader("Content-Encoding", BkMakeBuffer(contentEncoding).Wrap()))
    {
        if (base::EqualsCaseInsensitiveASCII(contentEncoding, "gzip"))
            m_response->GZipInflate();
    }
    m_client.RequestComplete(*m_response);
    return Continue(nullptr);
}

BkRequestController* BKAPI WinRequest::RequireLifecycleController(void)
{
    assert(nullptr == m_hEventCancel);
    m_hEventCancel = CreateEvent(nullptr, TRUE, FALSE, nullptr);

    return RequestImpl::RequireLifecycleController();
}

int WinRequest::SendData(void)
{
    assert(!m_body.empty());

    DWORD dwWritten = 0;
    if (m_request.Write(m_body.data(), m_body.size(), &dwWritten))
        return Continue(&WinRequest::EndRequest);

    assert(false);
    return BkError::NetworkError;
}

void BKAPI WinRequest::SetHeader(const char *name, const char *value)
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
    m_response = new ResponseImpl(m_URL);
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
                m_response->SetErrorCode(BkError::NetworkError);
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BkRequest* RequestImpl::CreateInstance(const char *URL, BkRequestClient &client)
{
    return new WinRequest(URL, client);
}

} // namespace BlinKit
