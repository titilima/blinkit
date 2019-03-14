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
}

namespace BlinKit {

struct ResponseData;

class LoaderTask : public blink::WebTaskRunner::Task
{
public:
    static LoaderTask* Create(const blink::KURL &URI);

    void Setup(blink::WebURLLoader *loader, blink::WebTaskRunner *taskRunner, blink::WebURLLoaderClient *client) {
        m_loader = loader; m_taskRunner = taskRunner; m_client = client;
    }
protected:
    LoaderTask(const blink::KURL &URI);
    void RespondToLoader(void);
    void ReportErrorToLoader(int errorCode);

    std::shared_ptr<ResponseData> m_responseData;
private:
    blink::WebURLLoader *m_loader = nullptr;
    blink::WebTaskRunner *m_taskRunner = nullptr;
    blink::WebURLLoaderClient *m_client = nullptr;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_LOADER_TASK_H
