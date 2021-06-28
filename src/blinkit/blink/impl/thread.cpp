// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: thread.cpp
// Description: Thread Class
//      Author: Ziming Li
//     Created: 2019-03-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "./thread.h"

using namespace blink;

namespace BlinKit {

bool Thread::isCurrentThread(void) const
{
    return CurrentThreadId() == threadId();
}

} // namespace BlinKit
