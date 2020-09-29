// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_preload_scanner.cc
// Description: HTMLPreloadScanner Class
//      Author: Ziming Li
//     Created: 2019-10-18
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2008 Apple Inc. All Rights Reserved.
 * Copyright (C) 2009 Torch Mobile, Inc. http://www.torchmobile.com/
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

#include "third_party/blink/renderer/core/html/parser/html_preload_scanner.h"

#include <memory>
#include <optional>
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/html/parser/html_parser_idioms.h"
#include "third_party/blink/renderer/core/html/parser/html_tokenizer.h"
#include "third_party/blink/renderer/core/html_names.h"
#include "third_party/blink/renderer/core/script/script_loader.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource.h"
#include "third_party/blink/renderer/platform/network/mime/content_type.h"
#include "third_party/blink/renderer/platform/network/mime/mime_type_registry.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "third_party/blink/renderer/core/css/media_list.h"
#   include "third_party/blink/renderer/core/css/media_query_evaluator.h"
#   include "third_party/blink/renderer/core/css/media_values_cached.h"
#   include "third_party/blink/renderer/core/css/parser/sizes_attribute_parser.h"
#   include "third_party/blink/renderer/core/frame/viewport_data.h"
#if 0 // BKTODO:
#   include "third_party/blink/renderer/core/html/html_dimension.h"
#   include "third_party/blink/renderer/core/html/html_image_element.h"
#   include "third_party/blink/renderer/core/html/html_meta_element.h"
#   include "third_party/blink/renderer/core/html/link_rel_attribute.h"
#   include "third_party/blink/renderer/core/html/parser/html_srcset_parser.h"
#endif
#   include "third_party/blink/renderer/core/input_type_names.h"
// BKTODO: #include "third_party/blink/renderer/core/loader/link_loader.h"
#endif

namespace blink {

using namespace html_names;

static bool Match(const StringImpl* impl, const QualifiedName& q_name) {
  return impl == q_name.LocalName().Impl();
}

static bool Match(const AtomicString& name, const QualifiedName& q_name) {
  DCHECK(IsMainThread());
  return q_name.LocalName() == name;
}

static bool Match(const String& name, const QualifiedName& q_name) {
  return ThreadSafeMatch(name, q_name);
}

static const StringImpl* TagImplFor(const HTMLToken::DataVector& data) {
  AtomicString tag_name(data);
  const StringImpl* result = tag_name.Impl();
  if (result->IsStatic())
    return result;
  return nullptr;
}

static const StringImpl* TagImplFor(const String& tag_name) {
  const StringImpl* result = tag_name.Impl();
  if (result->IsStatic())
    return result;
  return nullptr;
}

static String InitiatorFor(const StringImpl* tag_impl) {
  DCHECK(tag_impl);
  if (Match(tag_impl, kImgTag))
    return kImgTag.LocalName();
  if (Match(tag_impl, kInputTag))
    return kInputTag.LocalName();
  if (Match(tag_impl, kLinkTag))
    return kLinkTag.LocalName();
  if (Match(tag_impl, kScriptTag))
    return kScriptTag.LocalName();
  if (Match(tag_impl, kVideoTag))
    return kVideoTag.LocalName();
  NOTREACHED();
  return g_empty_string;
}

#ifndef BLINKIT_CRAWLER_ONLY
static bool MediaAttributeMatches(const MediaValuesCached& media_values,
                                  const String& attribute_value) {
  scoped_refptr<MediaQuerySet> media_queries =
      MediaQuerySet::Create(attribute_value);
  MediaQueryEvaluator media_query_evaluator(media_values);
  return media_query_evaluator.Eval(*media_queries);
}

static bool IsDimensionSmallAndAbsoluteForLazyLoad(
    const String& attribute_value) {
  // Minimum height or width of the image to start lazyloading.
  const unsigned kMinDimensionToLazyLoad = 10;
  ASSERT(false); // BKTODO:
  return false;
#if 0
  HTMLDimension dimension;
  return ParseDimensionValue(attribute_value, dimension) &&
         dimension.IsAbsolute() && dimension.Value() <= kMinDimensionToLazyLoad;
#endif
}
#endif // BLINKIT_CRAWLER_ONLY

class TokenPreloadScanner::StartTagScanner {
  STACK_ALLOCATED();

 public:
  StartTagScanner(const StringImpl* tag_impl,
                  MediaValuesCached* media_values,
                  TokenPreloadScanner::ScannerType scanner_type)
      : tag_impl_(tag_impl),
#ifndef BLINKIT_CRAWLER_ONLY
        link_is_style_sheet_(false),
        link_is_preconnect_(false),
        link_is_preload_(false),
        link_is_modulepreload_(false),
        link_is_import_(false),
#endif
        matched_(true),
#ifndef BLINKIT_CRAWLER_ONLY
        input_is_image_(false),
#endif
        nomodule_attribute_value_(false),
#ifndef BLINKIT_CRAWLER_ONLY
        source_size_(0),
        source_size_set_(false),
        media_values_(media_values),
#endif
        referrer_policy_set_(false),
        referrer_policy_(kReferrerPolicyDefault),
#ifndef BLINKIT_CRAWLER_ONLY
        // BKTODO: lazyload_attr_set_to_off_(false),
        width_attr_small_absolute_(false),
        height_attr_small_absolute_(false),
#endif
        scanner_type_(scanner_type) {
#ifdef BLINKIT_CRAWLER_ONLY
    ASSERT(nullptr == media_values);
    if (!Match(tag_impl_, kScriptTag))
      tag_impl_ = nullptr;
#else
    if (Match(tag_impl_, kImgTag) || Match(tag_impl_, kSourceTag)) {
      source_size_ = SizesAttributeParser(media_values_, String()).length();
      return;
    }
    if (!Match(tag_impl_, kInputTag) && !Match(tag_impl_, kLinkTag) && !Match(tag_impl_, kScriptTag))
      tag_impl_ = nullptr;
#endif
  }

