// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: file_loader_task_win.cpp
// Description: FileLoaderTask Class
//      Author: Ziming Li
//     Created: 2019-03-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "./file_loader_task.h"

#include "third_party/zed/include/zed/net/http_codecs.hpp"
#include "third_party/zed/include/zed/string/conv.hpp"

using namespace blink;

namespace BlinKit {

int FileLoaderTask::LoadFileData(const zed::url &URI, std::string &dst)
{
    std::string host = URI.get_host();
    std::string path = zed::decode_uri_component(URI.get_path());

    std::wstring filePath;
    if (!host.empty())
    {
        filePath.assign(L"//");
        filePath.append(zed::multi_byte_to_wide_string(host, CP_UTF8));
        filePath.append(zed::multi_byte_to_wide_string(path, CP_UTF8));
    }
    else
    {
        filePath = zed::multi_byte_to_wide_string(std::string_view(path.data() + 1, path.length() - 1), CP_UTF8);
    }
    std::replace(filePath.begin(), filePath.end(), L'/', L'\\');

    HANDLE hFile = CreateFileW(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
    if (INVALID_HANDLE_VALUE == hFile)
    {
        DWORD lastError = GetLastError();
        ASSERT(INVALID_HANDLE_VALUE != hFile);

        switch (lastError)
        {
            case ERROR_FILE_NOT_FOUND:
                return BK_ERR_NOT_FOUND;
            case ERROR_ACCESS_DENIED:
                return BK_ERR_FORBIDDEN;
            default:
                return BK_ERR_UNKNOWN;
        }
    }

    DWORD size = GetFileSize(hFile, nullptr);
    dst.resize(size);
    ReadFile(hFile, dst.data(), dst.size(), &size, nullptr);

    ASSERT(dst.size() == size);

    CloseHandle(hFile);
    return BK_ERR_SUCCESS;
}

} // namespace BlinKit
