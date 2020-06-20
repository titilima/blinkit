// -------------------------------------------------
// BlinKit - BkBase Library
// -------------------------------------------------
//   File Name: win_request.h
// Description: WinRequest Class
//      Author: Ziming Li
//     Created: 2018-09-27
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_WIN_REQUEST_H
#define BLINKIT_BLINKIT_WIN_REQUEST_H

#pragma once

#include "request_impl.h"
#include "win/inet.h"

namespace BlinKit {

class WinRequest final : public RequestImpl
{
public:
    WinRequest(const char *URL, const BkRequestClient &client);
    ~WinRequest(void) override;
private:
    bool OpenSession(void);
    int OpenRequest(const std::string &URL);
    int WaitForIOPending(void);
    void StatusCallback(HINTERNET hInternet, DWORD dwInternetStatus,
        PVOID lpvStatusInformation, DWORD dwStatusInformationLength);
    static void CALLBACK StatusCallback(HINTERNET hInternet, DWORD_PTR dwContext, DWORD dwInternetStatus,
        PVOID lpvStatusInformation, DWORD dwStatusInformationLength);

    typedef int (WinRequest::*ThreadWorker)(void);
    void StartWorkThread(void);
    ThreadWorker DetachThreadWorker(void);
    int Continue(ThreadWorker nextWorker, bool signal);
    int SendRequest(void);
    int SendBody(void);
    int EndRequest(void);
    int QueryRequest(void);
    int ReceiveData(void);
    int RequestComplete(void);
    int RequestRedirect(void);
    DWORD DoThreadWork(void);
    static DWORD WINAPI ThreadProc(PVOID param);

    // RequestImpl
    int Perform(void) override;
    void SetHeader(const char *name, const char *value) override;
    ControllerImpl* GetController(void) override;
    void Cancel(void) override;

    std::string m_userAgent, m_referer;
    InetSession m_session;
    InetConnection m_connection;
    InetRequest m_request;

    HANDLE m_hEvent = nullptr;
    HANDLE m_hEventCancel = nullptr;
    HANDLE m_hThread = nullptr;
    ThreadWorker m_nextWorker = nullptr;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_WIN_REQUEST_H
