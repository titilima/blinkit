// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: client_loader_task.cpp
// Description: ClientLoaderTask Class
//      Author: Ziming Li
//     Created: 2021-11-10
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./client_loader_task.h"

#include "blinkit/app/app_impl.h"
#include "blinkit/blink/renderer/platform/network/ResourceRequest.h"
#include "blinkit/blink/renderer/platform/network/ResourceResponse.h"

using namespace blink;

namespace BlinKit {

ClientLoaderTask::ClientLoaderTask(const ResourceRequest &request, WebURLLoader *loader, const std::shared_ptr<WebTaskRunner> &taskRunner, WebURLLoaderClient *client)
    : LoaderTaskForUI(request, loader, taskRunner, client)
{
    // Nothing
}

int ClientLoaderTask::PerformRequest(void)
{
    if (!AppImpl::Get().LoadResourceFromClient(m_URI.spec().c_str(), m_data))
        m_statusCode = 404;
    return BK_ERR_SUCCESS;
}

int ClientLoaderTask::PopulateResponse(ResourceResponse &resourceResponse, std::string_view &body) const
{
    resourceResponse.setHTTPStatusCode(m_statusCode);
    resourceResponse.setMimeType(MIMEType());

    body = m_data;
    return BK_ERR_SUCCESS;
}

} // namespace BlinKit
