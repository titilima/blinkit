// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_style_sheet_resource.cc
// Description: CSSStyleSheetResource Class
//      Author: Ziming Li
//     Created: 2020-10-04
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
    Copyright (C) 1998 Lars Knoll (knoll@mpi-hd.mpg.de)
    Copyright (C) 2001 Dirk Mueller (mueller@kde.org)
    Copyright (C) 2002 Waldo Bastian (bastian@kde.org)
    Copyright (C) 2006 Samuel Weinig (sam.weinig@gmail.com)
    Copyright (C) 2004, 2005, 2006 Apple Computer, Inc.

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

    This class provides all functionality needed for loading images, style
    sheets and html pages from the web. It has a memory cache for these objects.
*/

#include "third_party/blink/renderer/core/loader/resource/css_style_sheet_resource.h"

// BKTODO: #include "services/network/public/mojom/request_context_frame_type.mojom-blink.h"
#include "third_party/blink/renderer/core/css/style_sheet_contents.h"
#include "third_party/blink/renderer/core/frame/web_feature.h"
#include "third_party/blink/renderer/platform/loader/fetch/fetch_parameters.h"
// BKTODO: #include "third_party/blink/renderer/platform/loader/fetch/memory_cache.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_fetcher.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_loader_options.h"
#include "third_party/blink/renderer/platform/loader/fetch/text_resource_decoder_options.h"
#include "third_party/blink/renderer/platform/network/http_names.h"
#include "third_party/blink/renderer/platform/network/mime/mime_type_registry.h"
#include "third_party/blink/renderer/platform/runtime_enabled_features.h"
#include "third_party/blink/renderer/platform/weborigin/security_policy.h"
#include "third_party/blink/renderer/platform/wtf/text/text_encoding.h"

