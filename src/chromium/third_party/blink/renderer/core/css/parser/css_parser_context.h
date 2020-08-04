// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_parser_context.h
// Description: CSSParserContext Class
//      Author: Ziming Li
//     Created: 2020-03-27
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_CSS_PARSER_CSS_PARSER_CONTEXT_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_CSS_PARSER_CSS_PARSER_CONTEXT_H_

#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/core/css/parser/css_parser_mode.h"
#include "third_party/blink/renderer/core/execution_context/execution_context.h"
#include "third_party/blink/renderer/core/frame/web_feature_forward.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_loader_options.h"
#include "third_party/blink/renderer/platform/wtf/text/text_encoding.h"
#include "url/gurl.h"

namespace blink {

class CSSStyleSheet;
class Document;

class CORE_EXPORT CSSParserContext
    : public GarbageCollectedFinalized<CSSParserContext> {
 public:
  // https://drafts.csswg.org/selectors/#profiles
  enum SelectorProfile { kLiveProfile, kSnapshotProfile };

  static std::unique_ptr<CSSParserContext> Create(
      const Document&,
      const GURL& base_url_override,
      bool is_opaque_response_from_service_worker,
      const WTF::TextEncoding& charset = WTF::TextEncoding(),
      SelectorProfile = kLiveProfile);
  ~CSSParserContext(void)
  {
      int x = 0;
  }

  CSSParserMode Mode() const { return mode_; }
  CSSParserMode MatchMode() const { return match_mode_; }
  const GURL& BaseURL() const { return base_url_; }
  const WTF::TextEncoding& Charset() const { return charset_; }
  const std::string& GetReferrer() const { return referrer_; }
  bool IsHTMLDocument() const { return is_html_document_; }
  bool IsLiveProfile() const { return profile_ == kLiveProfile; }

 private:
  CSSParserContext(const GURL& base_url,
                   bool is_opaque_response_from_service_worker,
                   const WTF::TextEncoding& charset,
                   CSSParserMode,
                   CSSParserMode match_mode,
                   SelectorProfile,
                   const std::string&,
                   bool is_html_document,
                   bool use_legacy_background_size_shorthand_behavior,
                   SecureContextMode,
                   ContentSecurityPolicyDisposition,
                   const Document* use_counter_document);

  GURL base_url_;
  const bool is_opaque_response_from_service_worker_;
  WTF::TextEncoding charset_;
  CSSParserMode mode_;
  CSSParserMode match_mode_;
  SelectorProfile profile_ = kLiveProfile;
  std::string referrer_;
  bool is_html_document_;
  bool use_legacy_background_size_shorthand_behavior_;
  SecureContextMode secure_context_mode_;
  ContentSecurityPolicyDisposition should_check_content_security_policy_;

  WeakMember<const Document> document_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_CSS_PARSER_CSS_PARSER_CONTEXT_H_
