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
#include "blinkit/blink/renderer/platform/network/ResourceRequest.h"
#include "blinkit/blink/renderer/platform/network/ResourceResponse.h"
#include "blinkit/blink/renderer/wtf/MainThread.h"
#ifdef BLINKIT_UI_ENABLED
#   include "blinkit/blink/renderer/platform/MIMETypeRegistry.h"
#endif

using namespace blink;

namespace BlinKit {

LoaderTask::LoaderTask(WebURLLoader *loader, const std::shared_ptr<WebTaskRunner> &taskRunner, WebURLLoaderClient *client)
    : m_clientGuard(client), m_loader(loader), m_taskRunner(taskRunner), m_client(client)
{
}

LoaderTask::~LoaderTask(void) = default;

void LoaderTask::run(void)
{
    ASSERT(!isMainThread());

    int r = PerformRequest();
    if (BK_ERR_SUCCESS == r)
    {
        auto callback = [this]
        {
            ASSERT(isMainThread());

            ResourceResponse response;
            response.setURL(URI());
            std::string_view body;
            int r = PopulateResponse(response, body);
            if (BK_ERR_CANCELLED == r)
                return; // The task will process the response itself.

            if (BK_ERR_SUCCESS == r)
            {
                m_client->didReceiveResponse(m_loader, response);
                m_client->didReceiveData(m_loader, body.data(), body.length(), 0);
                m_client->didFinishLoading(m_loader, body.length());
            }
            else
            {
                ASSERT(false); // BKTODO:  m_client->DidFail(ResourceError(r, u));
            }
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

#ifdef BLINKIT_UI_ENABLED
LoaderTaskForUI::LoaderTaskForUI(const ResourceRequest &request, blink::WebURLLoader *loader, const std::shared_ptr<WebTaskRunner> &taskRunner, WebURLLoaderClient *client)
    : LoaderTask(loader, taskRunner, client)
    , m_URI(request.url())
{
}

AtomicString LoaderTaskForUI::MIMEType(void) const
{
    const std::string fileName = m_URI.ExtractFileName();

    size_t p = fileName.rfind('.');
    if (std::string::npos == p)
        return AtomicString::fromUTF8("application/x-unknown-content-type");

    String ret = MIMETypeRegistry::getMIMETypeForExtension(String::fromUTF8(fileName.data() + p + 1));
    return AtomicString(ret);
}
#endif

} // namespace BlinKit
