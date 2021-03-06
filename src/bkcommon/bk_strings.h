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

#ifndef BLINKIT_BKCOMMON_BK_STRINGS_H
#define BLINKIT_BKCOMMON_BK_STRINGS_H

#pragma once

namespace BlinKit {

namespace Strings {

extern const char DefaultUserAgent[];

namespace HttpHeader {
extern const char AcceptEncoding[];
extern const char ContentEncoding[];
extern const char ContentLength[];
extern const char ContentType[];
extern const char Cookie[];
extern const char Referer[];
extern const char UserAgent[];
} // namespace HttpHeader

} // namespace Strings

} // namespace BlinKit

#endif // BLINKIT_BKCOMMON_BK_STRINGS_H
