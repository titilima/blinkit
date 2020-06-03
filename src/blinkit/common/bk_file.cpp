// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: bk_file.cpp
// Description: BkFile Class
//      Author: Ziming Li
//     Created: 2020-06-03
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "bk_file.h"

namespace BlinKit {

#ifdef OS_WIN

int BkFile::WriteContent(const BkPathChar *fileName, const void *data, size_t size)
{
    HANDLE hFile = CreateFileW(fileName, GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS,
        FILE_ATTRIBUTE_ARCHIVE, nullptr);
    if (INVALID_HANDLE_VALUE == hFile)
    {
        ASSERT(INVALID_HANDLE_VALUE != hFile);
        return BK_ERR_FORBIDDEN;
    }

    DWORD dwWritten = 0;
    WriteFile(hFile, data, size, &dwWritten, nullptr);
    ASSERT(size == dwWritten);
    CloseHandle(hFile);
    return BK_ERR_SUCCESS;
}

#endif // OS_WIN

} // namespace BlinKit
