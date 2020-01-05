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

namespace base {
class SingleThreadTaskRunner;
}
namespace blink {
class ResourceRequest;
class WebURLLoaderClient;
}

namespace BlinKit {

class BkURL;

class LoaderTask
{
public:
    virtual ~LoaderTask(void);

    static void ReportError(blink::WebURLLoaderClient *client, base::SingleThreadTaskRunner *taskRunner, int errorCode, const BkURL &URL);

    virtual int Run(const blink::ResourceRequest &request) = 0;
protected:
    LoaderTask(const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner, blink::WebURLLoaderClient *client);

    std::shared_ptr<base::SingleThreadTaskRunner> m_taskRunner;
    blink::WebURLLoaderClient *m_client;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_LOADER_TASK_H
