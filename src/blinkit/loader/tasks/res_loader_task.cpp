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

#include "res_loader_task.h"

#include "third_party/blink/renderer/platform/loader/fetch/resource_request.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_response.h"

using namespace blink;

namespace BlinKit {

ResLoaderTask::ResLoaderTask(const ResourceRequest &request, const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner, WebURLLoaderClient *client)
    : LoaderTaskForUI(request, taskRunner, client)
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
    resourceResponse.SetHTTPStatusCode(m_statusCode);
    resourceResponse.SetMimeType(MIMEType());

    body = m_data;
    return BK_ERR_SUCCESS;
}

} // namespace BlinKit
