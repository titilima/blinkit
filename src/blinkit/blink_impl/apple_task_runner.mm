// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: apple_task_runner.mm
// Description: AppleTaskRunner Class
//      Author: Ziming Li
//     Created: 2019-08-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "apple_task_runner.h"

using namespace blink;

namespace BlinKit {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<WebTaskRunner> TaskRunnerImpl::CreateInstance(ThreadImpl &thread)
{
    assert(false); // BKTODO:
    return nullptr;
}

} // namespace BlinKit
