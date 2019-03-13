// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_thread.h
// Description: WinThread Class
//      Author: Ziming Li
//     Created: 2019-03-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_WIN_THREAD_H
#define BLINKIT_BLINKIT_WIN_THREAD_H

#pragma once

#include "thread_impl.h"

namespace BlinKit {

class WinThread final : public ThreadImpl
{
public:
    ~WinThread(void);

    struct CreateData;
    void Initialize(CreateData &cd);
private:
    static void ApplyName(const char *name);
    static void EnsureMessageQueueCreated(HANDLE hEvent);
    DWORD Run(void);
    static DWORD WINAPI ThreadProc(PVOID param);

#ifdef _DEBUG
    std::string m_name;
#endif
    HANDLE m_hThread = nullptr;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_WIN_THREAD_H
