// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: text_resource_decoder.cpp
// Description: TextResourceDecoder Class
//      Author: Ziming Li
//     Created: 2019-10-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
    Copyright (C) 1999 Lars Knoll (knoll@mpi-hd.mpg.de)
    Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2012 Apple Inc. All
    rights reserved.
    Copyright (C) 2005, 2006, 2007 Alexey Proskuryakov (ap@nypop.com)

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

#include "text_resource_decoder.h"

#include "third_party/blink/renderer/platform/wtf/string_extras.h"
#include "third_party/blink/renderer/platform/wtf/text/text_encoding_registry.h"

namespace blink {

TextResourceDecoder::TextResourceDecoder(const TextResourceDecoderOptions &options) : m_options(options)
{
#ifndef NDEBUG
    // TODO(hiroshige): Move the invariant check to TextResourceDecoderOptions.
    if (m_options.GetEncodingDetectionOption() == TextResourceDecoderOptions::kAlwaysUseUTF8ForText)
    {
        ASSERT(m_options.GetContentType() == TextResourceDecoderOptions::kPlainTextContent);
        ASSERT(m_encoding == UTF8Encoding());
    }
#endif
}

TextResourceDecoder::~TextResourceDecoder(void) = default;

void TextResourceDecoder::AutoDetectEncodingIfAllowed(const char *data, wtf_size_t len)
{
    ASSERT(false); // BKTODO:
}

String TextResourceDecoder::Decode(const char *data, size_t length)
{
    ASSERT(false); // BKTODO:
    return String();
}

String TextResourceDecoder::Flush(void)
{
    // If we can not identify the encoding even after a document is completely
    // loaded, we need to detect the encoding if other conditions for
    // autodetection is satisfied.
    if (0 != m_buffer.size())
    {
        bool tryAutoDetectEncoding = false;
        switch (m_options.GetContentType())
        {
            case TextResourceDecoderOptions::kHTMLContent:
            case TextResourceDecoderOptions::kXMLContent:
                if (!m_checkedForXmlCharset)
                    tryAutoDetectEncoding = true;
                break;
            case TextResourceDecoderOptions::kCSSContent:
                if (!m_checkedForCssCharset)
                    tryAutoDetectEncoding = true;
                break;
        }

        if (tryAutoDetectEncoding)
            AutoDetectEncodingIfAllowed(m_buffer.data(), m_buffer.size());
    }

    if (!m_codec)
        m_codec = NewTextCodec(m_encoding);

    String result = m_codec->Decode(m_buffer.data(), m_buffer.size(), WTF::FlushBehavior::kFetchEOF,
        m_options.GetContentType() == TextResourceDecoderOptions::kXMLContent && !m_options.GetUseLenientXMLDecoding(),
        m_sawError);
    m_buffer.clear();
    m_codec.reset();
    m_checkedForBom = false;  // Skip BOM again when re-decoding.
    return result;
}

void TextResourceDecoder::SetEncoding(const WTF::TextEncoding &encoding, EncodingSource source)
{
    // In case the encoding didn't exist, we keep the old one (helps some sites
    // specifying invalid encodings).
    if (!encoding.IsValid())
        return;

    // Always use UTF-8 for |kAlwaysUseUTF8ForText|.
    if (m_options.GetEncodingDetectionOption() == TextResourceDecoderOptions::kAlwaysUseUTF8ForText)
        return;

    // When encoding comes from meta tag (i.e. it cannot be XML files sent via
    // XHR), treat x-user-defined as windows-1252 (bug 18270)
    if (kEncodingFromMetaTag == source && !strcasecmp(encoding.GetName(), "x-user-defined"))
        m_encoding = WTF::TextEncoding("windows-1252");
    else if (kEncodingFromMetaTag == source || kEncodingFromXMLHeader == source || kEncodingFromCSSCharset == source)
        m_encoding = encoding.ClosestByteBasedEquivalent();
    else
        m_encoding = encoding;

    m_codec.reset();
    m_source = source;
}

}  // namespace blink
