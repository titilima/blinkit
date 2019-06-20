// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: text_codec_win.cpp
// Description: TextCodecWin Class
//      Author: Ziming Li
//     Created: 2019-06-20
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "text_codec_win.h"

#include "base/strings/sys_string_conversions.h"
#include "wtf/text/TextEncoding.h"

namespace WTF {

TextCodecWin::TextCodecWin(UINT codePage) : m_codePage(codePage)
{
    // Nothing
}

PassOwnPtr<TextCodec> TextCodecWin::Create(const TextEncoding &encoding, const void*)
{
    UINT codePage;

    const char *encodingName = encoding.name();
    if (0 == strcmpi(encodingName, "GBK"))
    {
        codePage = 936;
    }
    else
    {
        ASSERT(false); // Not reached: any other code pages to support?
        return nullptr;
    }

    return adoptPtr(new TextCodecWin(codePage));
}

String TextCodecWin::decode(const char *bytes, size_t length, FlushBehavior flush, bool stopOnError, bool &sawError)
{
    std::wstring ws = base::SysMultiByteToWide(base::StringPiece(bytes, length), m_codePage);
    return String(ws.data(), ws.length());
}

CString TextCodecWin::encode(const UChar *characters, size_t length, UnencodableHandling handling)
{
    ASSERT(false); // BKTODO:
    return CString();
}

CString TextCodecWin::encode(const LChar *characters, size_t length, UnencodableHandling handling)
{
    ASSERT(false); // BKTODO:
    return CString();
}

void TextCodecWin::RegisterCodecs(TextCodecRegistrar registrar)
{
    registrar("GBK", Create, nullptr);
}

void TextCodecWin::RegisterEncodingNames(EncodingNameRegistrar registrar)
{
    registrar("GBK", "GBK");
    registrar("gb2312", "GBK");
}

} // namespace WTF
