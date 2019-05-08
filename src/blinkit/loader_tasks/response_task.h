// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: response_task.h
// Description: ResponseTask Class
//      Author: Ziming Li
//     Created: 2019-03-14
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_RESPONSE_TASK_H
#define BLINKIT_BLINKIT_RESPONSE_TASK_H

#pragma once

#include "public/platform/WebTaskRunner.h"
#include "response_data.h"

namespace blink {
class WebURLLoader;
class WebURLLoaderClient;
}

namespace BlinKit {

class ResponseTask : public blink::WebTaskRunner::Task
{
public:
    ResponseTask(blink::WebURLLoader *loader, blink::WebURLLoaderClient *client, ResponseData &responseData);
protected:
    // blink::WebTaskRunner::Task
    void run(void) override;

    std::shared_ptr<ResponseData> m_responseData;
private:
    blink::WebURLLoader *m_loader;
    blink::WebURLLoaderClient *m_client;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_RESPONSE_TASK_H
