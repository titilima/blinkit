// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: thread_impl.cpp
// Description: ThreadImpl Class
//      Author: Ziming Li
//     Created: 2019-03-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "thread_impl.h"

using namespace blink;

namespace BlinKit {

ThreadImpl::~ThreadImpl(void) = default;

bool ThreadImpl::IsCurrentThread(void) const
{
    return CurrentThreadId() == ThreadId();
}

} // namespace BlinKit