  enum URLReplacement { kAllowURLReplacement, kDisallowURLReplacement };

  void ProcessAttributes(const HTMLToken::AttributeList& attributes) {
    DCHECK(IsMainThread());
    if (!tag_impl_)
      return;
    for (const HTMLToken::Attribute& html_token_attribute : attributes) {
      AtomicString attribute_name(html_token_attribute.GetName());
      String attribute_value = html_token_attribute.Value8BitIfNecessary();
      ProcessAttribute(attribute_name, attribute_value);
    }
    PostProcessAfterAttributes();
  }

  void ProcessAttributes(
      const Vector<CompactHTMLToken::Attribute>& attributes) {
    if (!tag_impl_)
      return;
    for (const CompactHTMLToken::Attribute& html_token_attribute : attributes)
      ProcessAttribute(html_token_attribute.GetName(),
                       html_token_attribute.Value());
    PostProcessAfterAttributes();
  }

  void PostProcessAfterAttributes() {
#ifndef BLINKIT_CRAWLER_ONLY
    ASSERT(false); // BKTODO:
#if 0
    if (Match(tag_impl_, kImgTag) ||
        (link_is_preload_ && as_attribute_value_ == "image" &&
         RuntimeEnabledFeatures::PreloadImageSrcSetEnabled()))
      SetUrlFromImageAttributes();
#endif
#endif
  }

#ifndef BLINKIT_CRAWLER_ONLY
  void HandlePictureSourceURL(PictureData& picture_data) {
    if (Match(tag_impl_, kSourceTag) && matched_ &&
        picture_data.source_url.IsEmpty()) {
      ASSERT(false); // BKTODO:
#if 0
      // Must create an IsolatedCopy() since the srcset attribute value will get
      // sent back to the main thread between when we set this, and when we
      // process the closing tag which would clear picture_data_. Having any ref
      // to a string we're going to send will fail
      // IsSafeToSendToAnotherThread().
      picture_data.source_url =
          srcset_image_candidate_.ToString().IsolatedCopy();
      picture_data.source_size_set = source_size_set_;
      picture_data.source_size = source_size_;
      picture_data.picked = true;
#endif
    } else if (Match(tag_impl_, kImgTag) && !picture_data.source_url.IsEmpty()) {
      SetUrlToLoad(picture_data.source_url, kAllowURLReplacement);
    }
  }
#endif

  std::unique_ptr<PreloadRequest> CreatePreloadRequest(
      const GURL& predicted_base_url,
      const SegmentedString& source,
      const PictureData& picture_data,
      const ReferrerPolicy document_referrer_policy) {
    PreloadRequest::RequestType request_type =
        PreloadRequest::kRequestTypePreload;
    std::optional<ResourceType> type;
#ifdef BLINKIT_CRAWLER_ONLY
    ASSERT(false); // BKTODO:
#else
    if (ShouldPreconnect()) {
      request_type = PreloadRequest::kRequestTypePreconnect;
    } else {
      if (IsLinkRelPreload()) {
        request_type = PreloadRequest::kRequestTypeLinkRelPreload;
        type = ResourceTypeForLinkPreload();
        if (type == base::nullopt)
          return nullptr;
      } else if (IsLinkRelModulePreload()) {
        request_type = PreloadRequest::kRequestTypeLinkRelPreload;
        type = ResourceType::kScript;
      }
      if (!ShouldPreload(type)) {
        return nullptr;
      }
    }
#endif

    ASSERT(false); // BKTODO:
    return nullptr;
#if 0
    TextPosition position =
        TextPosition(source.CurrentLine(), source.CurrentColumn());
    FetchParameters::ResourceWidth resource_width;
    float source_size = source_size_;
    bool source_size_set = source_size_set_;
    if (picture_data.picked) {
      source_size_set = picture_data.source_size_set;
      source_size = picture_data.source_size;
    }
    ResourceFetcher::IsImageSet is_image_set =
        (picture_data.picked || !srcset_image_candidate_.IsEmpty())
            ? ResourceFetcher::kImageIsImageSet
            : ResourceFetcher::kImageNotImageSet;

    if (source_size_set) {
      resource_width.width = source_size;
      resource_width.is_set = true;
    }

    if (type == base::nullopt)
      type = GetResourceType();

    // The element's 'referrerpolicy' attribute (if present) takes precedence
    // over the document's referrer policy.
    ReferrerPolicy referrer_policy =
        (referrer_policy_ != kReferrerPolicyDefault) ? referrer_policy_
                                                     : document_referrer_policy;
    auto request = PreloadRequest::CreateIfNeeded(
        InitiatorFor(tag_impl_), position, url_to_load_, predicted_base_url,
        type.value(), referrer_policy, PreloadRequest::kDocumentIsReferrer,
        is_image_set, resource_width, client_hints_preferences, request_type);
    if (!request)
      return nullptr;

    if ((Match(tag_impl_, scriptTag) && type_attribute_value_ == "module") ||
        IsLinkRelModulePreload()) {
      request->SetScriptType(ScriptType::kModule);
    }

    request->SetCrossOrigin(cross_origin_);
    request->SetImportance(importance_);
    request->SetNonce(nonce_);
    request->SetCharset(Charset());
    request->SetDefer(defer_);

    if (lazyload_attr_set_to_off_ ||
        (width_attr_small_absolute_ && height_attr_small_absolute_)) {
      request->SetIsLazyloadImageDisabled(true);
    }

    // The only link tags that should keep the integrity metadata are
    // stylesheets until crbug.com/677022 is resolved.
    if (link_is_style_sheet_ || !Match(tag_impl_, linkTag))
      request->SetIntegrityMetadata(integrity_metadata_);

    if (scanner_type_ == ScannerType::kInsertion)
      request->SetFromInsertionScanner(true);

    return request;
#endif
  }

