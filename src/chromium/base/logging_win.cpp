// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: logging_win.cpp
// Description: BkLog Implementation
//      Author: Ziming Li
//     Created: 2018-09-28
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include "./logging.h"

#include <Windows.h>
#include "chromium/base/strings/stringprintf.h"
#include "third_party/zed/include/zed/string/conv.hpp"

namespace BlinKit {

void BkLog(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    std::string s = base::StringPrintV(format, args);
    va_end(args);

    std::wstring log = zed::multi_byte_to_wide_string(s, CP_UTF8);
    log.append(L"\r\n");
    OutputDebugStringW(log.c_str());
}

} // namespace BlinKit
