// -------------------------------------------------
// BlinKit - BkHelper Library
// -------------------------------------------------
//   File Name: login_proxy_impl.cpp
// Description: ProxyImpl Class
//      Author: Ziming Li
//     Created: 2020-04-25
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "proxy_impl.h"

#include <iterator>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include "bkcommon/bk_file.h"
#include "bkcommon/bk_strings.h"
#include "bkcommon/buffer_impl.hpp"
#include "bkhelper/proxy/proxy_globals.h"
#include "bkhelper/proxy/tasks/http_request_task.h"
#include "bkhelper/ssl/ssl_pair.h"

using namespace BlinKit;

static const size_t ExitStringLength = 4;

static std::string GenerateExitString(void)
{
    std::string ret;
    srand(static_cast<unsigned>(time(nullptr)));
    for (size_t i = 0; i < ExitStringLength; ++i)
    {
        int n = rand() % 62;
        if (0 <= n && n < 26)
        {
            ret.push_back('a' + n);
            continue;
        }

        n -= 26;
        if (0 <= n && n < 26)
        {
            ret.push_back('A' + n);
            continue;
        }

        n -= 26;
        ASSERT(0 <= n && n < 10);
        ret.push_back('0' + n);
    }
    return ret;
}

ProxyImpl::ProxyImpl(const BkProxyClient &client) : m_client(client), m_exitString(GenerateExitString())
{
}

ProxyImpl::~ProxyImpl(void)
{
    // The socket SHOULD BE closed before dtor,
    // because WSACleanup is called in ~WinLoginProxy (which before ~LoginProxy).
    ASSERT(m_socket == INVALID_SOCKET);

    for (auto &it : m_sslData)
        SSL_CTX_free(it.second);
}

void ProxyImpl::AddTask(ProxyTask *task)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_tasks.push(task);
    m_cond.notify_one();
}

int ProxyImpl::ApplyPEM(const std::string &privateKey, const std::string &cert)
{
    ProxyGlobals::EnsureSSLInitialized();

    m_caData = std::make_unique<SSLPair>(privateKey, cert);
    if (!m_caData->IsValid())
    {
        m_caData.reset();
        return BK_ERR_FORBIDDEN;
    }
    return BK_ERR_SUCCESS;
}

void ProxyImpl::ExitLoop(void)
{
    m_running = false;
    StopListeningThread();
}

std::string ProxyImpl::GetExitBody(void) const
{
    std::string ret;
    if (nullptr != m_client.GetConfig)
        m_client.GetConfig(BK_PROXY_CFG_EXIT_BODY, BufferImpl::Wrap(ret), m_client.UserData);
    return ret;
}

bool ProxyImpl::InitSocket(uint16_t port)
{
    ASSERT(INVALID_SOCKET == m_socket);

    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (INVALID_SOCKET == m_socket)
    {
        BKLOG("Create socket failed!");
        return false;
    }

    m_port = port;

    sockaddr_in addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port);
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

SSL* ProxyImpl::NewSSL(const std::string &domain)
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

bool ProxyImpl::PreProcessRequestComplete(BkResponse response)
{
    m_client.RequestComplete(response, m_client.UserData);
    return m_running;
}

bool ProxyImpl::PreProcessRequestPerform(RequestTaskBase *requestTask)
{
    m_client.BeforeRequestPerform(requestTask, m_client.UserData);
    return m_running;
}

int ProxyImpl::RunLoop(uint16_t port)
{
    int r = BK_ERR_SUCCESS;
    do {
        if (!InitSocket(port))
        {
            r = BK_ERR_NETWORK;
            break;
        }

        if (!StartListeningThread())
        {
            r = BK_ERR_UNKNOWN;
            break;
        }

        RunTaskLoop();
    } while (false);
    delete this;
    return r;
}

void ProxyImpl::RunListeningThread(void)
{
    for (;;)
    {
        SOCKET clientSocket = accept(m_socket, nullptr, nullptr);

        char lead[ExitStringLength + 1] = { 0 };
        int size = recv(clientSocket, lead, std::size(lead), 0);
        if (size < 0)
        {
            closesocket(clientSocket);
            continue;
        }

        if (ExitStringLength == size)
        {
            lead[ExitStringLength] = '\0';
            if (m_exitString == lead)
            {
                closesocket(clientSocket);
                break;
            }
        }

        AddTask(new HTTPRequestTask(clientSocket, lead, size));
    }

    closesocket(m_socket);
    m_socket = INVALID_SOCKET;
}

void ProxyImpl::RunTaskLoop(void)
{
    while (m_running)
    {
        ProxyTask *task = nullptr;
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            while (m_tasks.empty())
                m_cond.wait(lock);
            task = m_tasks.front();
            m_tasks.pop();
        }

        do {
            std::unique_ptr<ProxyTask> currentTask(task);
            task = currentTask->Execute(*this);
        } while (nullptr != task);
    }
}

int ProxyImpl::SetupCA(const BkPathChar *privateKeyFile, const BkPathChar *certFile)
{
    std::string privateKey, cert;

    int r = BkFile::ReadContent(privateKeyFile, BufferImpl::Wrap(privateKey));
    if (BK_ERR_SUCCESS != r)
    {
        ASSERT(BK_ERR_SUCCESS == r);
        return r;
    }

    r = BkFile::ReadContent(certFile, BufferImpl::Wrap(cert));
    if (BK_ERR_SUCCESS != r)
    {
        ASSERT(BK_ERR_SUCCESS == r);
        return r;
    }

    return ApplyPEM(privateKey, cert);
}

void ProxyImpl::StopListeningThread(void)
{
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    connect(s, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
    send(s, m_exitString.data(), m_exitString.length(), 0);
    shutdown(s, SD_BOTH);

    closesocket(s);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT void BKAPI BkExitProxyLoop(BkProxy proxy)
{
    proxy->ExitLoop();
}

BKEXPORT int BKAPI BkRunProxyLoop(BkProxy proxy, uint16_t port)
{
    return proxy->RunLoop(port);
}

BKEXPORT int BKAPI BkSetupProxyCA(BkProxy proxy, const BkPathChar *privateKeyFile, const BkPathChar *certFile)
{
    return proxy->SetupCA(privateKeyFile, certFile);
}

} // extern "C"
