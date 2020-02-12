// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: text_codec_win.h
// Description: TextCodecWin Class
//      Author: Ziming Li
//     Created: 2020-02-12
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_TEXT_CODEC_WIN_H
#define BLINKIT_BLINK_TEXT_CODEC_WIN_H

#pragma once

#include <Windows.h>
#include "third_party/blink/renderer/platform/wtf/text/text_codec.h"

namespace BlinKit {

class TextCodecWin final : public TextCodec
{
public:
    TextCodecWin(UINT codePage);
private:
    String Decode(const char *bytes, wtf_size_t length, WTF::FlushBehavior flush, bool stopOnError,
        bool &sawError) override;
    CString Encode(const UChar *characters, wtf_size_t length, WTF::UnencodableHandling handling) override;
    CString Encode(const LChar *characters, wtf_size_t length, WTF::UnencodableHandling handling) override;

    UINT m_codePage;
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_TEXT_CODEC_WIN_H
