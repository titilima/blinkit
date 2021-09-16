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

namespace blink {

std::string KURL::ExtractFileName(void) const
{
    std::string path = get_path();
    size_t p = path.rfind('/');
    return std::string::npos != p ? path.substr(p + 1) : path;
}

String KURL::PartToString(const zed::url_parts::part& part) const
{
    ASSERT(is_valid());
    return String::fromUTF8(part.data(), part.length());
}

const KURL& blankURL(void)
{
    static KURL staticBlankURL("about:blank");
    return staticBlankURL;
}

} // namespace blink
