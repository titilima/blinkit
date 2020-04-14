// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: logging_apple.mm
// Description: BkLog Implementation
//      Author: Ziming Li
//     Created: 2019-08-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "logging.h"

#include "base/strings/stringprintf.h"

namespace BlinKit {

void BkLog(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    std::string s = base::StringPrintV(format, args);
    va_end(args);
    NSLog(@"%s", s.c_str());
}

} // namespace BlinKit
