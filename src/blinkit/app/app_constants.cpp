// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: app_constants.cpp
// Description: Application Constants
//      Author: Ziming Li
//     Created: 2019-03-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "app_constants.h"

namespace BlinKit {
namespace AppConstants {

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

} // namespace AppConstants
} // namespace BlinKit
