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

#include "sdk/include/BlinKit.h"

using namespace blink;

namespace BlinKit {

ResLoaderTask::ResLoaderTask(const KURL &URI, WebURLLoaderClient *client) : LoaderTask(URI, client)
{
    // Nothing
}

void ResLoaderTask::run(void)
{
    int r = LoadResData(m_responseData->URI, m_responseData->Body);
    switch (r)
    {
        case BkError::NotFound:
            m_responseData->StatusCode = 404;
            [[fallthrough]];
        case BkError::Success:
            RespondToLoader();
            break;
        default:
            ReportErrorToLoader(r);
    }
}

} // namespace BlinKit
