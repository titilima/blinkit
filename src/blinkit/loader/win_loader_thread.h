// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_loader_thread.h
// Description: WinLoaderThread Class
//      Author: Ziming Li
//     Created: 2020-11-05
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_WIN_LOADER_THREAD_H
#define BLINKIT_BLINKIT_WIN_LOADER_THREAD_H

#pragma once

#include "blinkit/loader/loader_thread.h"

namespace BlinKit {

class WinLoaderThread final : public LoaderThread
{
public:
    WinLoaderThread(void);
    ~WinLoaderThread(void) override;
private:
    static DWORD WINAPI ThreadProc(PVOID param);

    HANDLE m_hThread = nullptr;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_WIN_LOADER_THREAD_H
