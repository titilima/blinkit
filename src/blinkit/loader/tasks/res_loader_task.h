// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: res_loader_task.h
// Description: ResLoaderTask Class
//      Author: Ziming Li
//     Created: 2018-08-23
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_RES_LOAD_TASK_H
#define BLINKIT_RES_LOAD_TASK_H

#pragma once

#include "blinkit/loader/loader_task.h"

namespace BlinKit {

class ResLoaderTask final : public LoaderTaskForUI
{
public:
    ResLoaderTask(const blink::ResourceRequest &request, const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner, blink::WebURLLoaderClient *client);
private:
    static int LoadResData(const GURL &URI, std::string &dst);

    int PerformRequest(void) override;
    int PopulateResponse(blink::ResourceResponse &resourceResponse, std::string_view &body) const override;

    int m_statusCode = 200;
};

} // namespace BlinKit

#endif // BLINKIT_RES_LOAD_TASK_H