 private:
  template <typename NameType>
  void ProcessScriptAttribute(const NameType& attribute_name,
                              const String& attribute_value) {
    // FIXME - Don't set crossorigin multiple times.
    if (Match(attribute_name, kSrcAttr)) {
      SetUrlToLoad(attribute_value, kDisallowURLReplacement);
    } else if (Match(attribute_name, kTypeAttr)) {
      type_attribute_value_ = attribute_value;
    } else if (Match(attribute_name, kLanguageAttr)) {
      language_attribute_value_ = attribute_value;
    } else if (Match(attribute_name, kNomoduleAttr)) {
      nomodule_attribute_value_ = true;
    } else if (!referrer_policy_set_ &&
               Match(attribute_name, kReferrerpolicyAttr) &&
               !attribute_value.IsNull()) {
      SetReferrerPolicy(attribute_value,
                        kDoNotSupportReferrerPolicyLegacyKeywords);
    }
  }

#ifndef BLINKIT_CRAWLER_ONLY
  template <typename NameType>
  void ProcessImgAttribute(const NameType& attribute_name,
                           const String& attribute_value) {
    ASSERT(false); // BKTODO:
#if 0
    if (Match(attribute_name, kSrcAttr) && img_src_url_.IsNull()) {
      img_src_url_ = attribute_value;
    } else if (Match(attribute_name, crossoriginAttr)) {
      SetCrossOrigin(attribute_value);
    } else if (Match(attribute_name, srcsetAttr) &&
               srcset_attribute_value_.IsNull()) {
      srcset_attribute_value_ = attribute_value;
    } else if (Match(attribute_name, sizesAttr) && !source_size_set_) {
      ParseSourceSize(attribute_value);
    } else if (!referrer_policy_set_ &&
               Match(attribute_name, referrerpolicyAttr) &&
               !attribute_value.IsNull()) {
      SetReferrerPolicy(attribute_value, kSupportReferrerPolicyLegacyKeywords);
    } else if (!importance_mode_set_ && Match(attribute_name, importanceAttr) &&
               RuntimeEnabledFeatures::PriorityHintsEnabled()) {
      SetImportance(attribute_value);
    } else if (!lazyload_attr_set_to_off_ &&
               Match(attribute_name, lazyloadAttr) &&
               RuntimeEnabledFeatures::LazyImageLoadingEnabled() &&
               EqualIgnoringASCIICase(attribute_value, "off")) {
      lazyload_attr_set_to_off_ = true;
    } else if (!width_attr_small_absolute_ &&
               Match(attribute_name, widthAttr) &&
               RuntimeEnabledFeatures::LazyImageLoadingEnabled()) {
      width_attr_small_absolute_ =
          IsDimensionSmallAndAbsoluteForLazyLoad(attribute_value);
    } else if (!height_attr_small_absolute_ &&
               Match(attribute_name, heightAttr) &&
               RuntimeEnabledFeatures::LazyImageLoadingEnabled()) {
      height_attr_small_absolute_ =
          IsDimensionSmallAndAbsoluteForLazyLoad(attribute_value);
    }
#endif
  }

  void SetUrlFromImageAttributes() {
    ASSERT(false); // BKTODO:
#if 0
    srcset_image_candidate_ =
        BestFitSourceForSrcsetAttribute(media_values_->DevicePixelRatio(),
                                        source_size_, srcset_attribute_value_);
    SetUrlToLoad(BestFitSourceForImageAttributes(
                     media_values_->DevicePixelRatio(), source_size_,
                     img_src_url_, srcset_image_candidate_),
                 kAllowURLReplacement);
#endif
  }

