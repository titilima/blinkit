// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: res_loader_task.cpp
// Description: ResLoaderTask Class
//      Author: Ziming Li
//     Created: 2018-08-23
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include "./res_loader_task.h"

#include "blinkit/blink/renderer/platform/network/ResourceRequest.h"
#include "blinkit/blink/renderer/platform/network/ResourceResponse.h"

using namespace blink;

namespace BlinKit {

ResLoaderTask::ResLoaderTask(const ResourceRequest &request, WebURLLoader *loader, const std::shared_ptr<WebTaskRunner> &taskRunner, WebURLLoaderClient *client)
    : LoaderTaskForUI(request, loader, taskRunner, client)
{
    // Nothing
}

int ResLoaderTask::PerformRequest(void)
{
    int r = LoadResData(m_URI, m_data);
    if (BK_ERR_NOT_FOUND == r)
    {
        m_statusCode = 404;
        r = BK_ERR_SUCCESS;
    }
    return r;
}

int ResLoaderTask::PopulateResponse(ResourceResponse &resourceResponse, std::string_view &body) const
{
    resourceResponse.setHTTPStatusCode(m_statusCode);
    resourceResponse.setMimeType(MIMEType());

    body = m_data;
    return BK_ERR_SUCCESS;
}

} // namespace BlinKit
