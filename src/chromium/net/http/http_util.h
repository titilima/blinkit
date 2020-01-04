// -------------------------------------------------
// BlinKit - net Library
// -------------------------------------------------
//   File Name: http_util.h
// Description: HttpUtil Class
//      Author: Ziming Li
//     Created: 2019-12-16
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BLINKIT_NET_HTTP_UTIL_H
#define BLINKIT_NET_HTTP_UTIL_H

#pragma once

#include <string>
#include <string_view>

// This is a macro to support extending this string literal at compile time.
// Please excuse me polluting your global namespace!
#define HTTP_LWS " \t"

namespace net {

class HttpUtil
{
public:
    // Parses the value of a Content-Type header.  |mime_type|, |charset|, and
    // |had_charset| output parameters must be valid pointers.  |boundary| may be
    // nullptr.  |*mime_type| and |*charset| should be empty and |*had_charset|
    // false when called with the first Content-Type header value in a given
    // header list.
    //
    // ParseContentType() supports parsing multiple Content-Type headers in the
    // same header list.  For this operation, subsequent calls should pass in the
    // same |mime_type|, |charset|, and |had_charset| arguments without clearing
    // them.
    //
    // The resulting mime_type and charset values are normalized to lowercase.
    // The mime_type and charset output values are only modified if the
    // content_type_str contains a mime type and charset value, respectively.  If
    // |boundary| is not null, then |*boundary| will be assigned the (unquoted)
    // value of the boundary parameter, if any.
    static void ParseContentType(const std::string &contentTypeStr, std::string *mimeType, std::string *charset,
        bool *hadCharset, std::string *boundary);

    // Return true if the character is HTTP "linear white space" (SP | HT).
    // This definition corresponds with the HTTP_LWS macro, and does not match
    // newlines.
    static bool IsLWS(char c);

    // Trim HTTP_LWS chars from the beginning and end of the string.
    static std::string_view TrimLWS(const std::string_view &string);
};

} // namespace net

#endif // BLINKIT_NET_HTTP_UTIL_H
