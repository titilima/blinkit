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

#include "third_party/blink/renderer/core/html/parser/html_meta_charset_parser.h"
#include "third_party/blink/renderer/platform/wtf/string_extras.h"
#include "third_party/blink/renderer/platform/wtf/text/text_encoding_registry.h"

namespace blink {

static const int kMinimumLengthOfXMLDeclaration = 8;

static inline bool BytesEqual(const char *p, char b0, char b1, char b2, char b3, char b4)
{
    return p[0] == b0 && p[1] == b1 && p[2] == b2 && p[3] == b3 && p[4] == b4;
}

static inline bool BytesEqual(const char *p, char b0, char b1, char b2, char b3, char b4, char b5)
{
    return p[0] == b0 && p[1] == b1 && p[2] == b2 && p[3] == b3 && p[4] == b4 && p[5] == b5;
}

TextResourceDecoder::TextResourceDecoder(const TextResourceDecoderOptions &options)
    : m_options(options)
    , m_encoding(DefaultEncoding(m_options.GetContentType(), m_options.DefaultEncoding()))
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
    if (m_options.GetEncodingDetectionOption() != TextResourceDecoderOptions::kUseAllAutoDetection || m_detectionCompleted)
        return;

    // Just checking hint_encoding_ suffices here because it's only set
    // in SetHintEncoding when the source is AutoDetectedEncoding.
    if (!(kDefaultEncoding == m_source || (kEncodingFromParentFrame == m_source && m_options.HintEncoding())))
        return;

    WTF::TextEncoding detectedEncoding;
    ASSERT(false); // BKTODO:
#if 0
    if (DetectTextEncoding(data, len, m_options.HintEncoding(), m_options.HintURL(), m_options.HintLanguage(), &detectedEncoding))
        SetEncoding(detectedEncoding, kEncodingFromContentSniffing);
#endif
    if (detectedEncoding != WTF::UnknownEncoding())
        m_detectionCompleted = true;
}

wtf_size_t TextResourceDecoder::CheckForBOM(const char *data, wtf_size_t len)
{
    // Check for UTF-16 or UTF-8 BOM mark at the beginning, which is a sure
    // sign of a Unicode encoding. We let it override even a user-chosen encoding.
    ASSERT(!m_checkedForBom);

    wtf_size_t lengthOfBom = 0;
    const wtf_size_t maxBomLength = 3;

    wtf_size_t bufferLength = m_buffer.size();

    wtf_size_t buf1Len = bufferLength;
    wtf_size_t buf2Len = len;
    const unsigned char *buf1 = reinterpret_cast<const unsigned char *>(m_buffer.data());
    const unsigned char *buf2 = reinterpret_cast<const unsigned char *>(data);
    unsigned char c1 = buf1Len ? (--buf1Len, *buf1++) : buf2Len ? (--buf2Len, *buf2++) : 0;
    unsigned char c2 = buf1Len ? (--buf1Len, *buf1++) : buf2Len ? (--buf2Len, *buf2++) : 0;
    unsigned char c3 = buf1Len ? *buf1 : buf2Len ? *buf2 : 0;

    // Check for the BOM.
    if (c1 == 0xEF && c2 == 0xBB && c3 == 0xBF)
    {
        SetEncoding(UTF8Encoding(), kAutoDetectedEncoding);
        lengthOfBom = 3;
    }
    else if (m_options.GetEncodingDetectionOption() != TextResourceDecoderOptions::kAlwaysUseUTF8ForText)
    {
        if (c1 == 0xFE && c2 == 0xFF)
        {
            SetEncoding(UTF16BigEndianEncoding(), kAutoDetectedEncoding);
            lengthOfBom = 2;
        }
        else if (c1 == 0xFF && c2 == 0xFE)
        {
            SetEncoding(UTF16LittleEndianEncoding(), kAutoDetectedEncoding);
            lengthOfBom = 2;
        }
    }

    if (lengthOfBom || bufferLength + len >= maxBomLength)
        m_checkedForBom = true;

    return lengthOfBom;
}