  template <typename NameType>
  void ProcessLinkAttribute(const NameType& attribute_name,
                            const String& attribute_value) {
    ASSERT(false); // BKTODO:
#if 0
    // FIXME - Don't set rel/media/crossorigin multiple times.
    if (Match(attribute_name, kHrefAttr)) {
      SetUrlToLoad(attribute_value, kDisallowURLReplacement);
      // Used in SetUrlFromImageAttributes() when as=image.
      img_src_url_ = attribute_value;
    } else if (Match(attribute_name, kRelAttr)) {
      LinkRelAttribute rel(attribute_value);
      link_is_style_sheet_ = rel.IsStyleSheet() && !rel.IsAlternate() &&
                             rel.GetIconType() == kInvalidIcon &&
                             !rel.IsDNSPrefetch();
      link_is_preconnect_ = rel.IsPreconnect();
      link_is_preload_ = rel.IsLinkPreload();
      link_is_modulepreload_ = rel.IsModulePreload();
      link_is_import_ = rel.IsImport();
    } else if (Match(attribute_name, mediaAttr)) {
      matched_ &= MediaAttributeMatches(*media_values_, attribute_value);
    } else if (Match(attribute_name, crossoriginAttr)) {
      SetCrossOrigin(attribute_value);
    } else if (Match(attribute_name, nonceAttr)) {
      SetNonce(attribute_value);
    } else if (Match(attribute_name, asAttr)) {
      as_attribute_value_ = attribute_value.DeprecatedLower();
    } else if (Match(attribute_name, typeAttr)) {
      type_attribute_value_ = attribute_value;
    } else if (!referrer_policy_set_ &&
               Match(attribute_name, referrerpolicyAttr) &&
               !attribute_value.IsNull()) {
      SetReferrerPolicy(attribute_value,
                        kDoNotSupportReferrerPolicyLegacyKeywords);
    } else if (!integrity_attr_set_ && Match(attribute_name, integrityAttr)) {
      integrity_attr_set_ = true;
      SubresourceIntegrity::ParseIntegrityAttribute(
          attribute_value, integrity_features_, integrity_metadata_);
    } else if (Match(attribute_name, srcsetAttr) &&
               srcset_attribute_value_.IsNull()) {
      srcset_attribute_value_ = attribute_value;
    } else if (Match(attribute_name, imgsizesAttr) && !source_size_set_) {
      ParseSourceSize(attribute_value);
    } else if (!importance_mode_set_ && Match(attribute_name, importanceAttr) &&
               RuntimeEnabledFeatures::PriorityHintsEnabled()) {
      SetImportance(attribute_value);
    }
#endif
  }

  template <typename NameType>
  void ProcessInputAttribute(const NameType& attribute_name,
                             const String& attribute_value) {
    // FIXME - Don't set type multiple times.
    if (Match(attribute_name, kSrcAttr)) {
      SetUrlToLoad(attribute_value, kDisallowURLReplacement);
    } else if (Match(attribute_name, kTypeAttr)) {
      ASSERT(false); // BKTODO:
#if 0
      input_is_image_ =
          DeprecatedEqualIgnoringCase(attribute_value, InputTypeNames::image);
#endif
    }
  }

  template <typename NameType>
  void ProcessSourceAttribute(const NameType& attribute_name,
                              const String& attribute_value) {
    ASSERT(false); // BKTODO:
#if 0
    if (Match(attribute_name, srcsetAttr) &&
        srcset_image_candidate_.IsEmpty()) {
      srcset_attribute_value_ = attribute_value;
      srcset_image_candidate_ = BestFitSourceForSrcsetAttribute(
          media_values_->DevicePixelRatio(), source_size_, attribute_value);
    } else if (Match(attribute_name, sizesAttr) && !source_size_set_) {
      ParseSourceSize(attribute_value);
      if (!srcset_image_candidate_.IsEmpty()) {
        srcset_image_candidate_ = BestFitSourceForSrcsetAttribute(
            media_values_->DevicePixelRatio(), source_size_,
            srcset_attribute_value_);
      }
    } else if (Match(attribute_name, mediaAttr)) {
      // FIXME - Don't match media multiple times.
      matched_ &= MediaAttributeMatches(*media_values_, attribute_value);
    } else if (Match(attribute_name, typeAttr)) {
      matched_ &= MIMETypeRegistry::IsSupportedImagePrefixedMIMEType(
          ContentType(attribute_value).GetType());
    }
#endif
  }
#endif // BLINKIT_CRAWLER_ONLY

  template <typename NameType>
  void ProcessAttribute(const NameType& attribute_name,
                        const String& attribute_value) {
    if (Match(attribute_name, kCharsetAttr))
      charset_ = attribute_value;

    if (Match(tag_impl_, kScriptTag))
      ProcessScriptAttribute(attribute_name, attribute_value);
#ifndef BLINKIT_CRAWLER_ONLY
    else if (Match(tag_impl_, kImgTag))
      ProcessImgAttribute(attribute_name, attribute_value);
    else if (Match(tag_impl_, kLinkTag))
      ProcessLinkAttribute(attribute_name, attribute_value);
    else if (Match(tag_impl_, kInputTag))
      ProcessInputAttribute(attribute_name, attribute_value);
    else if (Match(tag_impl_, kSourceTag))
      ProcessSourceAttribute(attribute_name, attribute_value);
#endif
  }

