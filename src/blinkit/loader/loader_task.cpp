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
#include "third_party/blink/renderer/platform/loader/fetch/resource_request.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_response.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "third_party/blink/renderer/platform/network/mime/mime_type_registry.h"
#endif

using namespace blink;

namespace BlinKit {

LoaderTask::LoaderTask(const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner, WebURLLoaderClient *client)
    : m_taskRunner(taskRunner), m_client(client)
{
}

LoaderTask::~LoaderTask(void) = default;

void LoaderTask::Run(void)
{
    ASSERT(!IsMainThread());

    int r = PerformRequest();
    if (BK_ERR_SUCCESS == r)
    {
        const auto callback = [this]
        {
            ASSERT(IsMainThread());

            const GURL &u = URI();

            ResourceResponse resourceResponse(u);
            std::string_view body;
            int r = PopulateResponse(resourceResponse, body);
            if (BK_ERR_CANCELLED == r)
                return; // The task will process the response itself.

            if (BK_ERR_SUCCESS == r)
            {
                m_client->DidReceiveResponse(resourceResponse);
                m_client->DidReceiveData(body.data(), body.length());
                m_client->DidFinishLoading();
            }
            else
            {
                m_client->DidFail(ResourceError(r, u));
            }
            delete this;
        };
        m_taskRunner->PostTask(FROM_HERE, callback);
    }
    else
    {
        const auto errorCallback = [error = ResourceError(r, URI()), client = m_client]
        {
            ASSERT(IsMainThread());
            client->DidFail(error);
        };
        m_taskRunner->PostTask(FROM_HERE, errorCallback);
        delete this;
    }
}

static void ErrorWorker(WebURLLoaderClient *client, const ResourceError error)
{
    ASSERT(IsMainThread());
    client->DidFail(error);
}

void LoaderTask::ReportError(WebURLLoaderClient *client, base::SingleThreadTaskRunner *taskRunner, int errorCode, const GURL &URL)
{
    ResourceError error(errorCode, URL);
    std::function<void()> callback = std::bind(&ErrorWorker, client, error);
    taskRunner->PostTask(FROM_HERE, callback);
}

#ifndef BLINKIT_CRAWLER_ONLY
LoaderTaskForUI::LoaderTaskForUI(const ResourceRequest &request, const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner, WebURLLoaderClient *client)
    : LoaderTask(taskRunner, client)
    , m_URI(request.Url())
{
}

AtomicString LoaderTaskForUI::MIMEType(void) const
{
    const std::string fileName = m_URI.ExtractFileName();

    size_t p = fileName.rfind('.');
    if (std::string::npos == p)
        return AtomicString::FromUTF8("application/x-unknown-content-type");

    String ret = MIMETypeRegistry::GetWellKnownMIMETypeForExtension(String::FromUTF8(fileName.data() + p + 1));
    return AtomicString(ret);
}
#endif

} // namespace BlinKit
