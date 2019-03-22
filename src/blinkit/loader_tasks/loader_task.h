// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: loader_task.h
// Description: LoaderTask Class
//      Author: Ziming Li
//     Created: 2019-03-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_LOADER_TASK_H
#define BLINKIT_BLINKIT_LOADER_TASK_H

#pragma once

#include "platform/weborigin/KURL.h"
#include "public/platform/WebTaskRunner.h"
#include "response_data.h"

namespace blink {
class WebURLLoader;
class WebURLLoaderClient;
class WebURLRequest;
}

namespace BlinKit {

struct ResponseData;

class LoaderTask : public blink::WebTaskRunner::Task
{
public:
    static LoaderTask* Create(const blink::WebURLRequest &request, blink::WebURLLoaderClient *client);

    void Setup(blink::WebURLLoader *loader, blink::WebTaskRunner *taskRunner) {
        m_loader = loader; m_taskRunner = taskRunner;
    }
protected:
    LoaderTask(const blink::KURL &URI, blink::WebURLLoaderClient *client);
    void RespondToLoader(void);
    void ReportErrorToLoader(int errorCode);

    std::shared_ptr<ResponseData> m_responseData;
    blink::WebURLLoader *m_loader = nullptr;
    blink::WebTaskRunner *m_taskRunner = nullptr;
    blink::WebURLLoaderClient *m_client = nullptr;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_LOADER_TASK_H
