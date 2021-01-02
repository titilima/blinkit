// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: mime_type_registry.cc
// Description: MIMETypeRegistry Class
//      Author: Ziming Li
//     Created: 2019-10-31
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/platform/network/mime/mime_type_registry.h"

// BKTODO: #include "base/files/file_path.h"
#include "base/strings/string_util.h"
// BKTODO: #include "media/base/mime_util.h"
// BKTODO: #include "media/filters/stream_parser_factory.h"
// BKTODO: #include "net/base/mime_util.h"
// BKTODO: #include "third_party/blink/public/common/mime_util/mime_util.h"
// BKTODO: #include "third_party/blink/public/platform/file_path_conversion.h"
// BKTODO: #include "third_party/blink/public/platform/interface_provider.h"
// BKTODO: #include "third_party/blink/public/platform/mime_registry.mojom-blink.h"
#include "third_party/blink/public/platform/platform.h"
#include "third_party/blink/renderer/platform/wtf/assertions.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

namespace blink {

namespace MIMETypes {
static const char TextCSS[] = "text/css";
static const char TextHTML[] = "text/html";
}

namespace {

#if 0 // BKTODO:
struct MimeRegistryPtrHolder {
 public:
  MimeRegistryPtrHolder() {
    Platform::Current()->GetInterfaceProvider()->GetInterface(
        mojo::MakeRequest(&mime_registry));
  }
  ~MimeRegistryPtrHolder() = default;

