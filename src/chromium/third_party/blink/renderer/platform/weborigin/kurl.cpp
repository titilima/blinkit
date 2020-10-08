// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: kurl.cpp
// Description: URL Helpers
//      Author: Ziming Li
//     Created: 2020-06-09
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "kurl.h"

namespace blink {

const GURL& BlankURL(void)
{
    static GURL aboutBlank(url::kAboutBlankURL);
    return aboutBlank;
}

bool EqualIgnoringFragmentIdentifier(const GURL &a, const GURL &b)
{
    // Compute the length of each URL without its ref. Note that the reference
    // begin (if it exists) points to the character *after* the '#', so we need
    // to subtract one.
    int la = a.possibly_invalid_spec().length();
    const url::Component &ra = a.parsed_for_possibly_invalid_spec().ref;
    if (ra.len >= 0)
        la = ra.begin - 1;

    int lb = b.possibly_invalid_spec().length();
    const url::Component &rb = b.parsed_for_possibly_invalid_spec().ref;
    if (rb.len >= 0)
        lb = rb.begin - 1;

    if (la != lb)
        return false;

    const std::string &sa = a.possibly_invalid_spec();
    const std::string &sb = b.possibly_invalid_spec();
    // FIXME: Abstraction this into a function in WTFString.h.
    for (size_t i = 0; i < sa.length(); ++i)
    {
        if (sa.at(i) != sb.at(i))
            return false;
    }
    return true;
}

} // namespace blink
