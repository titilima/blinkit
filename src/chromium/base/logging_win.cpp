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

#include "logging.h"

#include <Windows.h>
#include "base/strings/stringprintf.h"
#include "base/strings/sys_string_conversions.h"

namespace BlinKit {

void BkLog(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    std::string s = base::StringPrintV(format, args);
    va_end(args);

    std::wstring log = base::SysUTF8ToWide(s);
    log.append(L"\r\n");
    OutputDebugStringW(log.c_str());
}

} // namespace BlinKit
