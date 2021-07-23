// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: TextResource.cpp
// Description: TextResource Class
//      Author: Ziming Li
//     Created: 2021-07-23
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/fetch/TextResource.h"

#include "core/html/parser/TextResourceDecoder.h"
#include "platform/SharedBuffer.h"
#include "wtf/text/StringBuilder.h"

namespace blink {

TextResource::TextResource(const ResourceRequest& resourceRequest, Resource::Type type, const String& mimeType, const String& charset)
    : Resource(resourceRequest, type)
    , m_decoder(TextResourceDecoder::create(mimeType, charset))
{
}

TextResource::~TextResource()
{
}

void TextResource::setEncoding(const String& chs)
{
    m_decoder->setEncoding(chs, TextResourceDecoder::EncodingFromHTTPHeader);
}

String TextResource::encoding() const
{
    return m_decoder->encoding().name();
}

String TextResource::decodedText() const
{
    ASSERT(m_data);

    StringBuilder builder;
    ASSERT(false); // BKTODO:
#if 0
    const char* data;
    size_t position = 0;
    while (size_t length = m_data->getSomeData(data, position)) {
        builder.append(m_decoder->decode(data, length));
        position += length;
    }
    builder.append(m_decoder->flush());
#endif
    return builder.toString();
}

} // namespace blink
