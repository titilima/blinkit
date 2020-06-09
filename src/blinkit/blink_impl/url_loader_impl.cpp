// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: url_loader_impl.cpp
// Description: URLLoaderImpl Class
//      Author: Ziming Li
//     Created: 2019-03-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "url_loader_impl.h"

#include "base/single_thread_task_runner.h"
#include "blinkit/loader_tasks/http_loader_task.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_request.h"

using namespace blink;

namespace BlinKit {

URLLoaderImpl::URLLoaderImpl(const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner) : m_taskRunner(taskRunner)
{
    // Nothing
}

URLLoaderImpl::~URLLoaderImpl(void) = default;

void URLLoaderImpl::LoadAsynchronously(const ResourceRequest &request, WebURLLoaderClient *client)
{
    const GURL &url = request.Url();

    LoaderTask *task = nullptr;
    do {
        if (url.SchemeIsHTTPOrHTTPS())
        {
            ASSERT(nullptr != request.Crawler());
            task = new HTTPLoaderTask(request.Crawler(), m_taskRunner, client);
        }

    } while (false);

    int error = BK_ERR_URI;
    if (nullptr != task)
        error = task->Run(request);
    if (BK_ERR_SUCCESS != error)
        LoaderTask::ReportError(client, m_taskRunner.get(), error, url);
}

} // namespace BlinKit
