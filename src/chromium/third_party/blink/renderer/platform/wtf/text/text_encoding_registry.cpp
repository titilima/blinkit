// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: text_encoding_registry.cpp
// Description: Text Encoding Helpers
//      Author: Ziming Li
//     Created: 2019-10-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006, 2007, 2011 Apple Inc. All rights reserved.
 * Copyright (C) 2007-2009 Torch Mobile, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "text_encoding_registry.h"

#include <mutex>
#include <unordered_map>
#include "base/strings/string_util.h"
#include "third_party/blink/renderer/platform/wtf/text/text_codec.h"
#include "third_party/blink/renderer/platform/wtf/text/text_codec_latin1.h"
#include "third_party/blink/renderer/platform/wtf/text/text_codec_replacement.h"
#include "third_party/blink/renderer/platform/wtf/text/text_codec_user_defined.h"
#include "third_party/blink/renderer/platform/wtf/text/text_codec_utf16.h"
#include "third_party/blink/renderer/platform/wtf/text/text_codec_utf8.h"
#include "third_party/blink/renderer/platform/wtf/text/text_encoding.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"

namespace WTF {

struct TextCodecFactory {
    NewTextCodecFunction function;
    const void *additionalData;
    TextCodecFactory(NewTextCodecFunction f = nullptr, const void *d = nullptr) : function(f), additionalData(d) {}
};

static std::mutex g_registryMutex;
static std::unordered_map<std::string, std::string> g_textEncodingNameMap;
static std::unordered_map<std::string, TextCodecFactory> g_textCodecMap;
static bool g_textCodecMapsExtended = false;

static void AddToTextEncodingNameMap(const char *alias, const char *name)
{
    std::string lower = base::ToLowerASCII(alias);
    g_textEncodingNameMap[lower] = name;
}

static void AddToTextCodecMap(const char *name, NewTextCodecFunction function, const void *additionalData)
{
    std::string lower = base::ToLowerASCII(name);
    auto it = g_textEncodingNameMap.find(lower);
    assert(std::end(g_textEncodingNameMap) != it);
    g_textCodecMap[it->second] = TextCodecFactory(function, additionalData);
}

static void BuildBaseTextCodecMaps(void)
{
    assert(IsMainThread());
    assert(g_textCodecMap.empty());
    assert(g_textEncodingNameMap.empty());

    TextCodecLatin1::RegisterEncodingNames(AddToTextEncodingNameMap);
    TextCodecLatin1::RegisterCodecs(AddToTextCodecMap);

    TextCodecUTF8::RegisterEncodingNames(AddToTextEncodingNameMap);
    TextCodecUTF8::RegisterCodecs(AddToTextCodecMap);

    TextCodecUTF16::RegisterEncodingNames(AddToTextEncodingNameMap);
    TextCodecUTF16::RegisterCodecs(AddToTextCodecMap);
}

static void ExtendTextCodecMaps(void)
{
    TextCodecReplacement::RegisterEncodingNames(AddToTextEncodingNameMap);
    TextCodecReplacement::RegisterCodecs(AddToTextCodecMap);

    TextCodecUserDefined::RegisterEncodingNames(AddToTextEncodingNameMap);
    TextCodecUserDefined::RegisterCodecs(AddToTextCodecMap);
}

const char* AtomicCanonicalTextEncodingName(const char *name)
{
    if (nullptr == name || '\0' == *name)
        return nullptr;

    if (g_textEncodingNameMap.empty())
        BuildBaseTextCodecMaps();

    std::string lower = base::ToLowerASCII(name);
    std::lock_guard<std::mutex> lock(g_registryMutex);
    auto it = g_textEncodingNameMap.find(lower);
    do {
        if (std::end(g_textEncodingNameMap) != it || g_textCodecMapsExtended)
            break;

        ExtendTextCodecMaps();
        g_textCodecMapsExtended = true;

        it = g_textEncodingNameMap.find(lower);
    } while (false);

    return std::end(g_textEncodingNameMap) != it ? it->second.c_str() : nullptr;
}

const char* AtomicCanonicalTextEncodingName(const String &alias)
{
    if (0 == alias.length())
        return nullptr;

    std::string utf8 = alias.StdUtf8();
    return AtomicCanonicalTextEncodingName(utf8.c_str());
}

std::unique_ptr<TextCodec> NewTextCodec(const TextEncoding &encoding)
{
    std::lock_guard<std::mutex> lock(g_registryMutex);

    ASSERT(!g_textCodecMap.empty());

    std::string name = encoding.GetName();
    auto it = g_textCodecMap.find(name);
    ASSERT(std::end(g_textCodecMap) != it);
    return it->second.function(encoding, it->second.additionalData);
}

bool NoExtendedTextEncodingNameUsed(void)
{
    std::lock_guard<std::mutex> lock(g_registryMutex);
    return !g_textCodecMapsExtended;
}

}  // namespace WTF
