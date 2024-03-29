// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: file_loader_task.cpp
// Description: FileLoaderTask Class
//      Author: Ziming Li
//     Created: 2019-03-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "./file_loader_task.h"

#include "blinkit/blink/renderer/platform/network/ResourceRequest.h"
#include "blinkit/blink/renderer/platform/network/ResourceResponse.h"

using namespace blink;

namespace BlinKit {

FileLoaderTask::FileLoaderTask(const ResourceRequest &request, WebURLLoader *loader, const std::shared_ptr<WebTaskRunner> &taskRunner, WebURLLoaderClient *client)
    : LoaderTaskForUI(request, loader, taskRunner, client)
{
    // Nothing
}

int FileLoaderTask::PerformRequest(void)
{
    int r = LoadFileData(m_URI, m_data);
    switch (r)
    {
        case BK_ERR_NOT_FOUND:
            m_statusCode = 404;
            break;
        case BK_ERR_FORBIDDEN:
            m_statusCode = 403;
            break;
        default:
            return r;
    }
    return BK_ERR_SUCCESS;
}

int FileLoaderTask::PopulateResponse(ResourceResponse &resourceResponse, std::string_view &body) const
{
    resourceResponse.setHTTPStatusCode(m_statusCode);
    resourceResponse.setMimeType(MIMEType());

    body = m_data;
    return BK_ERR_SUCCESS;
}

} // namespace BlinKit
