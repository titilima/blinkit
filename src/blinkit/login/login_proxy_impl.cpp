// -------------------------------------------------
// BlinKit - BkLogin Library
// -------------------------------------------------
//   File Name: login_proxy_impl.cpp
// Description: LoginProxyImpl Class
//      Author: Ziming Li
//     Created: 2020-04-25
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "login_proxy_impl.h"

#include <iterator>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include "blinkit/app/app_constants.h"
#include "blinkit/common/bk_file.h"
#include "blinkit/http/request_impl.h"
#include "blinkit/login/login_globals.h"
#include "blinkit/login/ssl/ssl_pair.h"
#include "blinkit/login/tasks/http_request_task.h"

using namespace BlinKit;

static const char EXIT[] = "EXIT";

LoginProxyImpl::LoginProxyImpl(const BkLoginProxyClient &client) : m_client(client)
{
}

LoginProxyImpl::~LoginProxyImpl(void)
{
    // The socket SHOULD BE closed before dtor,
    // because WSACleanup is called in ~WinLoginProxy (which before ~LoginProxy).
    ASSERT(m_socket == INVALID_SOCKET);

    for (auto &it : m_sslData)
        SSL_CTX_free(it.second);
}

void LoginProxyImpl::AddTask(LoginTask *task)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_tasks.push(task);
    m_cond.notify_one();
}

int LoginProxyImpl::ApplyPEM(const std::string &privateKey, const std::string &cert)
{
    LoginGlobals::EnsureSSLInitialized();

    m_caData = std::make_unique<SSLPair>(privateKey, cert);
    if (!m_caData->IsValid())
    {
        m_caData.reset();
        return BK_ERR_FORBIDDEN;
    }
    return BK_ERR_SUCCESS;
}

BkRequest LoginProxyImpl::CreateRequest(const std::string &URL, const BkRequestClient &client)
{
    return RequestImpl::CreateInstance(URL.c_str(), client);
}

std::string LoginProxyImpl::GetLoggedInHTML(void) const
{
    std::string ret;
    if (nullptr != m_client.GetConfig)
        m_client.GetConfig(BK_LPCFG_LOGGED_IN_HTML, BkMakeBuffer(ret), m_client.UserData);
    if (ret.empty())
        ret.assign("<html><body>Login successfully!<body></html>");
    return ret;
}

bool LoginProxyImpl::InitSocket(uint16_t port)
{
    ASSERT(INVALID_SOCKET == m_socket);

    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (INVALID_SOCKET == m_socket)
    {
        BKLOG("Create socket failed!");
        return false;
    }

    sockaddr_in addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(m_socket, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == SOCKET_ERROR)
    {
        BKLOG("Bind socket failed!");
        return false;
    }

    if (listen(m_socket, SOMAXCONN) == SOCKET_ERROR)
    {
        BKLOG("Socket listen failed!");
        return false;
    }

    BKLOG("Start listening port %d ...", static_cast<int>(port));
    return true;
}

bool LoginProxyImpl::IsLoginSuccessful(const std::string &currentURL) const
{
    if (!m_client.IsLoginSuccessful(currentURL.c_str(), m_client.UserData))
        return false;

    m_loggedIn = true;
    return true;
}

SSL* LoginProxyImpl::NewSSL(const std::string &domain)
{
    SSL_CTX *ctx = nullptr;

    auto it = m_sslData.find(domain);
    if (std::end(m_sslData) == it)
    {
        ctx = m_caData->NewDomainContext(domain);
        if (nullptr == ctx)
            return nullptr;

        m_sslData[domain] = ctx;
    }
    else
    {
        ctx = it->second;
    }

    return SSL_new(ctx);
}

int LoginProxyImpl::Run(uint16_t port)
{
    int r = BK_ERR_UNKNOWN;
    do {
        if (!InitSocket(port))
        {
            r = BK_ERR_NETWORK;
            break;
        }

        if (!StartListeningThread())
            break;

        r = RunTaskLoop();

        StopListeningThread(port);
    } while (false);
    return r;
}

void LoginProxyImpl::RunListeningThread(void)
{
    for (;;)
    {
        SOCKET clientSocket = accept(m_socket, nullptr, nullptr);

        char lead[5];
        int size = recv(clientSocket, lead, std::size(lead), 0);
        if (size < 0)
        {
            closesocket(clientSocket);
            continue;
        }

        if (size == std::size(EXIT) - 1 && 0 == strncmp(lead, EXIT, size))
        {
            closesocket(clientSocket);
            break;
        }

        AddTask(new HTTPRequestTask(clientSocket, lead, size));
    }

    closesocket(m_socket);
    m_socket = INVALID_SOCKET;
}

int LoginProxyImpl::RunTaskLoop(void)
{
    while (!m_loggedIn)
    {
        LoginTask *task = nullptr;
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            while (m_tasks.empty())
                m_cond.wait(lock);
            task = m_tasks.front();
            m_tasks.pop();
        }

        do {
            std::unique_ptr<LoginTask> currentTask(task);
            task = currentTask->Execute(*this);
        } while (nullptr != task);
    }
    return 0;
}

void LoginProxyImpl::SetCookie(const std::string &URL, const std::string &cookie)
{
    m_client.SetCookie(URL.c_str(), cookie.c_str(), m_client.UserData);
}

int LoginProxyImpl::SetupCA(const BkPathChar *privateKeyFile, const BkPathChar *certFile)
{
    std::string privateKey, cert;

    int r = BkFile::ReadContent(privateKeyFile, BkMakeBuffer(privateKey));
    if (BK_ERR_SUCCESS != r)
    {
        ASSERT(BK_ERR_SUCCESS == r);
        return r;
    }

    r = BkFile::ReadContent(certFile, BkMakeBuffer(cert));
    if (BK_ERR_SUCCESS != r)
    {
        ASSERT(BK_ERR_SUCCESS == r);
        return r;
    }

    return ApplyPEM(privateKey, cert);
}

void LoginProxyImpl::StopListeningThread(uint16_t port)
{
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    connect(s, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
    send(s, EXIT, std::size(EXIT) - 1, 0);
    shutdown(s, SD_BOTH);

    closesocket(s);
}

const std::string& LoginProxyImpl::UserAgent(void) const
{
    if (m_userAgent.empty())
    {
        if (nullptr != m_client.GetConfig)
            m_client.GetConfig(BK_LPCFG_USER_AGENT, BkMakeBuffer(m_userAgent), m_client.UserData);
        if (m_userAgent.empty())
            m_userAgent.assign(AppConstants::DefaultUserAgent);
    }
    return m_userAgent;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT void BKAPI BkDestroyLoginProxy(BkLoginProxy loginProxy)
{
    delete loginProxy;
}

BKEXPORT int BKAPI BkRunLoginProxy(BkLoginProxy loginProxy, uint16_t port)
{
    return loginProxy->Run(port);
}

BKEXPORT int BKAPI BkSetupLoginProxyCA(BkLoginProxy loginProxy, const BkPathChar *privateKeyFile, const BkPathChar *certFile)
{
    return loginProxy->SetupCA(privateKeyFile, certFile);
}

} // extern "C"
