// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: logging_posix.cpp
// Description: BkLog Implementation
//      Author: Ziming Li
//     Created: 2019-09-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "logging.h"

#include <cstdio>
#include "base/strings/stringprintf.h"

namespace BlinKit {

void BkLog(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    std::string s = base::StringPrintV(format, args);
    va_end(args);

    s.push_back('\n');
    fputs(s.c_str(), stderr);
}

} // namespace BlinKit
