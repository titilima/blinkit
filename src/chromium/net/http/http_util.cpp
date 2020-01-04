// -------------------------------------------------
// BlinKit - net Library
// -------------------------------------------------
//   File Name: http_util.cpp
// Description: HttpUtil Class
//      Author: Ziming Li
//     Created: 2019-12-16
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// The rules for parsing content-types were borrowed from Firefox:
// http://lxr.mozilla.org/mozilla/source/netwerk/base/src/nsURLHelper.cpp#834

#include "http_util.h"

#include <algorithm>
#include "base/strings/string_util.h"

namespace net {
namespace {

template <typename ConstIterator>
void TrimLWSImplementation(ConstIterator *begin, ConstIterator *end)
{
    // leading whitespace
    while (*begin < *end && HttpUtil::IsLWS((*begin)[0]))
        ++(*begin);

    // trailing whitespace
    while (*begin < *end && HttpUtil::IsLWS((*end)[-1]))
        --(*end);
}

} // namespace

bool HttpUtil::IsLWS(char c)
{
    const std::string_view kWhiteSpaceCharacters(HTTP_LWS);
    return kWhiteSpaceCharacters.find(c) != std::string_view::npos;
}

void HttpUtil::ParseContentType(
    const std::string &contentTypeStr,
    std::string *mimeType, std::string *charset,
    bool *hadCharset,
    std::string *boundary)
{
    // Trim leading and trailing whitespace from type.  We include '(' in
    // the trailing trim set to catch media-type comments, which are not at all
    // standard, but may occur in rare cases.
    size_t typeVal = contentTypeStr.find_first_not_of(HTTP_LWS);
    typeVal = std::min(typeVal, contentTypeStr.length());
    size_t typeEnd = contentTypeStr.find_first_of(HTTP_LWS ";(", typeVal);
    if (std::string::npos == typeEnd)
        typeEnd = contentTypeStr.length();

    std::string charsetValue;
    bool typeHasCharset = false;
    bool typeHasBoundary = false;

    // Iterate over parameters. Can't split the string around semicolons
    // preemptively because quoted strings may include semicolons. Mostly matches
    // logic in https://mimesniff.spec.whatwg.org/. Main differences: Does not
    // validate characters are HTTP token code points / HTTP quoted-string token
    // code points, and ignores spaces after "=" in parameters.
    std::string::size_type offset = contentTypeStr.find_first_of(';', typeEnd);
    while (offset < contentTypeStr.size())
    {
        assert(';' == contentTypeStr[offset]);
        // Trim off the semicolon.
        ++offset;

        // Trim off any following spaces.
        offset = contentTypeStr.find_first_not_of(HTTP_LWS, offset);
        std::string::size_type paramNameStart = offset;

        // Extend parameter name until run into a semicolon or equals sign.  Per
        // spec, trailing spaces are not removed.
        offset = contentTypeStr.find_first_of(";=", offset);

        // Nothing more to do if at end of string, or if there's no parameter
        // value, since names without values aren't allowed.
        if (offset == std::string::npos || contentTypeStr[offset] == ';')
            continue;

        std::string_view paramName(contentTypeStr.data() + paramNameStart, offset - paramNameStart);

        // Now parse the value.
        assert('=' == contentTypeStr[offset]);
        // Trim off the '='.
        ++offset;

        // Remove leading spaces. This violates the spec, though it matches
        // pre-existing behavior.
        //
        // TODO(mmenke): Consider doing this (only?) after parsing quotes, which
        // seems to align more with the spec - not the content-type spec, but the
        // GET spec's way of getting an encoding, and the spec for handling
        // boundary values as well.
        // See https://encoding.spec.whatwg.org/#names-and-labels.
        offset = contentTypeStr.find_first_not_of(HTTP_LWS, offset);

        std::string paramValue;
        if (offset == std::string::npos || contentTypeStr[offset] == ';')
        {
            // Nothing to do here - an unquoted string of only whitespace should be
            // skipped.
            continue;
        }
        else if (contentTypeStr[offset] != '"')
        {
            // If the first character is not a quotation mark, copy data directly.
            std::string::size_type valueStart = offset;
            offset = contentTypeStr.find_first_of(';', offset);
            std::string::size_type valueEnd = offset;

            // Remove terminal whitespace. If ran off the end of the string, have to
            // update |value_end| first.
            if (valueEnd == std::string::npos)
                valueEnd = contentTypeStr.size();
            while (valueEnd > valueStart && IsLWS(contentTypeStr[valueEnd - 1]))
                --valueEnd;

            paramValue = contentTypeStr.substr(valueStart, valueEnd - valueStart);
        }
        else
        {
            // Otherwise, append data, with special handling for backslashes, until
            // a close quote.

            // Skip open quote.
            assert('"' == contentTypeStr[offset]);
            ++offset;

            while (offset < contentTypeStr.size() && contentTypeStr[offset] != '"')
            {
                // Skip over backslash and append the next character, when not at
                // the end of the string. Otherwise, copy the next character (Which may
                // be a backslash).
                if (contentTypeStr[offset] == '\\' && offset + 1 < contentTypeStr.size())
                    ++offset;
                paramValue += contentTypeStr[offset];
                ++offset;
            }

            std::string_view trimmed = TrimLWS(paramValue);
            paramValue.assign(trimmed.data(), trimmed.length());

            offset = contentTypeStr.find_first_of(';', offset);
        }

        // TODO(mmenke): Check that name has only valid characters.
        if (!typeHasCharset && base::LowerCaseEqualsASCII(paramName, "charset"))
        {
            typeHasCharset = true;
            charsetValue = paramValue;
            continue;
        }

        if (nullptr != boundary && !typeHasBoundary && base::LowerCaseEqualsASCII(paramName, "boundary"))
        {
            typeHasBoundary = true;
            boundary->assign(std::move(paramValue));
            continue;
        }
    }

    // If the server sent "*/*", it is meaningless, so do not store it.
    // Also, reject a mime-type if it does not include a slash.
    // Some servers give junk after the charset parameter, which may
    // include a comma, so this check makes us a bit more tolerant.
    if (contentTypeStr.length() == 0 || contentTypeStr == "*/*"
        || contentTypeStr.find_first_of('/') == std::string::npos)
    {
        return;
    }

    // If type_val is the same as mime_type, then just update the charset.
    // However, if charset is empty and mime_type hasn't changed, then don't
    // wipe-out an existing charset.
    // It is common that mime_type is empty.
    bool eq = !mimeType->empty()
        && base::LowerCaseEqualsASCII(std::string_view(contentTypeStr.data() + typeVal, typeEnd - typeVal), mimeType->data());
    if (!eq)
        *mimeType = base::ToLowerASCII(base::StringPiece(contentTypeStr.data() + typeVal, typeEnd - typeVal));
    if ((!eq && *hadCharset) || typeHasCharset)
    {
        *hadCharset = true;
        *charset = base::ToLowerASCII(charsetValue);
    }
}

std::string_view HttpUtil::TrimLWS(const std::string_view &string)
{
    const char *begin = string.data();
    const char *end = string.data() + string.size();
    TrimLWSImplementation(&begin, &end);
    return std::string_view(begin, end - begin);
}

}  // namespace net
