// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_parser_context.cc
// Description: CSSParserContext Class
//      Author: Ziming Li
//     Created: 2020-03-27
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/css/parser/css_parser_context.h"

#include "base/memory/ptr_util.h"
#include "third_party/blink/renderer/core/execution_context/execution_context.h"
#include "third_party/blink/renderer/core/dom/document.h"

namespace blink {

// static
std::unique_ptr<CSSParserContext> CSSParserContext::Create(
    const Document& document,
    const GURL& base_url_override,
    bool is_opaque_response_from_service_worker,
    const WTF::TextEncoding& charset,
    SelectorProfile profile) {
  CSSParserMode mode =
      document.InQuirksMode() ? kHTMLQuirksMode : kHTMLStandardMode;
  CSSParserMode match_mode = mode;

  const std::string referrer = base_url_override.GetAsReferrer().spec();

  bool use_legacy_background_size_shorthand_behavior = false;
#if 0 // BKTODO: Check below
      document.GetSettings()
          ? document.GetSettings()
                ->GetUseLegacyBackgroundSizeShorthandBehavior()
          : false;
#endif

  ContentSecurityPolicyDisposition policy_disposition = kDoNotCheckContentSecurityPolicy;

  return base::WrapUnique(new CSSParserContext(
      base_url_override, is_opaque_response_from_service_worker, charset, mode,
      match_mode, profile, referrer, document.IsHTMLDocument(),
      use_legacy_background_size_shorthand_behavior,
      SecureContextMode::kSecureContext,
      // BKTODO: document.GetSecureContextMode(),
      policy_disposition, &document));
}

CSSParserContext::CSSParserContext(
    const GURL& base_url,
    bool is_opaque_response_from_service_worker,
    const WTF::TextEncoding& charset,
    CSSParserMode mode,
    CSSParserMode match_mode,
    SelectorProfile profile,
    const std::string& referrer,
    bool is_html_document,
    bool use_legacy_background_size_shorthand_behavior,
    SecureContextMode secure_context_mode,
    ContentSecurityPolicyDisposition policy_disposition,
    const Document* use_counter_document)
    : base_url_(base_url),
      is_opaque_response_from_service_worker_(
          is_opaque_response_from_service_worker),
      charset_(charset),
      mode_(mode),
      match_mode_(match_mode),
      profile_(profile),
      referrer_(referrer),
      is_html_document_(is_html_document),
      use_legacy_background_size_shorthand_behavior_(
          use_legacy_background_size_shorthand_behavior),
      secure_context_mode_(secure_context_mode),
      should_check_content_security_policy_(policy_disposition),
      document_(use_counter_document) {}

}  // namespace blink
