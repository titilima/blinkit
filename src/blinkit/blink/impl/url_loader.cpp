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
#include "blinkit/loader/tasks/http_loader_task.h"
#if 0 // BKTODO: def BLINKIT_UI_ENABLED
#   include "blinkit/loader/tasks/file_loader_task.h"
#   include "blinkit/loader/tasks/res_loader_task.h"
#endif

using namespace blink;

namespace BlinKit {

void URLLoader::loadAsynchronously(const ResourceRequest &request, WebURLLoaderClient *client)
{
    const zed::url &url = request.url();

    LoaderTask *task = nullptr;
    do {
#ifdef BLINKIT_CRAWLER_ENABLED
        if (url.scheme_is_in_http_family())
        {
            if (request.ForCrawler())
                ASSERT(false); // BKTODO: task = new HTTPLoaderTask(request, m_taskRunner, client);
            else
                NOTREACHED();
            break;
        }

        if (request.ForCrawler())
            break;
#endif

#ifdef BLINKIT_CRAWLER_ENABLED
        if (url.scheme_is_file())
        {
            ASSERT(false); // BKTODO: task = new FileLoaderTask(request, m_taskRunner, client);
            break;
        }
        if (url.scheme_is("res"))
        {
            ASSERT(false); // BKTODO: task = new ResLoaderTask(request, m_taskRunner, client);
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
