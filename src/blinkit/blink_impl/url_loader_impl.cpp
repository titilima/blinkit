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

#include "public/platform/Platform.h"
#include "public/platform/WebTraceLocation.h"
#include "public/platform/WebURL.h"
#include "public/platform/WebURLLoaderClient.h"

#include "app/app_impl.h"
#include "loader_tasks/loader_task.h"

using namespace blink;

namespace BlinKit {

URLLoaderImpl::URLLoaderImpl(void) : m_taskRunner(Platform::current()->currentThread()->taskRunner())
{
    // Nothing
}

void URLLoaderImpl::cancel(void)
{
    // Currently nothing to do.
}

void URLLoaderImpl::loadAsynchronously(const WebURLRequest &request, WebURLLoaderClient *client)
{
    LoaderTask *task = LoaderTask::Create(request.url(), client);
    if (nullptr != task)
    {
        task->Setup(this, m_taskRunner);
        AppImpl::Get().IOThread().taskRunner()->postTask(BLINK_FROM_HERE, task);
    }
    else
    {
        WebURLError e;
        e.reason = BkError::UnknownError;
        client->didFail(this, e);
    }
}

void URLLoaderImpl::loadSynchronously(const WebURLRequest &, WebURLResponse&, WebURLError&, WebData& data)
{
    assert(false); // Not reached!
}

void URLLoaderImpl::setDefersLoading(bool)
{
    assert(false); // Not reached!
}

} // namespace BlinKit
