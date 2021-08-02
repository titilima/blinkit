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

#include "blinkit/blink/public/platform/WebTaskRunner.h"
#include "blinkit/blink/public/platform/WebTraceLocation.h"
#include "blinkit/blink/public/platform/WebURLLoaderClient.h"
#include "blinkit/blink/renderer/platform/network/ResourceError.h"
#include "blinkit/blink/renderer/wtf/MainThread.h"
#if 0 // BKTODO:
#include "third_party/blink/renderer/platform/loader/fetch/resource_request.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_response.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "third_party/blink/renderer/platform/network/mime/mime_type_registry.h"
#endif
#endif
using namespace blink;

namespace BlinKit {

LoaderTask::LoaderTask(const std::shared_ptr<WebTaskRunner> &taskRunner, WebURLLoaderClient *client)
    : m_taskRunner(taskRunner), m_client(client)
{
}

LoaderTask::~LoaderTask(void) = default;

void LoaderTask::Run(void)
{
    ASSERT(!isMainThread());

    int r = PerformRequest();
    if (BK_ERR_SUCCESS == r)
    {
        auto callback = [this]
        {
            ASSERT(isMainThread());

            ASSERT(false); // BKTODO:
#if 0
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
#endif
            delete this;
        };
        m_taskRunner->postTask(BLINK_FROM_HERE, callback);
    }
    else
    {
        ASSERT(false); // BKTODO:
#if 0
        auto errorCallback = [error = ResourceError(r, URI()), client = m_client]
        {
            ASSERT(IsMainThread());
            client->DidFail(error);
        };
        m_taskRunner->postTask(BLINK_FROM_HERE, errorCallback);
#endif
        delete this;
    }
}

static void ErrorWorker(WebURLLoaderClient *client, const ResourceError &error)
{
    ASSERT(isMainThread());
    ASSERT(false); // BKTODO: client->didFail(error);
}

void LoaderTask::ReportError(WebURLLoaderClient *client, WebTaskRunner *taskRunner, int errorCode, const zed::url &URL)
{
    ASSERT(false); // BKTODO:
#if 0
    ResourceError error(errorCode, URL);
    taskRunner->PostTask(BLINK_FROM_HERE,
        std::bind(&ErrorWorker, client, error)
    );
#endif
}

#if 0 // BKTODO: ndef BLINKIT_CRAWLER_ONLY
LoaderTaskForUI::LoaderTaskForUI(const ResourceRequest &request, const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner, WebURLLoaderClient *client)
    : LoaderTask(taskRunner, client)
    , m_URI(request.Url())
    , m_scheduler(request.View())
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
