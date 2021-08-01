#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: KURL.h
// Description: Placeholder for KURL
//      Author: Ziming Li
//     Created: 2021-07-05
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_KURL_H
#define BLINKIT_BLINKIT_KURL_H

#include "blinkit/blink/renderer/wtf/text/WTFString.h"
#include "third_party/zed/include/zed/net/url.hpp"

namespace blink {

class KURL final : public zed::url
{
public:
    bool isEmpty(void) const { return spec().empty(); }
    bool isValid(void) const { return is_valid(); }

    String string(void) const { return String::fromStdUTF8(spec()); }

    String protocol(void) const { return is_valid() ? partToString(raw_parts().scheme) : String(); }
    bool protocolIs(const char *protocol) const { return scheme_is(protocol); }
    bool isLocalFile(void) const { return scheme_is_file(); }
    bool protocolIsAbout(void) const { return scheme_is("about"); }
    bool protocolIsData(void) const { return scheme_is("data"); }
    bool protocolIsInHTTPFamily(void) const { return scheme_is_in_http_family(); }

    String user(void) const { return is_valid() ? partToString(raw_parts().username) : String(); }
    String pass(void) const { return is_valid() ? partToString(raw_parts().password) : String(); }
    String host(void) const { return is_valid() ? partToString(raw_parts().host) : String(); }
    String path(void) const { return is_valid() ? partToString(raw_parts().path) : String(); }
private:
    String partToString(const zed::url_parts::part& part) const
    {
        ASSERT(is_valid());
        return String::fromUTF8(part.data(), part.length());
    }
};

} // namespace blink

#endif // BLINKIT_BLINKIT_KURL_H
