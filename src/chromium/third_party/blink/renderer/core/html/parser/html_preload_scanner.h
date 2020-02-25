// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_preload_scanner.h
// Description: HTMLPreloadScanner Class
//      Author: Ziming Li
//     Created: 2019-10-18
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2008 Apple Inc. All Rights Reserved.
 * Copyright (C) 2010 Google Inc. All Rights Reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_HTML_PARSER_HTML_PRELOAD_SCANNER_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_HTML_PARSER_HTML_PRELOAD_SCANNER_H_

#include <memory>
#include <utility>

#include "base/macros.h"
#include "base/memory/ptr_util.h"
#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/core/html/parser/compact_html_token.h"
#include "third_party/blink/renderer/core/html/parser/html_token.h"
#include "third_party/blink/renderer/core/html/parser/preload_request.h"
#include "third_party/blink/renderer/platform/text/segmented_string.h"
#include "third_party/blink/renderer/platform/weborigin/security_policy.h"
#include "third_party/blink/renderer/platform/wtf/vector.h"
#include "url/bk_url.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "third_party/blink/renderer/core/css/media_values_cached.h"
#   include "third_party/blink/renderer/core/html/parser/css_preload_scanner.h"
#   include "third_party/blink/renderer/core/page/viewport_description.h"
#endif

namespace blink {

typedef wtf_size_t TokenPreloadScannerCheckpoint;

class Document;
class HTMLParserOptions;
class HTMLTokenizer;
class SegmentedString;

#ifdef BLINKIT_CRAWLER_ONLY

class CSSPreloadScanner;
class MediaValuesCached {
public:
    struct MediaValuesCachedData {
        explicit MediaValuesCachedData(Document &) {}
    };
};
struct ViewportDescriptionWrapper;

#else

struct ViewportDescriptionWrapper {
  ViewportDescription description;
  bool set;
  ViewportDescriptionWrapper() : set(false) {}
};

#endif // BLINKIT_CRAWLER_ONLY

struct CORE_EXPORT CachedDocumentParameters {
  USING_FAST_MALLOC(CachedDocumentParameters);

 public:
  static std::unique_ptr<CachedDocumentParameters> Create(Document* document) {
    return base::WrapUnique(new CachedDocumentParameters(document));
  }

  static std::unique_ptr<CachedDocumentParameters> Create() {
    return base::WrapUnique(new CachedDocumentParameters);
  }

  bool do_html_preload_scanning;
#ifndef BLINKIT_CRAWLER_ONLY
  Length default_viewport_min_width;
  bool viewport_meta_zero_values_quirk;
  bool viewport_meta_enabled;
#endif
  ReferrerPolicy referrer_policy;

 private:
  explicit CachedDocumentParameters(Document*);
  CachedDocumentParameters() = default;
};

class TokenPreloadScanner {
  USING_FAST_MALLOC(TokenPreloadScanner);

 public:
  enum class ScannerType { kMainDocument, kInsertion };

  TokenPreloadScanner(const BlinKit::BkURL &document_url,
                      std::unique_ptr<CachedDocumentParameters>,
                      const MediaValuesCached::MediaValuesCachedData&,
                      const ScannerType);
  ~TokenPreloadScanner();

  void SetIsForCrawler(void) { for_crawler_ = true; }

  void Scan(const HTMLToken&,
            const SegmentedString&,
            PreloadRequestStream& requests,
            ViewportDescriptionWrapper*,
            bool* is_csp_meta_tag);
  void Scan(const CompactHTMLToken&,
            const SegmentedString&,
            PreloadRequestStream& requests,
            ViewportDescriptionWrapper*,
            bool* is_csp_meta_tag);

  void SetPredictedBaseElementURL(const BlinKit::BkURL& url) {
    predicted_base_element_url_ = url;
  }

  // A TokenPreloadScannerCheckpoint is valid until the next call to rewindTo,
  // at which point all outstanding checkpoints are invalidated.
  TokenPreloadScannerCheckpoint CreateCheckpoint();
  void RewindTo(TokenPreloadScannerCheckpoint);

 private:
  class StartTagScanner;

  template <typename Token>
  inline void ScanCommon(const Token&,
                         const SegmentedString&,
                         PreloadRequestStream& requests,
                         ViewportDescriptionWrapper*,
                         bool* is_csp_meta_tag);

  template <typename Token>
  void UpdatePredictedBaseURL(const Token&);

  struct Checkpoint {
    Checkpoint(const BlinKit::BkURL& predicted_base_element_url,
               bool in_style,
               bool in_script,
               size_t template_count)
        : predicted_base_element_url(predicted_base_element_url),
          in_style(in_style),
          in_script(in_script),
          template_count(template_count) {}

    BlinKit::BkURL predicted_base_element_url;
    bool in_style;
    bool in_script;
    size_t template_count;
  };

#ifdef BLINKIT_CRAWLER_ONLY
  struct PictureData { /* Just a placeholder */ };
#else
  struct PictureData {
    PictureData() : source_size(0.0), source_size_set(false), picked(false) {}
    String source_url;
    float source_size;
    bool source_size_set;
    bool picked;
  };

  CSSPreloadScanner css_scanner_;
#endif
  const BlinKit::BkURL document_url_;
  BlinKit::BkURL predicted_base_element_url_;
  bool in_style_;
  bool in_picture_;
  bool in_script_;
  bool for_crawler_ = false;
#ifndef BLINKIT_CRAWLER_ONLY
  PictureData picture_data_;
#endif
  size_t template_count_;
  std::unique_ptr<CachedDocumentParameters> document_parameters_;
#ifndef BLINKIT_CRAWLER_ONLY
  Persistent<MediaValuesCached> media_values_;
#endif
  ScannerType scanner_type_;

  bool did_rewind_ = false;

  Vector<Checkpoint> checkpoints_;

  DISALLOW_COPY_AND_ASSIGN(TokenPreloadScanner);
};

class CORE_EXPORT HTMLPreloadScanner {
  USING_FAST_MALLOC(HTMLPreloadScanner);

 public:
  static std::unique_ptr<HTMLPreloadScanner> Create(
      const HTMLParserOptions& options,
      const BlinKit::BkURL& document_url,
      std::unique_ptr<CachedDocumentParameters> document_parameters,
      const MediaValuesCached::MediaValuesCachedData& media_values_cached_data,
      const TokenPreloadScanner::ScannerType scanner_type) {
    return base::WrapUnique(new HTMLPreloadScanner(
        options, document_url, std::move(document_parameters),
        media_values_cached_data, scanner_type));
  }

  ~HTMLPreloadScanner();

  void AppendToEnd(const SegmentedString&);
  PreloadRequestStream Scan(const BlinKit::BkURL& document_base_element_url,
                            ViewportDescriptionWrapper*);

 private:
  HTMLPreloadScanner(const HTMLParserOptions&,
                     const BlinKit::BkURL& document_url,
                     std::unique_ptr<CachedDocumentParameters>,
                     const MediaValuesCached::MediaValuesCachedData&,
                     const TokenPreloadScanner::ScannerType);

  TokenPreloadScanner scanner_;
  SegmentedString source_;
  HTMLToken token_;
  std::unique_ptr<HTMLTokenizer> tokenizer_;

  DISALLOW_COPY_AND_ASSIGN(HTMLPreloadScanner);
};

}  // namespace blink

#endif
