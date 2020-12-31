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

#include "file_loader_task.h"

#include "third_party/blink/renderer/platform/loader/fetch/resource_request.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_response.h"

using namespace blink;

namespace BlinKit {

FileLoaderTask::FileLoaderTask(const ResourceRequest &request, const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner, WebURLLoaderClient *client)
    : LoaderTaskForUI(request, taskRunner, client)
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
    resourceResponse.SetHTTPStatusCode(m_statusCode);
    resourceResponse.SetMimeType(MIMEType());

    body = m_data;
    return BK_ERR_SUCCESS;
}

} // namespace BlinKit
