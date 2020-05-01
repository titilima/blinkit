// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: login_proxy.cpp
// Description: LoginProxy Class
//      Author: Ziming Li
//     Created: 2020-04-25
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "login_proxy.h"

#include <iterator>
#include "base/memory/ptr_util.h"
#include "blinkit/app/app_constants.h"
#include "blinkit/crawler/login/client_request_task.h"
#include "blinkit/crawler/login/client_response_task.h"
#include "blinkit/http/request_impl.h"

namespace BlinKit {

struct RequestData {
    LoginProxy *This;
    SOCKET Client;
};

static const char EXIT[] = "EXIT";

LoginProxy::LoginProxy(const BkLoginProxyClient &client) : m_client(client)
{
}

LoginProxy::~LoginProxy(void)
{
    // The socket SHOULD BE closed before dtor,
    // because WSACleanup is called in ~WinLoginProxy (which before ~LoginProxy).
    ASSERT(m_socket == INVALID_SOCKET);
}

BkRequest LoginProxy::CreateRequest(const std::string &URL, SOCKET socket)
{
    BkRequestClient client = { 0 };
    client.SizeOfStruct = sizeof(BkRequestClient);
    client.UserData = new RequestData({ this, socket });
    client.RequestComplete = ProcessRequestComplete;
    client.RequestFailed = ProcessRequestFailed;
    return BkCreateRequest(URL.c_str(), &client);
}

std::string LoginProxy::GetLoginSuccessfulResponseData(void) const
{
    std::string headers =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Content-Type: text/html\r\n\r\n"
        ;

    std::string html;
    if (nullptr != m_client.GetConfig)
        m_client.GetConfig(BK_CFG_LOGGED_IN_HTML, BkMakeBuffer(html), m_client.UserData);
    if (html.empty())
        html.assign("<html><body>Login successfully!<body></html>");

    return headers + html;
}

bool LoginProxy::InitSocket(uint16_t port)
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
    addr.sin_addr.S_un.S_addr = INADDR_ANY;
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

    BKLOG("Start listening port %d for %s ...", static_cast<int>(port), m_loginURL.c_str());
    return true;
}

bool LoginProxy::IsLoginSuccessful(const std::string &currentURL) const
{
    if (!m_client.IsLoginSuccessful(currentURL.c_str(), m_client.UserData))
        return false;

    m_loggedIn = true;
    return true;
}

void LoginProxy::ProcessRequestComplete(BkResponse response, SOCKET client)
{
    ClientResponseTask *task = new ClientResponseTask(response, client);
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_tasks.push(task);
        m_cond.notify_one();
    }
}

void BKAPI LoginProxy::ProcessRequestComplete(BkResponse response, void *userData)
{
    RequestData *requestData = reinterpret_cast<RequestData *>(userData);
    requestData->This->ProcessRequestComplete(response, requestData->Client);
    delete requestData;
}

void BKAPI LoginProxy::ProcessRequestFailed(int errorCode, void *userData)
{
    ASSERT(false); // BKTODO:
}

int LoginProxy::Run(const char *loginURL, uint16_t port)
{
    m_loginURL.assign(loginURL);

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

        SOCKET exitSocket = socket(AF_INET, SOCK_STREAM, 0);        
        sockaddr_in addr = { 0 };
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
        connect(exitSocket, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
        send(exitSocket, EXIT, std::size(EXIT) - 1, 0);
        closesocket(exitSocket);
    } while (false);

    return r;
}

void LoginProxy::RunListeningThread(void)
{
    for (;;)
    {
        SOCKET clientSocket = accept(m_socket, nullptr, nullptr);

        char lead[5];
        int size = recv(clientSocket, lead, std::size(lead), 0);
        if (size == std::size(EXIT) - 1 && 0 == strncmp(lead, EXIT, size))
        {
            closesocket(clientSocket);
            break;
        }

        std::unique_ptr<ClientRequestTask> task = base::WrapUnique(new ClientRequestTask(clientSocket, lead, size));

#if 0
        if (taskData->IsTunnelConnection())
        {
            BKLOG("Tunnel to %s ...", taskData->RequestArg());
            ASSERT(false); // BKTODO:
            static const char ConnectionEstablished[] = "HTTP/1.1 200 Connection Established\r\nConnection: close\r\n\r\n";
            send(clientSocket, ConnectionEstablished, std::size(ConnectionEstablished) - 1, 0);
            shutdown(clientSocket, SD_BOTH);
            closesocket(clientSocket);
            continue;
        }
#endif

        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_tasks.push(task.release());
            m_cond.notify_one();
        }
    }

    closesocket(m_socket);
    m_socket = INVALID_SOCKET;
}

int LoginProxy::RunTaskLoop(void)
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

        task->Execute(*this);
        delete task;
    }
    return 0;
}

void LoginProxy::SetCookie(const std::string &cookie)
{
    m_client.SetCookie(cookie.c_str(), m_client.UserData);
}

const std::string& LoginProxy::UserAgent(void) const
{
    if (m_userAgent.empty())
    {
        if (nullptr != m_client.GetConfig)
            m_client.GetConfig(BK_CFG_USER_AGENT, BkMakeBuffer(m_userAgent), m_client.UserData);
        if (m_userAgent.empty())
            m_userAgent.assign(AppConstants::DefaultUserAgent);
    }
    return m_userAgent;
}

} // namespace BlinKit

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT int BKAPI BkRunLoginProxy(const char *loginURL, uint16_t port, BkLoginProxyClient *client)
{
    using namespace BlinKit;
    std::unique_ptr<LoginProxy> loginProxy = LoginProxy::Create(*client);
    return loginProxy->Run(loginURL, port);
}

} // extern "C"