bool TextResourceDecoder::CheckForCSSCharset(const char *data, wtf_size_t len, bool &movedDataToBuffer)
{
    if (kDefaultEncoding != m_source && kEncodingFromParentFrame != m_source)
    {
        m_checkedForCssCharset = true;
        return true;
    }

    wtf_size_t oldSize = m_buffer.size();
    m_buffer.Grow(oldSize + len);
    memcpy(m_buffer.data() + oldSize, data, len);

    movedDataToBuffer = true;

    if (m_buffer.size() <= 13)  // strlen('@charset "x";') == 13
        return false;

    const char *dataStart = m_buffer.data();
    const char *dataEnd = dataStart + m_buffer.size();

    if (0 == memcmp(dataStart, "@charset \"", 10))
    {
        dataStart += 10;
        const char *pos = dataStart;

        while (pos < dataEnd && *pos != '"')
            ++pos;
        if (pos == dataEnd)
            return false;

        wtf_size_t encodingNameLength = static_cast<wtf_size_t>(pos - dataStart);

        ++pos;
        if (pos == dataEnd)
            return false;

        if (*pos == ';')
        {
            std::string encodingName(dataStart, encodingNameLength);
            SetEncoding(WTF::TextEncoding(encodingName.c_str()), kEncodingFromCSSCharset);
        }
    }

    m_checkedForCssCharset = true;
    return true;
}

void TextResourceDecoder::CheckForMetaCharset(const char *data, wtf_size_t length)
{
    if (kEncodingFromHTTPHeader == m_source || kAutoDetectedEncoding == m_source)
    {
        m_checkedForMetaCharset = true;
        return;
    }

    if (!m_charsetParser)
        m_charsetParser = HTMLMetaCharsetParser::Create();

    if (!m_charsetParser->CheckForMetaCharset(data, length))
        return;

    SetEncoding(m_charsetParser->Encoding(), kEncodingFromMetaTag);
    m_charsetParser.reset();
    m_checkedForMetaCharset = true;
}

bool TextResourceDecoder::CheckForXMLCharset(const char *data, wtf_size_t len, bool &movedDataToBuffer)
{
    if (m_source != kDefaultEncoding && m_source != kEncodingFromParentFrame)
    {
        m_checkedForXmlCharset = true;
        return true;
    }

    // This is not completely efficient, since the function might go
    // through the HTML head several times.

    wtf_size_t oldSize = m_buffer.size();
    m_buffer.Grow(oldSize + len);
    memcpy(m_buffer.data() + oldSize, data, len);

    movedDataToBuffer = true;

    const char *ptr = m_buffer.data();
    const char *pEnd = ptr + m_buffer.size();

    // Is there enough data available to check for XML declaration?
    if (m_buffer.size() < kMinimumLengthOfXMLDeclaration)
        return false;

    // Handle XML declaration, which can have encoding in it. This encoding is
    // honored even for HTML documents. It is an error for an XML declaration not
    // to be at the start of an XML document, and it is ignored in HTML documents
    // in such case.
    if (BytesEqual(ptr, '<', '?', 'x', 'm', 'l'))
    {
        ASSERT(false); // BKTODO:
#if 0
        const char* xml_declaration_end = ptr;
        while (xml_declaration_end != p_end && *xml_declaration_end != '>')
            ++xml_declaration_end;
        if (xml_declaration_end == p_end)
            return false;
        // No need for +1, because we have an extra "?" to lose at the end of XML
        // declaration.
        int len = 0;
        int pos =
            FindXMLEncoding(ptr, static_cast<int>(xml_declaration_end - ptr), len);
        if (pos != -1)
            SetEncoding(FindTextEncoding(ptr + pos, len), kEncodingFromXMLHeader);
        // continue looking for a charset - it may be specified in an HTTP-Equiv
        // meta
#endif
    }
    else if (BytesEqual(ptr, '<', 0, '?', 0, 'x', 0))
    {
        SetEncoding(UTF16LittleEndianEncoding(), kAutoDetectedEncoding);
    }
    else if (BytesEqual(ptr, 0, '<', 0, '?', 0, 'x'))
    {
        SetEncoding(UTF16BigEndianEncoding(), kAutoDetectedEncoding);
    }

    m_checkedForXmlCharset = true;
    return true;
}

