// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: res_loader_task_win.cpp
// Description: ResLoaderTask Class
//      Author: Ziming Li
//     Created: 2018-08-23
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include "res_loader_task.h"

#include "sdk/include/BlinKit.h"
#include "base/win/resource_util.h"

#define RT_HTMLA    MAKEINTRESOURCEA(23)

using namespace blink;

namespace BlinKit {

int ResLoaderTask::LoadResData(const KURL &URI, std::vector<unsigned char> &dst)
{
    std::string path = URI.path().to_string();

    void *data;
    size_t size;
    if (!base::GetResourceFromModule(nullptr, path.c_str(), RT_HTMLA, &data, &size))
    {
        assert(false); // Resource not found!
        return BkError::NotFound;
    }

    dst.resize(size);
    memcpy(dst.data(), data, size);
    return BkError::Success;
}

} // namespace BlinKit
