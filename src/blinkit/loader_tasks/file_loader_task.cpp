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

#include "sdk/include/BlinKit.h"

using namespace blink;

namespace BlinKit {

FileLoaderTask::FileLoaderTask(const KURL &URI, WebURLLoaderClient *client) : LoaderTask(URI, client)
{
    // Nothing
}

void FileLoaderTask::run(void)
{
    int r = LoadFileData(m_responseData->URI, m_responseData->Body);
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
