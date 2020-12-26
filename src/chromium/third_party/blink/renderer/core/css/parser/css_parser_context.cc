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

#include "third_party/blink/renderer/core/execution_context/execution_context.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/frame/settings.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "third_party/blink/renderer/core/css/css_style_sheet.h"
#   include "third_party/blink/renderer/core/css/style_sheet_contents.h"
// BKTODO: #include "third_party/blink/renderer/core/html/imports/html_imports_controller.h"
#   include "third_party/blink/renderer/core/loader/document_loader.h"
#   include "third_party/blink/renderer/core/page/page.h"
#endif

namespace blink {

#ifndef BLINKIT_CRAWLER_ONLY
// static
CSSParserContext* CSSParserContext::Create(const ExecutionContext& context) {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  const Referrer referrer(context.Url().StrippedForUseAsReferrer(),
                          context.GetReferrerPolicy());

  ContentSecurityPolicyDisposition policy_disposition;
  if (ContentSecurityPolicy::ShouldBypassMainWorld(&context))
    policy_disposition = kDoNotCheckContentSecurityPolicy;
  else
    policy_disposition = kCheckContentSecurityPolicy;

  return new CSSParserContext(
      context.Url(), false /* is_opaque_response_from_service_worker */,
      WTF::TextEncoding(), kHTMLStandardMode, kHTMLStandardMode, kLiveProfile,
      referrer, true, false, context.GetSecureContextMode(), policy_disposition,
      DynamicTo<Document>(context));
#endif
}

// static
CSSParserContext* CSSParserContext::CreateWithStyleSheet(
    const CSSParserContext* other,
    const CSSStyleSheet* style_sheet) {
  return CSSParserContext::Create(
      other, CSSStyleSheet::SingleOwnerDocument(style_sheet));
}

// static
CSSParserContext* CSSParserContext::CreateWithStyleSheetContents(
    const CSSParserContext* other,
    const StyleSheetContents* style_sheet_contents) {
  return CSSParserContext::Create(
      other, StyleSheetContents::SingleOwnerDocument(style_sheet_contents));
}

// static
CSSParserContext* CSSParserContext::Create(
    const CSSParserContext* other,
    const Document* use_counter_document) {
  return new CSSParserContext(
      other->base_url_, other->is_opaque_response_from_service_worker_,
      other->charset_, other->mode_, other->match_mode_, other->profile_,
      other->referrer_, other->is_html_document_,
      other->use_legacy_background_size_shorthand_behavior_,
      other->secure_context_mode_, other->should_check_content_security_policy_,
      use_counter_document);
}

// static
CSSParserContext* CSSParserContext::Create(
    const CSSParserContext* other,
    const GURL& base_url,
    bool is_opaque_response_from_service_worker,
    const WTF::TextEncoding& charset,
    const Document* use_counter_document) {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  return new CSSParserContext(
      base_url, is_opaque_response_from_service_worker, charset, other->mode_,
      other->match_mode_, other->profile_,
      Referrer(base_url.StrippedForUseAsReferrer(), referrer_policy),
      other->is_html_document_,
      other->use_legacy_background_size_shorthand_behavior_,
      other->secure_context_mode_, other->should_check_content_security_policy_,
      use_counter_document);
#endif
}

// static
CSSParserContext* CSSParserContext::Create(
    CSSParserMode mode,
    SecureContextMode secure_context_mode,
    SelectorProfile profile,
    const Document* use_counter_document) {
  return new CSSParserContext(
      GURL(), false /* is_opaque_response_from_service_worker */,
      WTF::TextEncoding(), mode, mode, profile, std::string(), false, false,
      secure_context_mode, kDoNotCheckContentSecurityPolicy,
      use_counter_document);
}

// static
CSSParserContext* CSSParserContext::Create(const Document& document) {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  return CSSParserContext::Create(
      document, document.BaseURL(),
      false /* is_opaque_response_from_service_worker */,
      document.GetReferrerPolicy(), WTF::TextEncoding(), kLiveProfile);
#endif
}
#endif // BLINKIT_CRAWLER_ONLY

// static
CSSParserContext* CSSParserContext::Create(
    ObjectType gcType,
    const Document& document,
    const GURL& base_url_override,
    bool is_opaque_response_from_service_worker,
    const WTF::TextEncoding& charset,
    SelectorProfile profile) {
  CSSParserMode mode =
      document.InQuirksMode() ? kHTMLQuirksMode : kHTMLStandardMode;
#ifdef BLINKIT_CRAWLER_ONLY
  CSSParserMode match_mode = mode;
#else
  CSSParserMode match_mode;
  HTMLImportsController* imports_controller = document.ImportsController();
  if (imports_controller && profile == kLiveProfile) {
    ASSERT(false); // BKTODO:
#if 0
    match_mode = imports_controller->Master()->InQuirksMode()
                     ? kHTMLQuirksMode
                     : kHTMLStandardMode;
#else
    match_mode = mode;
#endif
  } else {
    match_mode = mode;
  }
#endif

  const std::string referrer = base_url_override.GetAsReferrer().spec();

  ContentSecurityPolicyDisposition policy_disposition = kDoNotCheckContentSecurityPolicy;

  return new (gcType) CSSParserContext(
      base_url_override, is_opaque_response_from_service_worker, charset, mode,
      match_mode, profile, referrer, document.IsHTMLDocument(),
      Settings::UseLegacyBackgroundSizeShorthandBehavior,
      SecureContextMode::kSecureContext,
      // BKTODO: document.GetSecureContextMode(),
      policy_disposition, &document);
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

#ifndef BLINKIT_CRAWLER_ONLY
bool CSSParserContext::operator==(const CSSParserContext& other) const {
  return base_url_ == other.base_url_ && charset_ == other.charset_ &&
         mode_ == other.mode_ && match_mode_ == other.match_mode_ &&
         profile_ == other.profile_ &&
         is_html_document_ == other.is_html_document_ &&
         use_legacy_background_size_shorthand_behavior_ ==
             other.use_legacy_background_size_shorthand_behavior_ &&
         secure_context_mode_ == other.secure_context_mode_;
}

const CSSParserContext* StrictCSSParserContext(
    SecureContextMode secure_context_mode) {
  static CSSParserContext* s_strict_context = nullptr;
  if (!s_strict_context)
    ASSERT(false); // BKTODO:
  return s_strict_context;
#if 0
  DEFINE_THREAD_SAFE_STATIC_LOCAL(ThreadSpecific<Persistent<CSSParserContext>>,
                                  strict_context_pool, ());
  DEFINE_THREAD_SAFE_STATIC_LOCAL(ThreadSpecific<Persistent<CSSParserContext>>,
                                  secure_strict_context_pool, ());

  Persistent<CSSParserContext>& context =
      secure_context_mode == SecureContextMode::kSecureContext
          ? *secure_strict_context_pool
          : *strict_context_pool;
  if (!context) {
    context = CSSParserContext::Create(kHTMLStandardMode, secure_context_mode);
    context.RegisterAsStaticReference();
  }

  return context;
#endif
}

bool CSSParserContext::IsOpaqueResponseFromServiceWorker() const {
  return is_opaque_response_from_service_worker_;
}

bool CSSParserContext::IsSecureContext() const {
  return secure_context_mode_ == SecureContextMode::kSecureContext;
}

GURL CSSParserContext::CompleteURL(const String& url) const {
  if (url.IsNull())
    return GURL();
  if (!Charset().IsValid())
    return BaseURL().Resolve(url.StdUtf8());
  ASSERT(false); // BKTODO:
  return BaseURL().Resolve(url.StdUtf8());
#if 0
  return GURL(BaseURL(), url, Charset());
#endif
}
#endif // BLINKIT_CRAWLER_ONLY

void CSSParserContext::Count(WebFeature) const {
  // Nothing to do, just a placeholder.
}

#ifndef BLINKIT_CRAWLER_ONLY
void CSSParserContext::CountDeprecation(WebFeature feature) const {
  // Nothing to do, just a placeholder.
}

void CSSParserContext::Count(CSSParserMode mode, CSSPropertyID property) const {
  // Nothing to do, just a placeholder.
}

bool CSSParserContext::IsDocumentHandleEqual(const Document* other) const {
  return document_.Get() == other;
}
#endif // BLINKIT_CRAWLER_ONLY

}  // namespace blink
