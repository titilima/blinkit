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

#include "loader_task.h"

namespace BlinKit {

class ResLoaderTask final : public LoaderTask
{
public:
    ResLoaderTask(const blink::KURL &URI, blink::WebURLLoaderClient *client);
private:
    static int LoadResData(const blink::KURL &URI, std::vector<unsigned char> &dst);

    // blink::WebTaskRunner::Task
    void run(void) override;
};

} // namespace BlinKit

#endif // BLINKIT_RES_LOAD_TASK_H
