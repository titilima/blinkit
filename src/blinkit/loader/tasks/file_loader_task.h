// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: file_loader_task.h
// Description: FileLoaderTask Class
//      Author: Ziming Li
//     Created: 2019-03-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_FILE_LOADER_TASK_H
#define BLINKIT_BLINKIT_FILE_LOADER_TASK_H

#pragma once

#include "blinkit/loader/loader_task.h"

namespace BlinKit {

class FileLoaderTask final : public LoaderTaskForUI
{
public:
    FileLoaderTask(const blink::ResourceRequest &request, const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner, blink::WebURLLoaderClient *client);
private:
    static int LoadFileData(const GURL &URI, std::string &dst);

    int PerformRequest(void) override;
    int PopulateResponse(blink::ResourceResponse &resourceResponse, std::string_view &body) const override;

    int m_statusCode = 200;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_FILE_LOADER_TASK_H
