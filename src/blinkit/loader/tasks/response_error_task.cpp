// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: response_error_task.cpp
// Description: ResponseErrorTask Class
//      Author: Ziming Li
//     Created: 2019-08-30
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "response_error_task.h"

#include "public/platform/WebURLError.h"
#include "public/platform/WebURLLoaderClient.h"

#include "sdk/include/BlinKit.h"

using namespace blink;

namespace BlinKit {

ResponseErrorTask::ResponseErrorTask(int errorCode, WebURLLoader *loader, WebURLLoaderClient *client)
    : m_errorCode(errorCode), m_loader(loader), m_client(client)
{
    // Nothing
}

void ResponseErrorTask::run(void)
{
    WebURLError error;
    error.reason = m_errorCode;
    if (BkError::Cancelled == m_errorCode)
        error.isCancellation = true;
    m_client->didFail(m_loader, error);
}

} // namespace BlinKit
