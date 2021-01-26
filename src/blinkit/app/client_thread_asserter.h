// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: client_thread_asserter.h
// Description: ClientThreadAsserter Classes
//      Author: Ziming Li
//     Created: 2021-01-26
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_CLIENT_THREAD_ASSERTER_H
#define BLINKIT_BLINKIT_CLIENT_THREAD_ASSERTER_H

#pragma once

#include "blinkit/blink_impl/thread_impl.h"

namespace BlinKit {

#ifndef NDEBUG
class ClientThreadAsserter
{
public:
    void AttachClientThread(void)
    {
        ASSERT(0 == m_clientThreadId);
        m_clientThreadId = ThreadImpl::CurrentThreadId();
    }
protected:
    bool IsClientThread(void) const { return ThreadImpl::CurrentThreadId() == m_clientThreadId; }
private:
    blink::PlatformThreadId m_clientThreadId = 0;
};
#endif

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CLIENT_THREAD_ASSERTER_H
