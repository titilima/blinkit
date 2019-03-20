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

#include "win_clipboard.h"

#include "base/strings/sys_string_conversions.h"

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

WebString WinClipboard::readPlainText(Buffer buffer)
{
    ScopedClipboard sc;

    HANDLE hMem = GetClipboardData(CF_UNICODETEXT);
    if (nullptr == hMem)
        return WebString();

    WebString ret;
    PCWSTR s = reinterpret_cast<PCWSTR>(GlobalLock(hMem));
    ret.assign(s, wcslen(s));
    GlobalUnlock(hMem);
    return ret;
}

uint64_t WinClipboard::sequenceNumber(Buffer)
{
    return GetClipboardSequenceNumber();
}

void WinClipboard::writePlainText(const WebString &plainText)
{
    ScopedClipboard sc;
    EmptyClipboard();

    std::wstring ws = base::SysUTF8ToWide(plainText.utf8());

    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, sizeof(WCHAR) * (ws.length() + 1));
    if (nullptr == hMem)
    {
        assert(nullptr != hMem);
        return;
    }

    PWSTR dst = reinterpret_cast<PWSTR>(GlobalLock(hMem));
    memcpy(dst, ws.data(), sizeof(WCHAR) * ws.length());
    dst[ws.length()] = L'\0';
    GlobalUnlock(hMem);

    SetClipboardData(CF_UNICODETEXT, hMem);
}

} // namespace BlinKit
