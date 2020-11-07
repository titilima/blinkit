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
#include "blinkit/app/app_impl.h"
#include "blinkit/loader/loader_thread.h"
#include "blinkit/loader/tasks/http_loader_task.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_request.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "blinkit/loader/tasks/res_loader_task.h"
#endif

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
            if (request.ForCrawler())
                task = new HTTPLoaderTask(request, m_taskRunner, client);
            else
                NOTREACHED();
            break;
        }

#ifndef BLINKIT_CRAWLER_ONLY
        if (url.SchemeIs("res"))
        {
            task = new ResLoaderTask(request, m_taskRunner, client);
            break;
        }
#endif
    } while (false);

    int error = BK_ERR_URI;
    if (nullptr != task)
    {
        error = task->PreProcess();
        if (BK_ERR_CANCELLED == error)
            return;
    }

    if (BK_ERR_SUCCESS == error)
        AppImpl::Get().GetLoaderThread().AddTask(task);
    else
        LoaderTask::ReportError(client, m_taskRunner.get(), error, url);
}

} // namespace BlinKit
