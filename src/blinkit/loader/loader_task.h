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

#include <string_view>
#include "bk_def.h"
#include "third_party/zed/include/zed/net/url.hpp"
#ifdef BLINKIT_UI_ENABLED
#   include "blinkit/blink/renderer/wtf/text/AtomicString.h"
#endif

namespace blink {
class ResourceRequest;
class ResourceResponse;
class WebTaskRunner;
class WebURLLoaderClient;
}

namespace BlinKit {

class LoaderTask
{
public:
    virtual ~LoaderTask(void);

    void Run(void);

    virtual const zed::url& URI(void) const = 0;
    virtual int PreProcess(void) { return BK_ERR_SUCCESS; }

    static void ReportError(blink::WebURLLoaderClient *client, blink::WebTaskRunner *taskRunner, int errorCode, const zed::url &URL);
protected:
    LoaderTask(const std::shared_ptr<blink::WebTaskRunner> &taskRunner, blink::WebURLLoaderClient *client);

    const std::shared_ptr<blink::WebTaskRunner> m_taskRunner;
    blink::WebURLLoaderClient *m_client;
private:
    virtual int PerformRequest(void) = 0;
    virtual int PopulateResponse(blink::ResourceResponse &resourceResponse, std::string_view &body) const = 0;
};

#ifdef BLINKIT_UI_ENABLED
class LoaderTaskForUI : public LoaderTask
{
public:
    const zed::url& URI(void) const final { return m_URI; }
protected:
    LoaderTaskForUI(const blink::ResourceRequest &request, const std::shared_ptr<blink::WebTaskRunner> &taskRunner, blink::WebURLLoaderClient *client);

    AtomicString MIMEType(void) const;

    const zed::url m_URI;
    std::string m_data;
    // BKTODO: ScopedRenderingScheduler m_scheduler;
};
#endif

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_LOADER_TASK_H
