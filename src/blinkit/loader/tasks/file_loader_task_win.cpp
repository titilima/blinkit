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

#include "file_loader_task.h"

#include "base/strings/sys_string_conversions.h"

using namespace blink;

namespace BlinKit {

int FileLoaderTask::LoadFileData(const GURL &URI, std::string &dst)
{
    std::string host = URI.host();
    std::string path = URI.path();

    std::wstring filePath;
    if (!host.empty())
    {
        filePath.assign(L"//");
        filePath.append(base::SysUTF8ToWide(host));
        filePath.append(base::SysUTF8ToWide(path));
    }
    else
    {
        filePath = base::SysUTF8ToWide(std::string_view(path.data() + 1, path.length() - 1));
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