  void SetUrlToLoad(const String& value, URLReplacement replacement) {
    // We only respect the first src/href, per HTML5:
    // http://www.whatwg.org/specs/web-apps/current-work/multipage/tokenization.html#attribute-name-state
    if (replacement == kDisallowURLReplacement && !url_to_load_.IsEmpty())
      return;
    String url = StripLeadingAndTrailingHTMLSpaces(value);
    if (url.IsEmpty())
      return;
    url_to_load_ = url;
  }

  const String& Charset() const {
#ifndef BLINKIT_CRAWLER_ONLY
    // FIXME: Its not clear that this if is needed, the loader probably ignores
    // charset for image requests anyway.
    if (Match(tag_impl_, kImgTag))
      return g_empty_string;
#endif
    return charset_;
  }

#ifndef BLINKIT_CRAWLER_ONLY
  std::optional<ResourceType> ResourceTypeForLinkPreload() const {
    DCHECK(link_is_preload_);
    ASSERT(false); // BKTODO:
    return std::nullopt;
#if 0
    return LinkLoader::GetResourceTypeFromAsAttribute(as_attribute_value_);
#endif
  }
#endif

  ResourceType GetResourceType() const {
    if (Match(tag_impl_, kScriptTag))
      return ResourceType::kScript;
#ifndef BLINKIT_CRAWLER_ONLY
    if (Match(tag_impl_, kImgTag) || (Match(tag_impl_, kInputTag) && input_is_image_))
      return ResourceType::kImage;
    if (Match(tag_impl_, kLinkTag) && link_is_style_sheet_)
      return ResourceType::kCSSStyleSheet;
    if (link_is_preconnect_)
      return ResourceType::kRaw;
    if (Match(tag_impl_, kLinkTag) && link_is_import_)
      return ResourceType::kImportResource;
#endif
    NOTREACHED();
    return ResourceType::kRaw;
  }

#ifndef BLINKIT_CRAWLER_ONLY
  bool ShouldPreconnect() const {
    return Match(tag_impl_, kLinkTag) && link_is_preconnect_ &&
           !url_to_load_.IsEmpty();
  }

  bool IsLinkRelPreload() const {
    return Match(tag_impl_, kLinkTag) && link_is_preload_ &&
           !url_to_load_.IsEmpty();
  }

  bool IsLinkRelModulePreload() const {
    return Match(tag_impl_, kLinkTag) && link_is_modulepreload_ &&
           !url_to_load_.IsEmpty();
  }

  bool ShouldPreloadLink(std::optional<ResourceType>& type) const {
    if (link_is_style_sheet_) {
      return type_attribute_value_.IsEmpty() ||
             MIMETypeRegistry::IsSupportedStyleSheetMIMEType(
                 ContentType(type_attribute_value_).GetType());
    } else if (link_is_preload_) {
      if (type_attribute_value_.IsEmpty())
        return true;
      String type_from_attribute = ContentType(type_attribute_value_).GetType();
      if ((type == ResourceType::kFont &&
           !MIMETypeRegistry::IsSupportedFontMIMEType(type_from_attribute)) ||
          (type == ResourceType::kImage &&
           !MIMETypeRegistry::IsSupportedImagePrefixedMIMEType(
               type_from_attribute)) ||
          (type == ResourceType::kCSSStyleSheet &&
           !MIMETypeRegistry::IsSupportedStyleSheetMIMEType(
               type_from_attribute))) {
        return false;
      }
    } else if (link_is_modulepreload_) {
      return true;
    } else if (!link_is_import_) {
      return false;
    }

    return true;
  }
#endif // BLINKIT_CRAWLER_ONLY

  bool ShouldPreload(std::optional<ResourceType>& type) const {
    if (url_to_load_.IsEmpty())
      return false;
    if (!matched_)
      return false;
#ifndef BLINKIT_CRAWLER_ONLY
    if (Match(tag_impl_, kLinkTag))
      return ShouldPreloadLink(type);
    if (Match(tag_impl_, kInputTag) && !input_is_image_)
      return false;
#endif
    if (Match(tag_impl_, kScriptTag)) {
      ScriptType script_type = ScriptType::kClassic;
      if (!ScriptLoader::IsValidScriptTypeAndLanguage(
              type_attribute_value_, language_attribute_value_,
              ScriptLoader::kAllowLegacyTypeInTypeAttribute, script_type)) {
        return false;
      }
      if (ScriptLoader::BlockForNoModule(script_type,
                                         nomodule_attribute_value_)) {
        return false;
      }
    }
    return true;
  }

#ifndef BLINKIT_CRAWLER_ONLY
  void ParseSourceSize(const String& attribute_value) {
    source_size_ =
        SizesAttributeParser(media_values_, attribute_value).length();
    source_size_set_ = true;
  }
#endif

  void SetReferrerPolicy(
      const String& attribute_value,
      ReferrerPolicyLegacyKeywordsSupport legacy_keywords_support) {
    referrer_policy_set_ = true;
    SecurityPolicy::ReferrerPolicyFromString(
        attribute_value, legacy_keywords_support, &referrer_policy_);
  }

