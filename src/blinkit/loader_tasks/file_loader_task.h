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

#include "loader_task.h"

namespace BlinKit {

class FileLoaderTask final : public LoaderTask
{
public:
    FileLoaderTask(const blink::KURL &URI);
private:
    static int LoadFileData(const blink::KURL &URI, std::vector<unsigned char> &dst);

    // blink::WebTaskRunner::Task
    void run(void) override;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_FILE_LOADER_TASK_H
