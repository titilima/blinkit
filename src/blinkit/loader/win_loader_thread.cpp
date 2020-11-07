// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_loader_thread.cpp
// Description: WinLoaderThread Class
//      Author: Ziming Li
//     Created: 2020-11-05
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "win_loader_thread.h"

namespace BlinKit {

WinLoaderThread::WinLoaderThread(void)
{
    m_hThread = CreateThread(nullptr, 0, ThreadProc, this, 0, nullptr);
}

WinLoaderThread::~WinLoaderThread(void)
{
    Exit();
    WaitForSingleObject(m_hThread, INFINITE);
    CloseHandle(m_hThread);
}

DWORD WINAPI WinLoaderThread::ThreadProc(PVOID param)
{
#ifndef NDEBUG
    struct THREADNAME_INFO {
        DWORD dwType = 0x1000;
        PCSTR szName;
        DWORD dwThreadID;
        DWORD dwFlags = 0;
    } info;

    info.szName = "Loader Thread";
    info.dwThreadID = GetCurrentThreadId();
    __try
    {
        RaiseException(0x406D1388, 0, sizeof(info) / sizeof(DWORD), reinterpret_cast<ULONG_PTR *>(&info));
    }
    __except (EXCEPTION_CONTINUE_EXECUTION)
    {
    }
#endif

    reinterpret_cast<WinLoaderThread *>(param)->Run();
    return EXIT_SUCCESS;
}

std::unique_ptr<LoaderThread> LoaderThread::Create(void)
{
    return std::make_unique<WinLoaderThread>();
}

} // namespace BlinKit