  const StringImpl* tag_impl_;
  String url_to_load_;
#ifndef BLINKIT_CRAWLER_ONLY
  // BKTODO: ImageCandidate srcset_image_candidate_;
#endif
  String charset_;
#ifndef BLINKIT_CRAWLER_ONLY
  bool link_is_style_sheet_;
  bool link_is_preconnect_;
  bool link_is_preload_;
  bool link_is_modulepreload_;
  bool link_is_import_;
#endif
  bool matched_;
#ifndef BLINKIT_CRAWLER_ONLY
  bool input_is_image_;
#endif
  String img_src_url_;
  String srcset_attribute_value_;
  String as_attribute_value_;
  String type_attribute_value_;
  String language_attribute_value_;
  bool nomodule_attribute_value_;
#ifndef BLINKIT_CRAWLER_ONLY
  float source_size_;
  bool source_size_set_;
  Member<MediaValuesCached> media_values_;
#endif
  bool referrer_policy_set_;
  ReferrerPolicy referrer_policy_;
#ifndef BLINKIT_CRAWLER_ONLY
  bool width_attr_small_absolute_;
  bool height_attr_small_absolute_;
#endif
  TokenPreloadScanner::ScannerType scanner_type_;
};

TokenPreloadScanner::TokenPreloadScanner(
    const GURL& document_url,
    std::unique_ptr<CachedDocumentParameters> document_parameters,
    const MediaValuesCached::MediaValuesCachedData& media_values_cached_data,
    const ScannerType scanner_type)
    : document_url_(document_url),
      in_style_(false),
      in_picture_(false),
      in_script_(false),
      template_count_(0),
      document_parameters_(std::move(document_parameters)),
#ifndef BLINKIT_CRAWLER_ONLY
      // BKTODO: media_values_(MediaValuesCached::Create(media_values_cached_data)),
#endif
      scanner_type_(scanner_type),
      did_rewind_(false) {
  DCHECK(document_parameters_.get());
#ifndef BLINKIT_CRAWLER_ONLY
  ASSERT(false); // BKTODO: DCHECK(media_values_.Get());
#endif
  DCHECK(document_url.is_valid());
#ifndef BLINKIT_CRAWLER_ONLY
  ASSERT(false); // BKTODO: css_scanner_.SetReferrerPolicy(document_parameters_->referrer_policy);
#endif
}

TokenPreloadScanner::~TokenPreloadScanner() = default;

TokenPreloadScannerCheckpoint TokenPreloadScanner::CreateCheckpoint() {
  TokenPreloadScannerCheckpoint checkpoint = checkpoints_.size();
  checkpoints_.push_back(Checkpoint(predicted_base_element_url_, in_style_,
                                    in_script_, template_count_));
  return checkpoint;
}

void TokenPreloadScanner::RewindTo(
    TokenPreloadScannerCheckpoint checkpoint_index) {
  // If this ASSERT fires, checkpointIndex is invalid.
  DCHECK_LT(checkpoint_index, checkpoints_.size());
  const Checkpoint& checkpoint = checkpoints_[checkpoint_index];
  predicted_base_element_url_ = checkpoint.predicted_base_element_url;
  in_style_ = checkpoint.in_style;
  template_count_ = checkpoint.template_count;

  did_rewind_ = true;
  in_script_ = checkpoint.in_script;

#ifndef BLINKIT_CRAWLER_ONLY
  css_scanner_.Reset();
#endif
  checkpoints_.clear();
}

void TokenPreloadScanner::Scan(const HTMLToken& token,
                               const SegmentedString& source,
                               PreloadRequestStream& requests,
                               ViewportDescriptionWrapper* viewport,
                               bool* is_csp_meta_tag) {
  ScanCommon(token, source, requests, viewport, is_csp_meta_tag);
}

void TokenPreloadScanner::Scan(const CompactHTMLToken& token,
                               const SegmentedString& source,
                               PreloadRequestStream& requests,
                               ViewportDescriptionWrapper* viewport,
                               bool* is_csp_meta_tag) {
  ScanCommon(token, source, requests, viewport, is_csp_meta_tag);
}

#ifndef BLINKIT_CRAWLER_ONLY
static void HandleMetaViewport(
    const String& attribute_value,
    const CachedDocumentParameters* document_parameters,
    MediaValuesCached* media_values,
    ViewportDescriptionWrapper* viewport) {
  if (!document_parameters->viewport_meta_enabled)
    return;
  ViewportDescription description(ViewportDescription::kViewportMeta);
  ASSERT(false); // BKTODO:
#if 0
  HTMLMetaElement::GetViewportDescriptionFromContentAttribute(
      attribute_value, description, nullptr,
      document_parameters->viewport_meta_zero_values_quirk);
#endif
  if (viewport) {
    viewport->description = description;
    viewport->set = true;
  }
  FloatSize initial_viewport(media_values->DeviceWidth(),
                             media_values->DeviceHeight());
  PageScaleConstraints constraints = description.Resolve(
      initial_viewport, document_parameters->default_viewport_min_width);
  media_values->OverrideViewportDimensions(constraints.layout_size.Width(),
                                           constraints.layout_size.Height());
}
#endif

static void HandleMetaReferrer(const String& attribute_value,
                               CachedDocumentParameters* document_parameters,
                               CSSPreloadScanner* css_scanner) {
  ReferrerPolicy meta_referrer_policy = kReferrerPolicyDefault;
  if (!attribute_value.IsEmpty() && !attribute_value.IsNull() &&
      SecurityPolicy::ReferrerPolicyFromString(
          attribute_value, kSupportReferrerPolicyLegacyKeywords,
          &meta_referrer_policy)) {
    document_parameters->referrer_policy = meta_referrer_policy;
  }
#ifdef BLINKIT_CRAWLER_ONLY
  ASSERT(nullptr == css_scanner);
#else
  ASSERT(false); // BKTODO: css_scanner->SetReferrerPolicy(document_parameters->referrer_policy);
#endif
}

template <typename Token>
static void HandleMetaNameAttribute(
    const Token& token,
    CachedDocumentParameters* document_parameters,
    MediaValuesCached* media_values,
    CSSPreloadScanner* css_scanner,
    ViewportDescriptionWrapper* viewport) {
  const typename Token::Attribute* name_attribute =
      token.GetAttributeItem(kNameAttr);
  if (!name_attribute)
    return;

  String name_attribute_value(name_attribute->Value());
  const typename Token::Attribute* content_attribute =
      token.GetAttributeItem(kContentAttr);
  if (!content_attribute)
    return;

  String content_attribute_value(content_attribute->Value());
#ifndef BLINKIT_CRAWLER_ONLY
  if (DeprecatedEqualIgnoringCase(name_attribute_value, "viewport")) {
    HandleMetaViewport(content_attribute_value, document_parameters,
                       media_values, viewport);
    return;
  }
#endif

  if (DeprecatedEqualIgnoringCase(name_attribute_value, "referrer")) {
    HandleMetaReferrer(content_attribute_value, document_parameters,
                       css_scanner);
  }
}

template <typename Token>
void TokenPreloadScanner::ScanCommon(const Token& token,
                                     const SegmentedString& source,
                                     PreloadRequestStream& requests,
                                     ViewportDescriptionWrapper* viewport,
                                     bool* is_csp_meta_tag) {
  if (!document_parameters_->do_html_preload_scanning)
    return;

  switch (token.GetType()) {
    case HTMLToken::kCharacter: {
#ifdef BLINKIT_CRAWLER_ONLY
      ASSERT(for_crawler_ && !in_style_);
#else
      if (for_crawler_) {
        ASSERT(!in_style_);
      } else if (in_style_) {
        css_scanner_.Scan(token.Data(), source, requests,
                          predicted_base_element_url_);
      }
#endif
      return;
    }
    case HTMLToken::kEndTag: {
      const StringImpl* tag_impl = TagImplFor(token.Data());
#ifdef BLINKIT_CRAWLER_ONLY
      ASSERT(for_crawler_);
      if (Match(tag_impl, kScriptTag)) {
        in_script_ = false;
        return;
      }
#else
      if (!for_crawler_) {
        if (Match(tag_impl, kTemplateTag)) {
          if (template_count_)
            --template_count_;
          return;
        }
        if (Match(tag_impl, kStyleTag)) {
          if (in_style_)
            css_scanner_.Reset();
          in_style_ = false;
          return;
        }
        ASSERT(false); // BKTODO:
#if 0
        if (Match(tag_impl, kPictureTag)) {
          in_picture_ = false;
          picture_data_.picked = false;
        }
#endif
      }
      if (Match(tag_impl, kScriptTag)) {
        in_script_ = false;
        return;
      }
#endif
      return;
    }
    case HTMLToken::kStartTag: {
      if (template_count_)
        return;
      const StringImpl* tag_impl = TagImplFor(token.Data());
      if (Match(tag_impl, kTemplateTag)) {
        ++template_count_;
        return;
      }
#ifndef BLINKIT_CRAWLER_ONLY
      if (!for_crawler_) {
        if (Match(tag_impl, kStyleTag)) {
          in_style_ = true;
          return;
        }
        ASSERT(false); // BKTODO:
#if 0
        if (Match(tag_impl, kPictureTag)) {
          in_picture_ = true;
          picture_data_ = PictureData();
          return;
        }
#endif
      }
#endif
      // Don't early return, because the StartTagScanner needs to look at these
      // too.
      if (Match(tag_impl, kScriptTag)) {
        in_script_ = true;
      }
      if (Match(tag_impl, kBaseTag)) {
        // The first <base> element is the one that wins.
        if (!predicted_base_element_url_.is_empty())
          return;
        UpdatePredictedBaseURL(token);
        return;
      }
      if (Match(tag_impl, kMetaTag)) {
        const typename Token::Attribute* equiv_attribute =
            token.GetAttributeItem(kHttpEquivAttr);
        if (equiv_attribute) {
          String equiv_attribute_value(equiv_attribute->Value());
          if (DeprecatedEqualIgnoringCase(equiv_attribute_value,
                                          "content-security-policy")) {
            *is_csp_meta_tag = true;
          }
          return;
        }
#ifdef BLINKIT_CRAWLER_ONLY
        HandleMetaNameAttribute(token, document_parameters_.get(), nullptr, nullptr, nullptr);
#else
        ASSERT(false); // BKTODO:
#if 0
        HandleMetaNameAttribute(token, document_parameters_.get(),
                                media_values_.Get(), &css_scanner_, viewport);
#endif
#endif
      }

#ifdef BLINKIT_CRAWLER_ONLY
      MediaValuesCached *media_values_ = nullptr;
#else
      ASSERT(false); // BKTODO:
      MediaValuesCached *media_values_ = nullptr;
#endif
      StartTagScanner scanner(tag_impl, media_values_, scanner_type_);
      scanner.ProcessAttributes(token.Attributes());
#ifdef BLINKIT_CRAWLER_ONLY
      PictureData picture_data_;
#else
      ASSERT(false); // BKTODO:
#if 0
      // TODO(yoav): ViewportWidth is currently racy and might be zero in some
      // cases, at least in tests. That problem will go away once
      // ParseHTMLOnMainThread lands and MediaValuesCached is eliminated.
      if (for_crawler_ && in_picture_ && media_values_->ViewportWidth())
        scanner.HandlePictureSourceURL(picture_data_);
#endif
#endif
      std::unique_ptr<PreloadRequest> request = scanner.CreatePreloadRequest(predicted_base_element_url_, source,
          picture_data_, document_parameters_->referrer_policy);
      if (request)
        requests.push_back(std::move(request));
      return;
    }
    default: { return; }
  }
}

template <typename Token>
void TokenPreloadScanner::UpdatePredictedBaseURL(const Token& token) {
  DCHECK(predicted_base_element_url_.is_empty());
  if (const typename Token::Attribute* href_attribute =
          token.GetAttributeItem(kHrefAttr)) {
    ASSERT(false); // BKTODO:
#if 0
    BkURL url(document_url_, StripLeadingAndTrailingHTMLSpaces(
                                href_attribute->Value8BitIfNecessary()));
    predicted_base_element_url_ =
        url.IsValid() && !url.ProtocolIsData() ? url.Copy() : BkURL();
#endif
  }
}

HTMLPreloadScanner::HTMLPreloadScanner(
    const HTMLParserOptions& options,
    const GURL& document_url,
    std::unique_ptr<CachedDocumentParameters> document_parameters,
    const MediaValuesCached::MediaValuesCachedData& media_values_cached_data,
    const TokenPreloadScanner::ScannerType scanner_type)
    : scanner_(document_url,
               std::move(document_parameters),
               media_values_cached_data,
               scanner_type),
      tokenizer_(HTMLTokenizer::Create(options)) {
  if (options.for_crawler)
    scanner_.SetIsForCrawler();
}

HTMLPreloadScanner::~HTMLPreloadScanner() = default;

void HTMLPreloadScanner::AppendToEnd(const SegmentedString& source) {
  source_.Append(source);
}

PreloadRequestStream HTMLPreloadScanner::Scan(
    const GURL& starting_base_element_url,
    ViewportDescriptionWrapper* viewport) {
  // HTMLTokenizer::updateStateFor only works on the main thread.
  DCHECK(IsMainThread());

  // When we start scanning, our best prediction of the baseElementURL is the
  // real one!
  if (!starting_base_element_url.is_empty())
    scanner_.SetPredictedBaseElementURL(starting_base_element_url);

  PreloadRequestStream requests;

  while (tokenizer_->NextToken(source_, token_)) {
    if (token_.GetType() == HTMLToken::kStartTag)
      tokenizer_->UpdateStateFor(
          AttemptStaticStringCreation(token_.GetName(), kLikely8Bit));
    bool is_csp_meta_tag = false;
    scanner_.Scan(token_, source_, requests, viewport, &is_csp_meta_tag);
    token_.Clear();
    // Don't preload anything if a CSP meta tag is found. We should never really
    // find them here because the HTMLPreloadScanner is only used for
    // dynamically added markup.
    if (is_csp_meta_tag)
      return requests;
  }

  return requests;
}

CachedDocumentParameters::CachedDocumentParameters(Document* document) {
  DCHECK(IsMainThread());
  DCHECK(document);
  do_html_preload_scanning = false;
#ifndef BLINKIT_CRAWLER_ONLY
  ASSERT(false); // BKTODO:
#if 0
  do_html_preload_scanning =
      !document->GetSettings() ||
      document->GetSettings()->GetDoHtmlPreloadScanning();
  default_viewport_min_width =
      document->GetViewportData().ViewportDefaultMinWidth();
  viewport_meta_zero_values_quirk =
      document->GetSettings() &&
      document->GetSettings()->GetViewportMetaZeroValuesQuirk();
  viewport_meta_enabled = document->GetSettings() &&
                          document->GetSettings()->GetViewportMetaEnabled();
  referrer_policy = document->GetReferrerPolicy();
  integrity_features = SubresourceIntegrityHelper::GetFeatures(document);
#endif
#endif
  referrer_policy = kReferrerPolicyDefault;
}

}  // namespace blink
