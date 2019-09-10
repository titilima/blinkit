// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_thread.cpp
// Description: WinThread Class
//      Author: Ziming Li
//     Created: 2019-03-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "win_thread.h"

#include "blink_impl/win_task_runner.h"

using namespace blink;

namespace BlinKit {

struct WinThread::CreateData {
    WinThread *thread;
    const char *name;
    HANDLE hEvent;
};

WinThread::~WinThread(void)
{
    assert(!isCurrentThread());

    PostThreadMessage(threadId(), WM_QUIT, EXIT_SUCCESS, 0);
    WaitForSingleObject(m_hThread, INFINITE);
    CloseHandle(m_hThread);
}

void WinThread::ApplyName(const char *name)
{
    struct THREADNAME_INFO {
        DWORD dwType = 0x1000;
        PCSTR szName;
        DWORD dwThreadID;
        DWORD dwFlags = 0;
    } info;

    info.szName = name;
    info.dwThreadID = GetCurrentThreadId();
    __try
    {
        RaiseException(0x406D1388, 0, sizeof(info) / sizeof(DWORD), reinterpret_cast<ULONG_PTR *>(&info));
    }
    __except (EXCEPTION_CONTINUE_EXECUTION)
    {
    }
}

void WinThread::EnsureMessageQueueCreated(HANDLE hEvent)
{
    IsGUIThread(TRUE);
    SetEvent(hEvent);
}

void WinThread::Initialize(CreateData &cd)
{
#ifndef NDEBUG
    m_name = cd.name;
#endif
    DWORD threadId;
    m_hThread = CreateThread(nullptr, 0, ThreadProc, &cd, 0, &threadId);
    ApplyThreadId(threadId);
}

DWORD WinThread::Run(void)
{
    WinTaskRunner *runner = static_cast<WinTaskRunner *>(taskRunner());

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (runner->ProcessMessage(msg))
            continue;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return EXIT_SUCCESS;
}

DWORD WINAPI WinThread::ThreadProc(PVOID param)
{
    CreateData *cd = reinterpret_cast<CreateData *>(param);

    WinThread *This = cd->thread;
    ApplyName(cd->name);
    EnsureMessageQueueCreated(cd->hEvent);
    return This->Run();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ThreadImpl* ThreadImpl::CreateInstance(const char *name)
{
    WinThread::CreateData cd;
    cd.thread = new WinThread;
    cd.name = name;
    cd.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);

    cd.thread->Initialize(cd);

    WaitForSingleObject(cd.hEvent, INFINITE);
    CloseHandle(cd.hEvent);

    return cd.thread;
}

PlatformThreadId ThreadImpl::CurrentThreadId(void)
{
    return ::GetCurrentThreadId();
}

} // namespace BlinKit
