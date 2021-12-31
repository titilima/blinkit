#pragma once
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

#ifndef BLINKIT_LOADER_TASK_H
#define BLINKIT_LOADER_TASK_H

#include <string_view>
#include "bk_def.h"
#include "blinkit/blink/renderer/platform/weborigin/KURL.h"
#include "blinkit/gc/gc_def.h"
#include "third_party/zed/include/zed/threading/task_queue.hpp"
#ifdef BLINKIT_UI_ENABLED
#   include "blinkit/blink/renderer/wtf/text/AtomicString.h"
#endif

namespace blink {
class ResourceRequest;
class ResourceResponse;
class WebTaskRunner;
class WebURLLoader;
class WebURLLoaderClient;
}

namespace BlinKit {

class LoaderTask : public zed::task_thread::task
{
public:
    ~LoaderTask(void) override;

    virtual const KURL& URI(void) const = 0;
    virtual int PreProcess(void) { return BK_ERR_SUCCESS; }

    static void ReportError(WebURLLoaderClient *client, WebTaskRunner *taskRunner, int errorCode, const zed::url &URL);
protected:
    LoaderTask(WebURLLoader *loader, const std::shared_ptr<WebTaskRunner> &taskRunner, WebURLLoaderClient *client);

    const std::shared_ptr<WebTaskRunner> m_taskRunner;
    WebURLLoaderClient *m_client;
private:
    virtual int PerformRequest(void) = 0;
    virtual int PopulateResponse(ResourceResponse &resourceResponse, std::string_view &body) const = 0;

    void run(void) final;

    GCGuard m_clientGuard;
    WebURLLoader *m_loader;
};

#ifdef BLINKIT_UI_ENABLED
class LoaderTaskForUI : public LoaderTask
{
public:
    const KURL& URI(void) const final { return m_URI; }
protected:
    LoaderTaskForUI(const ResourceRequest &request, WebURLLoader *loader, const std::shared_ptr<WebTaskRunner> &taskRunner, WebURLLoaderClient *client);

    AtomicString MIMEType(void) const;

    const KURL m_URI;
    std::string m_data;
};
#endif

} // namespace BlinKit

#endif // BLINKIT_LOADER_TASK_H
