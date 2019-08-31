// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: response_error_task.h
// Description: ResponseErrorTask Class
//      Author: Ziming Li
//     Created: 2019-08-30
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_RESPONSE_ERROR_TASK_H
#define BLINKIT_BLINKIT_RESPONSE_ERROR_TASK_H

#pragma once

#include "public/platform/WebTaskRunner.h"

namespace blink {
class WebURLLoader;
class WebURLLoaderClient;
}

namespace BlinKit {

class ResponseErrorTask final : public blink::WebTaskRunner::Task
{
public:
    ResponseErrorTask(int errorCode, blink::WebURLLoader *loader, blink::WebURLLoaderClient *client);
private:
    // blink::WebTaskRunner::Task
    void run(void) override;

    const int m_errorCode;
    blink::WebURLLoader *m_loader;
    blink::WebURLLoaderClient *m_client;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_RESPONSE_ERROR_TASK_H
