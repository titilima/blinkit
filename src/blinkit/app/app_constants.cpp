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
#if OS_WIN
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36"
#elif OS_MACOSX
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36"
#endif
    ;

const unsigned long DefaultTimeoutInMs = 30000;

} // namespace AppConstants
} // namespace BlinKit
