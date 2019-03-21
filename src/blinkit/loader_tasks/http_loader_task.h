// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: http_loader_task.h
// Description: HTTPLoaderTask Class
//      Author: Ziming Li
//     Created: 2019-03-21
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_HTTP_LOADER_TASK_H
#define BLINKIT_BLINKIT_HTTP_LOADER_TASK_H

#pragma once

#include "sdk/include/BlinKit.h"
#include "loader_task.h"

namespace BlinKit {

class CrawlerImpl;

class HTTPLoaderTask final : public LoaderTask, public BkRequestClient
{
public:
    HTTPLoaderTask(CrawlerImpl &crawler, const blink::KURL &URI, blink::WebURLLoaderClient *client);
private:
    // blink::WebTaskRunner::Task
    void run(void) override;
    // BkRequestClient
    void BKAPI RequestComplete(const BkResponse &response) override;
    void BKAPI RequestFailed(int errorCode) override;

    CrawlerImpl &m_crawler;
    BkRetainedResponse *m_response = nullptr;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_HTTP_LOADER_TASK_H
