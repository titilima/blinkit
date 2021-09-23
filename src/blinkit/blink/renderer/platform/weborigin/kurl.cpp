// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: kurl.cpp
// Description: Placeholder for KURL
//      Author: Ziming Li
//     Created: 2021-08-11
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./KURL.h"

#include "blinkit/blink/renderer/wtf/text/TextEncoding.h"

namespace blink {

KURL::KURL(const KURL &base, const String &relative)
{
    ASSERT(false); // BKTODO:
}

KURL::KURL(const KURL &base, const String &relative, const WTF::TextEncoding &encoding)
    : zed::url(base.combine(relative.stdUtf8()))
{
    ASSERT(zed::strequ(encoding.name(), "UTF-8")); // BKTODO: Support other encodings?
}

std::string KURL::ExtractFileName(void) const
{
    std::string path = get_path();
    size_t p = path.rfind('/');
    return std::string::npos != p ? path.substr(p + 1) : path;
}

String KURL::PartToString(const zed::url_parts::part& part) const
{
    ASSERT(is_valid());
    std::string_view scheme = piece_of(part);
    return String::fromUTF8(scheme.data(), scheme.length());
}

const KURL& blankURL(void)
{
    static KURL staticBlankURL("about:blank");
    return staticBlankURL;
}

} // namespace blink
