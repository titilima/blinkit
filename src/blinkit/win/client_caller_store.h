// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: client_caller_store.h
// Description: ClientCallerStore Class
//      Author: Ziming Li
//     Created: 2021-01-29
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_CLIENT_CALLER_STORE_H
#define BLINKIT_BLINKIT_CLIENT_CALLER_STORE_H

#pragma once

#include "blinkit/app/caller.h"

namespace BlinKit {

class ClientCallerStore
{
public:
    virtual ClientCaller& Acquire(void) = 0;
};

#ifdef BLINKIT_CRAWLER_ONLY
class SingletonClientCallerStore : public ClientCallerStore, public ClientCaller
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

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CLIENT_CALLER_STORE_H
