// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: text_codec_win.h
// Description: TextCodecWin Class
//      Author: Ziming Li
//     Created: 2019-06-20
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_TEXT_CODEC_WIN_H
#define BLINKIT_BLINK_TEXT_CODEC_WIN_H

#pragma once

#include <Windows.h>
#include "wtf/text/TextCodec.h"

namespace WTF {

class TextCodecWin final : public TextCodec
{
public:
    static void RegisterEncodingNames(EncodingNameRegistrar);
    static void RegisterCodecs(TextCodecRegistrar);
private:
    TextCodecWin(UINT codePage);
    static PassOwnPtr<TextCodec> Create(const TextEncoding &, const void*);

    String decode(const char*, size_t length, FlushBehavior, bool stopOnError, bool& sawError) override;
    CString encode(const UChar*, size_t length, UnencodableHandling) override;
    CString encode(const LChar*, size_t length, UnencodableHandling) override;

    UINT m_codePage;
};

} // namespace WTF

#endif
