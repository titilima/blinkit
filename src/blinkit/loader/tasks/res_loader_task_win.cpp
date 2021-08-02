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

#include "./res_loader_task.h"

#include "bk_def.h"
#include "third_party/zed/include/zed/win/hmodule.hpp"

#define RT_HTMLA    MAKEINTRESOURCEA(23)

namespace BlinKit {

int ResLoaderTask::LoadResData(const zed::url &URI, std::string &dst)
{
    zed::hmodule::resource_data data;
    if (!zed::hmodule::get_resource_data<PCSTR>(data, nullptr, RT_HTMLA, URI.get_path().c_str()))
    {
        ASSERT(false); // Resource not found!
        return BK_ERR_NOT_FOUND;
    }

    dst.resize(std::get<1>(data));
    memcpy(dst.data(), std::get<0>(data), std::get<1>(data));
    return BK_ERR_SUCCESS;
}

} // namespace BlinKit
