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
#include "url/gurl.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "third_party/blink/renderer/platform/wtf/text/atomic_string.h"
#endif

namespace base {
class SingleThreadTaskRunner;
}
namespace blink {
class ResourceRequest;
class ResourceResponse;
class WebURLLoaderClient;
}

namespace BlinKit {

class LoaderTask
{
public:
    virtual ~LoaderTask(void);

    void Run(void);

    virtual const GURL& URI(void) const = 0;
    virtual int PreProcess(void) { return BK_ERR_SUCCESS; }

    static void ReportError(blink::WebURLLoaderClient *client, base::SingleThreadTaskRunner *taskRunner, int errorCode, const GURL &URL);

protected:
    LoaderTask(const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner, blink::WebURLLoaderClient *client);

    std::shared_ptr<base::SingleThreadTaskRunner> m_taskRunner;
    blink::WebURLLoaderClient *m_client;
private:
    virtual int PerformRequest(void) = 0;
    virtual int PopulateResponse(blink::ResourceResponse &resourceResponse, std::string_view &body) const = 0;
};

#ifndef BLINKIT_CRAWLER_ONLY
class LoaderTaskForUI : public LoaderTask
{
public:
    const GURL& URI(void) const final { return m_URI; }
protected:
    LoaderTaskForUI(const blink::ResourceRequest &request, const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner, blink::WebURLLoaderClient *client);

    AtomicString MIMEType(void) const;

    const GURL m_URI;
    std::string m_data;
};
#endif

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_LOADER_TASK_H
