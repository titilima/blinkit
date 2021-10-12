// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_clipboard.cpp
// Description: WinClipboard Class
//      Author: Ziming Li
//     Created: 2018-07-06
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include "./win_clipboard.h"

#include "third_party/zed/include/zed/string/conv.hpp"

using namespace blink;

namespace BlinKit {

class ScopedClipboard
{
public:
    ScopedClipboard(HWND owner = nullptr)
    {
        OpenClipboard(owner);
    }
    ~ScopedClipboard(void)
    {
        CloseClipboard();
    }
};

String WinClipboard::readPlainText(Buffer buffer)
{
    ScopedClipboard sc;

    HANDLE hMem = GetClipboardData(CF_UNICODETEXT);
    if (nullptr == hMem)
        return String();

    String ret(reinterpret_cast<PCWSTR>(GlobalLock(hMem)));
    GlobalUnlock(hMem);
    return ret;
}

#if 0 // BKTODO:
uint64_t WinClipboard::sequenceNumber(Buffer)
{
    return GetClipboardSequenceNumber();
}
#endif

void WinClipboard::writePlainText(const String &plainText)
{
    ScopedClipboard sc;
    EmptyClipboard();

    std::wstring ws = zed::multi_byte_to_wide_string(plainText.stdUtf8(), CP_UTF8);

    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, sizeof(WCHAR) * (ws.length() + 1));
    if (nullptr == hMem)
    {
        ASSERT(nullptr != hMem);
        return;
    }

    PWSTR dst = reinterpret_cast<PWSTR>(GlobalLock(hMem));
    memcpy(dst, ws.data(), sizeof(WCHAR) * ws.length());
    dst[ws.length()] = L'\0';
    GlobalUnlock(hMem);

    SetClipboardData(CF_UNICODETEXT, hMem);
}

} // namespace BlinKit
