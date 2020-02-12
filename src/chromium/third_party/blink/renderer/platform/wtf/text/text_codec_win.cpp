// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: text_codec_win.cpp
// Description: TextCodecWin Class
//      Author: Ziming Li
//     Created: 2020-02-12
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "text_codec_win.h"

#include "base/strings/sys_string_conversions.h"

namespace BlinKit {

TextCodecWin::TextCodecWin(UINT codePage) : m_codePage(codePage)
{
}

String TextCodecWin::Decode(
    const char *bytes, wtf_size_t length,
    WTF::FlushBehavior flush,
    bool stopOnError, bool &sawError)
{
    std::wstring ws = base::SysMultiByteToWide(base::StringPiece(bytes, length), m_codePage);
    return String(ws.data(), ws.length());
}

CString TextCodecWin::Encode(const UChar *characters, wtf_size_t length, WTF::UnencodableHandling handling)
{
    ASSERT(false); // BKTODO:
    return CString();
}

CString TextCodecWin::Encode(const LChar *characters, wtf_size_t length, WTF::UnencodableHandling handling)
{
    ASSERT(false); // BKTODO:
    return CString();
}

} // namespace BlinKit