  mojom::blink::MimeRegistryPtr mime_registry;
};

std::string ToASCIIOrEmpty(const WebString& string) {
  return string.ContainsOnlyASCII() ? string.Ascii() : std::string();
}
#endif

template <typename CHARTYPE, typename SIZETYPE>
std::string ToLowerASCIIInternal(CHARTYPE* str, SIZETYPE length) {
  std::string lower_ascii;
  lower_ascii.reserve(length);
  for (CHARTYPE* p = str; p < str + length; p++)
    lower_ascii.push_back(base::ToLowerASCII(static_cast<char>(*p)));
  return lower_ascii;
}

// Does the same as ToASCIIOrEmpty, but also makes the chars lower.
std::string ToLowerASCIIOrEmpty(const String& str) {
  if (str.IsEmpty() || !str.ContainsOnlyASCII())
    return std::string();
  if (str.Is8Bit())
    return ToLowerASCIIInternal(str.Characters8(), str.length());
  return ToLowerASCIIInternal(str.Characters16(), str.length());
}

// BKTODO: STATIC_ASSERT_ENUM(MIMETypeRegistry::kIsNotSupported, media::IsNotSupported);
// BKTODO: STATIC_ASSERT_ENUM(MIMETypeRegistry::kIsSupported, media::IsSupported);
// BKTODO: STATIC_ASSERT_ENUM(MIMETypeRegistry::kMayBeSupported, media::MayBeSupported);

}  // namespace

String MIMETypeRegistry::GetMIMETypeForExtension(const String& ext) {
  return GetWellKnownMIMETypeForExtension(ext);
}

String MIMETypeRegistry::GetWellKnownMIMETypeForExtension(const String& ext) {
  using namespace WTF;
  if (0 == CodePointCompareIgnoringASCIICase(ext, "css"))
    return String::FromUTF8(MIMETypes::TextCSS);
  if (0 == CodePointCompareIgnoringASCIICase(ext, "html"))
    return String::FromUTF8(MIMETypes::TextHTML);
#ifndef NDEBUG
  const std::string s = ext.StdUtf8();
  ASSERT(false); // BKTODO:
#endif
  return String();
}

bool MIMETypeRegistry::IsSupportedMIMEType(const String &mime_type)
{
  using namespace WTF;
  if (0 == CodePointCompareIgnoringASCIICase(mime_type, MIMETypes::TextCSS)
      || 0 == CodePointCompareIgnoringASCIICase(mime_type, MIMETypes::TextHTML))
  {
    return true;
  }
#ifndef NDEBUG
  const std::string s = mime_type.StdUtf8();
  ASSERT(false); // BKTODO:
#endif
  return false;
}

bool MIMETypeRegistry::IsSupportedImageMIMEType(const String& mime_type) {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return blink::IsSupportedImageMimeType(ToLowerASCIIOrEmpty(mime_type));
#endif
}

bool MIMETypeRegistry::IsSupportedImageResourceMIMEType(
    const String& mime_type) {
  return IsSupportedImageMIMEType(mime_type);
}

bool MIMETypeRegistry::IsSupportedImagePrefixedMIMEType(
    const String& mime_type) {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  std::string ascii_mime_type = ToLowerASCIIOrEmpty(mime_type);
  return (blink::IsSupportedImageMimeType(ascii_mime_type) ||
          (base::StartsWith(ascii_mime_type, "image/",
                            base::CompareCase::SENSITIVE) &&
           blink::IsSupportedNonImageMimeType(ascii_mime_type)));
#endif
}

bool MIMETypeRegistry::IsSupportedImageMIMETypeForEncoding(
    const String& mime_type) {
  return (EqualIgnoringASCIICase(mime_type, "image/jpeg") ||
          EqualIgnoringASCIICase(mime_type, "image/png") ||
          EqualIgnoringASCIICase(mime_type, "image/webp"));
}

bool MIMETypeRegistry::IsModernImageMIMEType(const String& mime_type) {
  return (EqualIgnoringASCIICase(mime_type, "image/gif") ||
          EqualIgnoringASCIICase(mime_type, "image/jpeg") ||
          EqualIgnoringASCIICase(mime_type, "image/png") ||
          EqualIgnoringASCIICase(mime_type, "image/svg+xml") ||
          EqualIgnoringASCIICase(mime_type, "image/webp"));
}

bool MIMETypeRegistry::IsSupportedJavaScriptMIMEType(const String& mime_type) {
  //  Support every script type mentioned in the spec, as it notes that "User
  //  agents must recognize all JavaScript MIME types." See
  //  https://html.spec.whatwg.org/#javascript-mime-type.
  const char* const kSupportedJavascriptTypes[] = {
    "application/ecmascript",
    "application/javascript",
    "application/x-ecmascript",
    "application/x-javascript",
    "text/ecmascript",
    "text/javascript",
    "text/javascript1.0",
    "text/javascript1.1",
    "text/javascript1.2",
    "text/javascript1.3",
    "text/javascript1.4",
    "text/javascript1.5",
    "text/jscript",
    "text/livescript",
    "text/x-ecmascript",
    "text/x-javascript",
  };

  const std::string s = ToLowerASCIIOrEmpty(mime_type);
  for (const char *ps : kSupportedJavascriptTypes)
  {
    if (s == ps)
      return true;
  }
  return false;
}

bool MIMETypeRegistry::IsLegacySupportedJavaScriptLanguage(
    const String& language) {
  // Mozilla 1.8 accepts javascript1.0 - javascript1.7, but WinIE 7 accepts only
  // javascript1.1 - javascript1.3.
  // Mozilla 1.8 and WinIE 7 both accept javascript and livescript.
  // WinIE 7 accepts ecmascript and jscript, but Mozilla 1.8 doesn't.
  // Neither Mozilla 1.8 nor WinIE 7 accept leading or trailing whitespace.
  // We want to accept all the values that either of these browsers accept, but
  // not other values.

  // FIXME: This function is not HTML5 compliant. These belong in the MIME
  // registry as "text/javascript<version>" entries.
  return EqualIgnoringASCIICase(language, "javascript") ||
         EqualIgnoringASCIICase(language, "javascript1.0") ||
         EqualIgnoringASCIICase(language, "javascript1.1") ||
         EqualIgnoringASCIICase(language, "javascript1.2") ||
         EqualIgnoringASCIICase(language, "javascript1.3") ||
         EqualIgnoringASCIICase(language, "javascript1.4") ||
         EqualIgnoringASCIICase(language, "javascript1.5") ||
         EqualIgnoringASCIICase(language, "javascript1.6") ||
         EqualIgnoringASCIICase(language, "javascript1.7") ||
         EqualIgnoringASCIICase(language, "livescript") ||
         EqualIgnoringASCIICase(language, "ecmascript") ||
         EqualIgnoringASCIICase(language, "jscript");
}

bool MIMETypeRegistry::IsSupportedNonImageMIMEType(const String& mime_type) {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return blink::IsSupportedNonImageMimeType(ToLowerASCIIOrEmpty(mime_type));
#endif
}

bool MIMETypeRegistry::IsSupportedMediaMIMEType(const String& mime_type,
                                                const String& codecs) {
  return SupportsMediaMIMEType(mime_type, codecs) != kIsNotSupported;
}

MIMETypeRegistry::SupportsType MIMETypeRegistry::SupportsMediaMIMEType(
    const String& mime_type,
    const String& codecs) {
  ASSERT(false); // BKTODO:
  return kIsNotSupported;
#if 0
  const std::string ascii_mime_type = ToLowerASCIIOrEmpty(mime_type);
  std::vector<std::string> codec_vector;
  media::SplitCodecs(ToASCIIOrEmpty(codecs), &codec_vector);
  return static_cast<SupportsType>(
      media::IsSupportedMediaFormat(ascii_mime_type, codec_vector));
#endif
}

bool MIMETypeRegistry::IsSupportedMediaSourceMIMEType(const String& mime_type,
                                                      const String& codecs) {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  const std::string ascii_mime_type = ToLowerASCIIOrEmpty(mime_type);
  if (ascii_mime_type.empty())
    return false;
  std::vector<std::string> parsed_codec_ids;
  media::SplitCodecs(ToASCIIOrEmpty(codecs), &parsed_codec_ids);
  return static_cast<MIMETypeRegistry::SupportsType>(
      media::StreamParserFactory::IsTypeSupported(ascii_mime_type,
                                                  parsed_codec_ids));
#endif
}

bool MIMETypeRegistry::IsJavaAppletMIMEType(const String& mime_type) {
  // Since this set is very limited and is likely to remain so we won't bother
  // with the overhead of using a hash set.  Any of the MIME types below may be
  // followed by any number of specific versions of the JVM, which is why we use
  // startsWith()
  return mime_type.StartsWithIgnoringASCIICase("application/x-java-applet") ||
         mime_type.StartsWithIgnoringASCIICase("application/x-java-bean") ||
         mime_type.StartsWithIgnoringASCIICase("application/x-java-vm");
}

bool MIMETypeRegistry::IsSupportedStyleSheetMIMEType(const String& mime_type) {
  return EqualIgnoringASCIICase(mime_type, "text/css");
}

bool MIMETypeRegistry::IsSupportedFontMIMEType(const String& mime_type) {
  static const unsigned kFontLen = 5;
  if (!mime_type.StartsWithIgnoringASCIICase("font/"))
    return false;
  String sub_type = mime_type.Substring(kFontLen).DeprecatedLower();
  return sub_type == "woff" || sub_type == "woff2" || sub_type == "otf" ||
         sub_type == "ttf" || sub_type == "sfnt";
}

bool MIMETypeRegistry::IsSupportedTextTrackMIMEType(const String& mime_type) {
  return EqualIgnoringASCIICase(mime_type, "text/vtt");
}

}  // namespace blink
