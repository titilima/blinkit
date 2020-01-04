// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: loader_task.cpp
// Description: LoaderTask Class
//      Author: Ziming Li
//     Created: 2019-03-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "loader_task.h"

#include "base/single_thread_task_runner.h"

using namespace blink;

namespace BlinKit {

LoaderTask::LoaderTask(const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner, WebURLLoaderClient *client)
    : m_taskRunner(taskRunner), m_client(client)
{
}

LoaderTask::~LoaderTask(void) = default;

void LoaderTask::ReportError(base::SingleThreadTaskRunner *taskRunner, int error)
{
    ASSERT(false); // BKTODO:
}

} // namespace BlinKit
