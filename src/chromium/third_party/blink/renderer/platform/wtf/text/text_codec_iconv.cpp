// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: text_codec_iconv.cpp
// Description: TextCodecIconv Class
//      Author: Ziming Li
//     Created: 2020-06-25
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "text_codec_iconv.h"

#include <iterator>

namespace BlinKit {

TextCodecIconv::TextCodecIconv(const char *name) : m_iconv(iconv_open("UTF-8", name))
{
    ASSERT(0 != m_iconv);
}

TextCodecIconv::~TextCodecIconv(void)
{
    if (0 != m_iconv)
        iconv_close(m_iconv);
}

String TextCodecIconv::Decode(
    const char *bytes, wtf_size_t length,
    WTF::FlushBehavior flush,
    bool stopOnError, bool &sawError)
{
    static const size_t BufSize = 256;

    if (0 == m_iconv)
        return String();

    std::string ret;

    char buf[BufSize];
    char *in = const_cast<char *>(bytes);
    size_t left = length;
    while (left > 0)
    {
        char *out = buf;
        size_t bufLeft = BufSize;
        iconv(m_iconv, &in, &left, &out, &bufLeft);
        if (bufLeft == BufSize)
        {
            // Convert error, skip the char.
            ret.push_back('?');
            ++in; --left;
        }
        else
        {
            ret.append(buf, BufSize - bufLeft);
        }
    }

    return String::FromStdUTF8(ret);
}

CString TextCodecIconv::Encode(const UChar *characters, wtf_size_t length, WTF::UnencodableHandling handling)
{
    ASSERT(false); // BKTODO:
    return CString();
}

CString TextCodecIconv::Encode(const LChar *characters, wtf_size_t length, WTF::UnencodableHandling handling)
{
    ASSERT(false); // BKTODO:
    return CString();
}

} // namespace BlinKit