namespace blink {

CSSStyleSheetResource* CSSStyleSheetResource::Fetch(FetchParameters& params,
                                                    ResourceFetcher* fetcher,
                                                    ResourceClient* client) {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  DCHECK_EQ(params.GetResourceRequest().GetFrameType(),
            network::mojom::RequestContextFrameType::kNone);
  params.SetRequestContext(mojom::RequestContextType::STYLE);
  CSSStyleSheetResource* resource = ToCSSStyleSheetResource(
      fetcher->RequestResource(params, CSSStyleSheetResourceFactory(), client));
  return resource;
#endif
}

CSSStyleSheetResource* CSSStyleSheetResource::CreateForTest(
    const GURL& url,
    const WTF::TextEncoding& encoding) {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  ResourceRequest request(url);
  request.SetFetchCredentialsMode(network::mojom::FetchCredentialsMode::kOmit);
  ResourceLoaderOptions options;
  TextResourceDecoderOptions decoder_options(
      TextResourceDecoderOptions::kCSSContent, encoding);
  return new CSSStyleSheetResource(request, options, decoder_options);
#endif
}

CSSStyleSheetResource::CSSStyleSheetResource(
    const ResourceRequest& resource_request,
    const ResourceLoaderOptions& options,
    const TextResourceDecoderOptions& decoder_options)
    : TextResource(resource_request,
                   ResourceType::kCSSStyleSheet,
                   options,
                   decoder_options) {}

CSSStyleSheetResource::~CSSStyleSheetResource() = default;

void CSSStyleSheetResource::SetParsedStyleSheetCache(
    StyleSheetContents* new_sheet) {
  if (parsed_style_sheet_cache_)
    parsed_style_sheet_cache_->ClearReferencedFromResource();
  parsed_style_sheet_cache_ = new_sheet;
  if (parsed_style_sheet_cache_)
    parsed_style_sheet_cache_->SetReferencedFromResource(this);

  // Updates the decoded size to take parsed stylesheet cache into account.
  UpdateDecodedSize();
}

#if 0 // BKTODO:
ReferrerPolicy CSSStyleSheetResource::GetReferrerPolicy() const {
  ReferrerPolicy referrer_policy = kReferrerPolicyDefault;
  String referrer_policy_header =
      GetResponse().HttpHeaderField(HTTPNames::Referrer_Policy);
  if (!referrer_policy_header.IsNull()) {
    SecurityPolicy::ReferrerPolicyFromHeaderValue(
        referrer_policy_header, kDoNotSupportReferrerPolicyLegacyKeywords,
        &referrer_policy);
  }
  return referrer_policy;
}
#endif

const String CSSStyleSheetResource::SheetText(
    const CSSParserContext* parser_context,
    MIMETypeCheck mime_type_check) const {
  if (!CanUseSheet(parser_context, mime_type_check))
    return String();

  // Use cached decoded sheet text when available
  if (!decoded_sheet_text_.IsNull()) {
    // We should have the decoded sheet text cached when the resource is fully
    // loaded.
    ASSERT(false); // BKTODO: DCHECK_EQ(GetStatus(), ResourceStatus::kCached);

    return decoded_sheet_text_;
  }

  ASSERT(false); // BKTODO:
#if 0
  if (!Data() || Data()->IsEmpty())
    return String();
#endif

  return DecodedText();
}

void CSSStyleSheetResource::NotifyFinished() {
  // Decode the data to find out the encoding and cache the decoded sheet text.
  if (Data())
    SetDecodedSheetText(DecodedText());

  Resource::NotifyFinished();

  // Clear raw bytes as now we have the full decoded sheet text.
  // We wait for all LinkStyle::setCSSStyleSheet to run (at least once)
  // as SubresourceIntegrity checks require raw bytes.
  // Note that LinkStyle::setCSSStyleSheet can be called from didAddClient too,
  // but is safe as we should have a cached ResourceIntegrityDisposition.
  ClearData();
}

#if 0 // BKTODO:
void CSSStyleSheetResource::DestroyDecodedDataIfPossible() {
  if (!parsed_style_sheet_cache_)
    return;

  SetParsedStyleSheetCache(nullptr);
}

void CSSStyleSheetResource::DestroyDecodedDataForFailedRevalidation() {
  SetDecodedSheetText(String());
  DestroyDecodedDataIfPossible();
}
#endif

bool CSSStyleSheetResource::CanUseSheet(const CSSParserContext* parser_context,
                                        MIMETypeCheck mime_type_check) const {
  if (ErrorOccurred())
    return false;

  // For `file:` URLs, we may need to be a little more strict than the below.
  // Though we'll likely change this in the future, for the moment we're going
  // to enforce a file-extension requirement on stylesheets loaded from `file:`
  // URLs and see how far it gets us.
  GURL sheet_url = GetResponse().Url();
  if (sheet_url.SchemeIsFile()) {
    if (parser_context) {
      parser_context->Count(WebFeature::kLocalCSSFile);
    }
    // Grab |sheet_url|'s filename's extension (if present), and check whether
    // or not it maps to a `text/css` MIME type:
    String extension;
    std::string fileName = sheet_url.ExtractFileName();
    size_t last_dot = fileName.rfind('.');
    if (std::string::npos != last_dot)
      extension = String::FromStdUTF8(fileName.substr(last_dot + 1));
    if (!EqualIgnoringASCIICase(
            MIMETypeRegistry::GetMIMETypeForExtension(extension), "text/css")) {
      if (parser_context) {
        parser_context->CountDeprecation(
            WebFeature::kLocalCSSFileExtensionRejected);
      }
      if (RuntimeEnabledFeatures::RequireCSSExtensionForFileEnabled()) {
        return false;
      }
    }
  }

  // This check exactly matches Firefox. Note that we grab the Content-Type
  // header directly because we want to see what the value is BEFORE content
  // sniffing. Firefox does this by setting a "type hint" on the channel. This
  // implementation should be observationally equivalent.
  //
  // This code defaults to allowing the stylesheet for non-HTTP protocols so
  // folks can use standards mode for local HTML documents.
  if (mime_type_check == MIMETypeCheck::kLax)
    return true;
  ASSERT(false); // BKTODO:
  return false;
#if 0
  AtomicString content_type = HttpContentType();
  return content_type.IsEmpty() ||
         DeprecatedEqualIgnoringCase(content_type, "text/css") ||
         DeprecatedEqualIgnoringCase(content_type,
                                     "application/x-unknown-content-type");
#endif
}

StyleSheetContents* CSSStyleSheetResource::CreateParsedStyleSheetFromCache(
    const CSSParserContext* context) {
  if (!parsed_style_sheet_cache_)
    return nullptr;
  if (parsed_style_sheet_cache_->HasFailedOrCanceledSubresources()) {
    SetParsedStyleSheetCache(nullptr);
    return nullptr;
  }

  DCHECK(parsed_style_sheet_cache_->IsCacheableForResource());
  DCHECK(parsed_style_sheet_cache_->IsReferencedFromResource());

  // Contexts must be identical so we know we would get the same exact result if
  // we parsed again.
  if (*parsed_style_sheet_cache_->ParserContext() != *context)
    return nullptr;

  DCHECK(!parsed_style_sheet_cache_->IsLoading());

  // If the stylesheet has a media query, we need to clone the cached sheet
  // due to potential differences in the rule set.
  if (RuntimeEnabledFeatures::CacheStyleSheetWithMediaQueriesEnabled() &&
      parsed_style_sheet_cache_->HasMediaQueries()) {
    ASSERT(false); // BKTODO: return parsed_style_sheet_cache_->Copy();
  }

  return parsed_style_sheet_cache_;
}

void CSSStyleSheetResource::SaveParsedStyleSheet(StyleSheetContents* sheet) {
  DCHECK(sheet);
  DCHECK(sheet->IsCacheableForResource());

  ASSERT(false); // BKTODO:
#if 0
  if (!GetMemoryCache()->Contains(this)) {
    // This stylesheet resource did conflict with another resource and was not
    // added to the cache.
    SetParsedStyleSheetCache(nullptr);
    return;
  }
#endif
  SetParsedStyleSheetCache(sheet);
}

void CSSStyleSheetResource::SetDecodedSheetText(
    const String& decoded_sheet_text) {
  decoded_sheet_text_ = decoded_sheet_text;
  UpdateDecodedSize();
}

void CSSStyleSheetResource::UpdateDecodedSize() {
  size_t decoded_size = decoded_sheet_text_.CharactersSizeInBytes();
  if (parsed_style_sheet_cache_)
    decoded_size += parsed_style_sheet_cache_->EstimatedSizeInBytes();
  ASSERT(false); // BKTODO: SetDecodedSize(decoded_size);
}

}  // namespace blink
