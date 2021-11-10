// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: url_loader.cpp
// Description: URLLoader Class
//      Author: Ziming Li
//     Created: 2019-03-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "./url_loader.h"

#include "blinkit/app/app_impl.h"
#include "blinkit/loader/loader_thread.h"
#ifdef BLINKIT_CRAWLER_ENABLED
#   include "blinkit/loader/tasks/http_loader_task.h"
#endif
#ifdef BLINKIT_UI_ENABLED
#   include "blinkit/loader/tasks/client_loader_task.h"
#   include "blinkit/loader/tasks/file_loader_task.h"
#   include "blinkit/loader/tasks/res_loader_task.h"
#endif

using namespace blink;

namespace BlinKit {

void URLLoader::cancel(void)
{
    // Currently nothing to do.
}

void URLLoader::loadAsynchronously(const ResourceRequest &request, WebURLLoaderClient *client)
{
    const zed::url &url = request.url();

    LoaderTask *task = nullptr;
    do {
#ifdef BLINKIT_CRAWLER_ENABLED
        if (request.IsForCrawler())
        {
            if (url.scheme_is_in_http_family())
                task = new HTTPLoaderTask(request, this, m_taskRunner, client);
            else
                ASSERT(url.scheme_is_in_http_family());
            break;
        }
#endif

#ifdef BLINKIT_UI_ENABLED
        if (url.scheme_is_file())
        {
            task = new FileLoaderTask(request, this, m_taskRunner, client);
            break;
        }
        if (url.scheme_is("res"))
        {
            task = new ResLoaderTask(request, this, m_taskRunner, client);
            break;
        }
        if (AppImpl::Get().HasClientLoader())
        {
            task = new ClientLoaderTask(request, this, m_taskRunner, client);
            break;
        }

        BKLOG("WARNING: Unexpected URI `%s`!", url.spec().c_str());
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
