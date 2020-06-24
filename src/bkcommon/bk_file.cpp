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

#if defined(OS_WIN)
#   include <Windows.h>
#elif defined(OS_POSIX)
#   include <cstdio>
#endif

namespace BlinKit {

#if defined(OS_WIN)

int BkFile::ReadContent(const BkPathChar *fileName, BkBuffer *dst)
{
    HANDLE hFile = CreateFileW(fileName, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
    if (INVALID_HANDLE_VALUE == hFile)
    {
        ASSERT(INVALID_HANDLE_VALUE != hFile);
        return BK_ERR_NOT_FOUND;
    }

    DWORD dwRead = 0;
    const DWORD size = GetFileSize(hFile, nullptr);
    void *buf = dst->Allocator(size, dst->UserData);
    ReadFile(hFile, buf, size, &dwRead, nullptr);
    ASSERT(size == dwRead);

    CloseHandle(hFile);
    return BK_ERR_SUCCESS;
}

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

#elif defined(OS_POSIX)

int BkFile::ReadContent(const BkPathChar *fileName, BkBuffer *dst)
{
    FILE *fp = fopen(fileName, "rb");
    if (nullptr == fp)
    {
        ASSERT(nullptr != fp);
        return BK_ERR_NOT_FOUND;
    }

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    void *buf = dst->Allocator(size, dst->UserData);
    fseek(fp, 0, SEEK_SET);
    fread(buf, 1, size, fp);

    fclose(fp);
    return true;
}

int BkFile::WriteContent(const BkPathChar *fileName, const void *data, size_t size)
{
    FILE *fp = fopen(fileName, "wb");
    if (nullptr == fp)
    {
        ASSERT(nullptr != fp);
        return BK_ERR_FORBIDDEN;
    }

    size_t written = fwrite(data, 1, size, fp);
    ASSERT(written == size);

    fclose(fp);
    return BK_ERR_SUCCESS;
}

#endif // OS_WIN

} // namespace BlinKit
