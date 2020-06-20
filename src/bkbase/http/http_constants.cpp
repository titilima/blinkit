// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: http_constants.cpp
// Description: HTTP Constants
//      Author: Ziming Li
//     Created: 2020-06-20
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "http_constants.h"

namespace BlinKit {
namespace HttpConstants {

const char DefaultUserAgent[] =
    "Mozilla/5.0 ("
#if OS_WIN
    "Windows NT 10.0; Win64; x64"
#elif OS_MACOSX
    "Macintosh; Intel Mac OS X 10_14_3"
#elif OS_LINUX
    "X11; Linux x86_64"
#else
#   error To be implemented!
#endif
    ") AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36";

const unsigned long DefaultTimeoutInMs = 30000;

} // namespace HttpConstants
} // namespace BlinKit