String TextResourceDecoder::Decode(const char *data, size_t dataLen)
{
    wtf_size_t len = SafeCast<wtf_size_t>(dataLen);
    wtf_size_t lengthOfBom = 0;
    if (!m_checkedForBom)
    {
        lengthOfBom = CheckForBOM(data, len);

        // BOM check can fail when the available data is not enough.
        if (!m_checkedForBom)
        {
            ASSERT(0 == lengthOfBom);
            m_buffer.Append(data, len);
            return g_empty_string;
        }
    }
    ASSERT(lengthOfBom <= m_buffer.size() + len);

    bool movedDataToBuffer = false;
    const TextResourceDecoderOptions::ContentType contentType = m_options.GetContentType();

    if (contentType == TextResourceDecoderOptions::kCSSContent && !m_checkedForCssCharset)
    {
        if (!CheckForCSSCharset(data, len, movedDataToBuffer))
            return g_empty_string;
    }

    // We check XML declaration in HTML content only if there is enough data
    // available
    if (((contentType == TextResourceDecoderOptions::kHTMLContent && len >= kMinimumLengthOfXMLDeclaration)
        || contentType == TextResourceDecoderOptions::kXMLContent) && !m_checkedForXmlCharset)
    {
        if (!CheckForXMLCharset(data, len, movedDataToBuffer))
            return g_empty_string;
    }

    const char *dataForDecode = data + lengthOfBom;
    wtf_size_t lengthForDecode = len - lengthOfBom;

    if (!m_buffer.IsEmpty())
    {
        if (!movedDataToBuffer)
        {
            wtf_size_t oldSize = m_buffer.size();
            m_buffer.Grow(oldSize + len);
            memcpy(m_buffer.data() + oldSize, data, len);
        }

        dataForDecode = m_buffer.data() + lengthOfBom;
        lengthForDecode = m_buffer.size() - lengthOfBom;
    }

    if (contentType == TextResourceDecoderOptions::kHTMLContent && !m_checkedForMetaCharset)
        CheckForMetaCharset(dataForDecode, lengthForDecode);

    AutoDetectEncodingIfAllowed(data, len);

    ASSERT(m_encoding.IsValid());

    if (!m_codec)
        m_codec = NewTextCodec(m_encoding);

    String result = m_codec->Decode(dataForDecode, lengthForDecode, WTF::FlushBehavior::kDoNotFlush,
        contentType == TextResourceDecoderOptions::kXMLContent && !m_options.GetUseLenientXMLDecoding(),
        m_sawError);

    m_buffer.clear();
    return result;
}

const WTF::TextEncoding& TextResourceDecoder::DefaultEncoding(
    TextResourceDecoderOptions::ContentType contentType,
    const WTF::TextEncoding& specifiedDefaultEncoding)
{
    // Despite 8.5 "Text/xml with Omitted Charset" of RFC 3023, we assume UTF-8
    // instead of US-ASCII for text/xml. This matches Firefox.
    if (contentType == TextResourceDecoderOptions::kXMLContent
        || contentType == TextResourceDecoderOptions::kJSONContent)
    {
        return UTF8Encoding();
    }

    if (!specifiedDefaultEncoding.IsValid())
        return Latin1Encoding();
    return specifiedDefaultEncoding;
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
                [[fallthrough]];
            default:
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
