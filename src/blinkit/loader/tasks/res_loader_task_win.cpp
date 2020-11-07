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

#include "bk_def.h"
#include "base/win/resource_util.h"

#define RT_HTMLA    MAKEINTRESOURCEA(23)

namespace BlinKit {

int ResLoaderTask::LoadResData(const GURL &URI, std::string &dst)
{
    void *data;
    size_t size;
    if (!base::GetResourceFromModule(nullptr, URI.path().c_str(), RT_HTMLA, &data, &size))
    {
        ASSERT(false); // Resource not found!
        return BK_ERR_NOT_FOUND;
    }

    dst.resize(size);
    memcpy(dst.data(), data, size);
    return BK_ERR_SUCCESS;
}

} // namespace BlinKit
