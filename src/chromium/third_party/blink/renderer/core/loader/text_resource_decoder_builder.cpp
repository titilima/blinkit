// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: text_resource_decoder_builder.cpp
// Description: TextResourceDecoder Builder Helpers
//      Author: Ziming Li
//     Created: 2019-10-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2013 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "text_resource_decoder_builder.h"

#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/html/parser/text_resource_decoder.h"
#include "third_party/blink/renderer/platform/loader/fetch/text_resource_decoder_options.h"

using namespace BlinKit;

namespace blink {

struct LegacyEncoding {
    const char *domain;
    const char *encoding;
};

static const LegacyEncoding kEncodings[] = {
    {"au", "windows-1252"}, {"az", "ISO-8859-9"},   {"bd", "windows-1252"},
    {"bg", "windows-1251"}, {"br", "windows-1252"}, {"ca", "windows-1252"},
    {"ch", "windows-1252"}, {"cn", "GBK"},          {"cz", "windows-1250"},
    {"de", "windows-1252"}, {"dk", "windows-1252"}, {"ee", "windows-1256"},
    {"eg", "windows-1257"}, {"et", "windows-1252"}, {"fi", "windows-1252"},
    {"fr", "windows-1252"}, {"gb", "windows-1252"}, {"gr", "ISO-8859-7"},
    {"hk", "Big5"},         {"hr", "windows-1250"}, {"hu", "ISO-8859-2"},
    {"il", "windows-1255"}, {"ir", "windows-1257"}, {"is", "windows-1252"},
    {"it", "windows-1252"}, {"jp", "Shift_JIS"},    {"kr", "windows-949"},
    {"lt", "windows-1256"}, {"lv", "windows-1256"}, {"mk", "windows-1251"},
    {"nl", "windows-1252"}, {"no", "windows-1252"}, {"pl", "ISO-8859-2"},
    {"pt", "windows-1252"}, {"ro", "ISO-8859-2"},   {"rs", "windows-1251"},
    {"ru", "windows-1251"}, {"se", "windows-1252"}, {"si", "ISO-8859-2"},
    {"sk", "windows-1250"}, {"th", "windows-874"},  {"tr", "ISO-8859-9"},
    {"tw", "Big5"},         {"tz", "windows-1252"}, {"ua", "windows-1251"},
    {"us", "windows-1252"}, {"vn", "windows-1258"}, {"xa", "windows-1252"},
    {"xb", "windows-1257"}
};

static const WTF::TextEncoding GetEncodingFromDomain(const BkURL &url)
{
    std::string host = url.Host();

    size_t p = host.rfind('.');
    if (std::string::npos != p)
    {
        std::string tld = host.substr(p + 1);
        ASSERT(false); // BKTODO: Test it!
        for (const LegacyEncoding &e : kEncodings)
        {
            if (tld == e.domain)
                return WTF::TextEncoding(e.encoding);
        }
    }
    return WTF::TextEncoding();
}

static TextResourceDecoderOptions::ContentType DetermineContentType(const String &mime_type)
{
    if (DeprecatedEqualIgnoringCase(mime_type, "text/css"))
        return TextResourceDecoderOptions::kCSSContent;
    if (DeprecatedEqualIgnoringCase(mime_type, "text/html"))
        return TextResourceDecoderOptions::kHTMLContent;
    return TextResourceDecoderOptions::kPlainTextContent;
}

std::unique_ptr<TextResourceDecoder> BuildTextResourceDecoderFor(
    Document *document,
    const AtomicString &mimeType, const AtomicString &encoding)
{
    TextResourceDecoderOptions::ContentType contentType = DetermineContentType(mimeType);
    const WTF::TextEncoding defaultEncoding = GetEncodingFromDomain(document->Url());

    TextResourceDecoderOptions options = TextResourceDecoderOptions::CreateWithAutoDetection(contentType,
        defaultEncoding, WTF::TextEncoding(), document->Url());

    std::unique_ptr<TextResourceDecoder> decoder = TextResourceDecoder::Create(options);
    ASSERT(decoder);

    if (!encoding.IsEmpty())
        decoder->SetEncoding(WTF::TextEncoding(encoding.GetString()), TextResourceDecoder::kEncodingFromHTTPHeader);

    return decoder;
}

} // namespace blink
