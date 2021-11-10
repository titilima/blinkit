#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: client_loader_task.h
// Description: ClientLoaderTask Class
//      Author: Ziming Li
//     Created: 2021-11-10
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_CLIENT_LOADER_TASK_H
#define BLINKIT_BLINKIT_CLIENT_LOADER_TASK_H

#include "blinkit/loader/loader_task.h"

namespace BlinKit {

class ClientLoaderTask final : public LoaderTaskForUI
{
public:
    ClientLoaderTask(const blink::ResourceRequest &request, blink::WebURLLoader *loader, const std::shared_ptr<blink::WebTaskRunner> &taskRunner, blink::WebURLLoaderClient *client);
private:
    int PerformRequest(void) override;
    int PopulateResponse(blink::ResourceResponse &resourceResponse, std::string_view &body) const override;

    int m_statusCode = 200;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CLIENT_LOADER_TASK_H
