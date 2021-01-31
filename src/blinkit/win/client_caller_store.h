// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: client_caller_store.h
// Description: ClientCallerStore Classes
//      Author: Ziming Li
//     Created: 2021-01-29
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_CLIENT_CALLER_STORE_H
#define BLINKIT_BLINKIT_CLIENT_CALLER_STORE_H

#pragma once

#include <unordered_map>
#include "bkcommon/bk_mutex.hpp"
#include "blinkit/app/caller.h"

namespace BlinKit {

class ClientCallerStore
{
public:
    virtual ClientCaller& Acquire(void) = 0;
};

#ifdef BLINKIT_CRAWLER_ONLY
class SingletonClientCallerStore final : public ClientCallerStore, public ClientCaller
{
public:
    SingletonClientCallerStore(void) : m_threadId(GetCurrentThreadId()) {}
private:
    ClientCaller& Acquire(void) override { return *this; }
    void Post(const base::Location &loc, std::function<void()> &&task) override;
#ifndef NDEBUG
    bool IsClientThread(void) const override;
#endif

    const DWORD m_threadId;
};
#endif

class ClientCallerStoreImpl final : public ClientCallerStore
{
public:
    ClientCallerStoreImpl(void);
    ~ClientCallerStoreImpl(void);

    ClientCaller& Acquire(void) override;
private:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    ATOM m_atom;
    BkMutex m_lock;

    class ClientCallerImpl;
    std::unordered_map<DWORD, ClientCallerImpl *> m_callerMap;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CLIENT_CALLER_STORE_H
