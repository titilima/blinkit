// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: client_caller_store.cpp
// Description: ClientCallerStore Class
//      Author: Ziming Li
//     Created: 2021-01-29
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "client_caller_store.h"

#include "blinkit/win/message_task.h"

namespace BlinKit {

#ifdef BLINKIT_CRAWLER_ONLY
#ifndef NDEBUG
bool SingletonClientCallerStore::IsClientThread(void) const
{
    return GetCurrentThreadId() == m_threadId;
}
#endif

void SingletonClientCallerStore::Post(const base::Location &, std::function<void()> &&task)
{
    MessageTask::Post(m_threadId, std::move(task));
}
#endif

} // namespace BlinKit
