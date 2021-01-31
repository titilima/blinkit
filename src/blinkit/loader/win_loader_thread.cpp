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
    reinterpret_cast<WinLoaderThread *>(param)->Run();
    return EXIT_SUCCESS;
}

std::unique_ptr<LoaderThread> LoaderThread::Create(void)
{
    return std::make_unique<WinLoaderThread>();
}

} // namespace BlinKit
