// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: text_resource_decoder.h
// Description: TextResourceDecoder Class
//      Author: Ziming Li
//     Created: 2019-10-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
    Copyright (C) 1999 Lars Knoll (knoll@mpi-hd.mpg.de)
    Copyright (C) 2006 Alexey Proskuryakov (ap@nypop.com)
    Copyright (C) 2006, 2008 Apple Inc. All rights reserved.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.

*/

#ifndef BLINKIT_BLINK_TEXT_RESOURCE_DECODER_H
#define BLINKIT_BLINK_TEXT_RESOURCE_DECODER_H

#pragma once

#include "base/macros.h"
#include "base/memory/ptr_util.h"
#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/platform/loader/fetch/text_resource_decoder_options.h"
#include "third_party/blink/renderer/platform/wtf/allocator.h"
#include "third_party/blink/renderer/platform/wtf/text/text_encoding.h"

namespace blink {

class TextResourceDecoder
{
    USING_FAST_MALLOC(TextResourceDecoder);
public:
    enum EncodingSource {
        kDefaultEncoding,
        kAutoDetectedEncoding,
        kEncodingFromContentSniffing,
        kEncodingFromXMLHeader,
        kEncodingFromMetaTag,
        kEncodingFromCSSCharset,
        kEncodingFromHTTPHeader,
        kEncodingFromParentFrame
    };

    static std::unique_ptr<TextResourceDecoder> Create(const TextResourceDecoderOptions &options)
    {
        return base::WrapUnique(new TextResourceDecoder(options));
    }
    ~TextResourceDecoder(void);

    const WTF::TextEncoding& Encoding(void) const { return m_encoding; }
    void SetEncoding(const WTF::TextEncoding &encoding, EncodingSource source);

    bool EncodingWasDetectedHeuristically(void) const
    {
        return kAutoDetectedEncoding == m_source || kEncodingFromContentSniffing == m_source;
    }
    bool SawError(void) const { return m_sawError; }

    String Decode(const char *data, size_t length);
    String Flush(void);

    wtf_size_t CheckForBOM(const char *data, wtf_size_t len);
protected:
    TextResourceDecoder(const TextResourceDecoderOptions &options);
private:
    void AutoDetectEncodingIfAllowed(const char *data, wtf_size_t len);
    bool CheckForCSSCharset(const char *data, wtf_size_t len, bool &movedDataToBuffer);
    bool CheckForXMLCharset(const char *data, wtf_size_t len, bool &movedDataToBuffer);
    void CheckForMetaCharset(const char *data, wtf_size_t length);

    const TextResourceDecoderOptions m_options;
    WTF::TextEncoding m_encoding;
    std::unique_ptr<TextCodec> m_codec;
    EncodingSource m_source = kDefaultEncoding;
    Vector<char> m_buffer;
    bool m_checkedForBom = false;
    bool m_checkedForCssCharset = false;
    bool m_checkedForXmlCharset = false;
    bool m_checkedForMetaCharset = false;
    bool m_sawError = false;
    bool m_detectionCompleted = false;

    DISALLOW_COPY_AND_ASSIGN(TextResourceDecoder);
};

}  // namespace blink

#endif // BLINKIT_BLINK_TEXT_RESOURCE_DECODER_H
