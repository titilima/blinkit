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
#include "third_party/blink/public/platform/web_url_loader_client.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_error.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"

using namespace blink;

namespace BlinKit {

LoaderTask::LoaderTask(const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner, WebURLLoaderClient *client)
    : m_taskRunner(taskRunner), m_client(client)
{
}

LoaderTask::~LoaderTask(void) = default;

static void ErrorWorker(WebURLLoaderClient *client, const ResourceError error)
{
    ASSERT(IsMainThread());
    client->DidFail(error);
}

void LoaderTask::ReportError(WebURLLoaderClient *client, base::SingleThreadTaskRunner *taskRunner, int errorCode, const BkURL &URL)
{
    ResourceError error(errorCode, URL);
    std::function<void()> callback = std::bind(&ErrorWorker, client, error);
    taskRunner->PostTask(FROM_HERE, callback);
}

} // namespace BlinKit
