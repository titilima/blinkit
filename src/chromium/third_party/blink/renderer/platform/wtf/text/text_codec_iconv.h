// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: text_codec_iconv.h
// Description: TextCodecIconv Class
//      Author: Ziming Li
//     Created: 2020-06-25
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_TEXT_CODEC_ICONV_H
#define BLINKIT_BLINK_TEXT_CODEC_ICONV_H

#pragma once

#include <iconv.h>
#include "third_party/blink/renderer/platform/wtf/text/text_codec.h"

namespace BlinKit {

class TextCodecIconv final : public TextCodec
{
public:
    TextCodecIconv(const char *name);
    virtual ~TextCodecIconv(void) override;
private:
    String Decode(const char *bytes, wtf_size_t length, WTF::FlushBehavior flush, bool stopOnError,
        bool &sawError) override;
    CString Encode(const UChar *characters, wtf_size_t length, WTF::UnencodableHandling handling) override;
    CString Encode(const LChar *characters, wtf_size_t length, WTF::UnencodableHandling handling) override;

    iconv_t m_iconv;
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_TEXT_CODEC_ICONV_H
