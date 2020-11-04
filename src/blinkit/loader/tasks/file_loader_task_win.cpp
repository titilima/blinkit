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

#include "sdk/include/BlinKit.h"

using namespace blink;

namespace BlinKit {

int FileLoaderTask::LoadFileData(const KURL &URI, std::vector<unsigned char> &dst)
{
    String host = URI.host(), path = URI.path();
    path.ensure16Bit();

    std::wstring filePath;
    if (!host.isEmpty())
    {
        host.ensure16Bit();

        filePath.assign(L"//");
        filePath.append(host.characters16(), host.length());
        filePath.append(path.characters16(), path.length());
    }
    else
    {
        filePath.assign(path.characters16() + 1, path.length() - 1);
    }
    std::replace(filePath.begin(), filePath.end(), L'/', L'\\');

    HANDLE hFile = CreateFileW(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
    if (INVALID_HANDLE_VALUE == hFile)
    {
        DWORD lastError = GetLastError();
        assert(INVALID_HANDLE_VALUE != hFile);

        switch (lastError)
        {
            case ERROR_FILE_NOT_FOUND:
                return BkError::NotFound;
            default:
                return BkError::UnknownError;
        }
    }

    DWORD size = GetFileSize(hFile, nullptr);
    dst.resize(size);
    ReadFile(hFile, dst.data(), dst.size(), &size, nullptr);

    assert(dst.size() == size);

    CloseHandle(hFile);
    return BkError::Success;
}

} // namespace BlinKit
