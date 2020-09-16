// -------------------------------------------------
// BlinKit - BkCommon Library
// -------------------------------------------------
//   File Name: bk_strings.h
// Description: Common Strings
//      Author: Ziming Li
//     Created: 2020-06-21
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "bk_strings.h"

namespace BlinKit {
namespace Strings {

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

namespace HttpHeader {

const char AcceptEncoding[] = "Accept-Encoding";
const char ContentEncoding[] = "Content-Encoding";
const char ContentLength[] = "Content-Length";
const char ContentType[] = "Content-Type";
const char Cookie[] = "Cookie";
const char Referer[] = "Referer";
const char UserAgent[] = "User-Agent";

} // namespace HttpHeader

} // namespace Strings

} // namespace BlinKit
