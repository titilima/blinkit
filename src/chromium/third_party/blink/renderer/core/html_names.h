// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_names.h
// Description: HTML Names
//      Author: Ziming Li
//     Created: 2020-07-08
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_HTML_NAMES_H
#define BLINKIT_BLINK_HTML_NAMES_H

#pragma once

#include "third_party/blink/renderer/core/dom/qualified_name.h"

namespace blink {

class HTMLQualifiedName : public QualifiedName {};

namespace html_names {

extern const WTF::AtomicString &xhtmlNamespaceURI;

// Tags
extern const blink::HTMLQualifiedName &kATag;
extern const blink::HTMLQualifiedName &kAddressTag;
extern const blink::HTMLQualifiedName &kAppletTag;
extern const blink::HTMLQualifiedName &kAreaTag;
extern const blink::HTMLQualifiedName &kArticleTag;
extern const blink::HTMLQualifiedName &kAsideTag;
extern const blink::HTMLQualifiedName &kBTag;
extern const blink::HTMLQualifiedName &kBaseTag;
extern const blink::HTMLQualifiedName &kBasefontTag;
extern const blink::HTMLQualifiedName &kBgsoundTag;
extern const blink::HTMLQualifiedName &kBigTag;
extern const blink::HTMLQualifiedName &kBlockquoteTag;
extern const blink::HTMLQualifiedName &kBodyTag;
extern const blink::HTMLQualifiedName &kBrTag;
extern const blink::HTMLQualifiedName &kButtonTag;
extern const blink::HTMLQualifiedName &kCaptionTag;
extern const blink::HTMLQualifiedName &kCenterTag;
extern const blink::HTMLQualifiedName &kCodeTag;
extern const blink::HTMLQualifiedName &kColTag;
extern const blink::HTMLQualifiedName &kColgroupTag;
extern const blink::HTMLQualifiedName &kCommandTag;
extern const blink::HTMLQualifiedName &kContentTag;
extern const blink::HTMLQualifiedName &kDdTag;
extern const blink::HTMLQualifiedName &kDetailsTag;
extern const blink::HTMLQualifiedName &kDirTag;
extern const blink::HTMLQualifiedName &kDivTag;
extern const blink::HTMLQualifiedName &kDlTag;
extern const blink::HTMLQualifiedName &kDtTag;
extern const blink::HTMLQualifiedName &kEmTag;
extern const blink::HTMLQualifiedName &kEmbedTag;
extern const blink::HTMLQualifiedName &kFieldsetTag;
extern const blink::HTMLQualifiedName &kFigcaptionTag;
extern const blink::HTMLQualifiedName &kFigureTag;
extern const blink::HTMLQualifiedName &kFontTag;
extern const blink::HTMLQualifiedName &kFooterTag;
extern const blink::HTMLQualifiedName &kFormTag;
extern const blink::HTMLQualifiedName &kFrameTag;
extern const blink::HTMLQualifiedName &kFramesetTag;
extern const blink::HTMLQualifiedName &kH1Tag;
extern const blink::HTMLQualifiedName &kH2Tag;
extern const blink::HTMLQualifiedName &kH3Tag;
extern const blink::HTMLQualifiedName &kH4Tag;
extern const blink::HTMLQualifiedName &kH5Tag;
extern const blink::HTMLQualifiedName &kH6Tag;
extern const blink::HTMLQualifiedName &kHeadTag;
extern const blink::HTMLQualifiedName &kHeaderTag;
extern const blink::HTMLQualifiedName &kHgroupTag;
extern const blink::HTMLQualifiedName &kHrTag;
extern const blink::HTMLQualifiedName &kHTMLTag;
extern const blink::HTMLQualifiedName &kITag;
extern const blink::HTMLQualifiedName &kIFrameTag;
extern const blink::HTMLQualifiedName &kImageTag;
extern const blink::HTMLQualifiedName &kImgTag;
extern const blink::HTMLQualifiedName &kInputTag;
extern const blink::HTMLQualifiedName &kKeygenTag;
extern const blink::HTMLQualifiedName &kLabelTag;
extern const blink::HTMLQualifiedName &kLiTag;
extern const blink::HTMLQualifiedName &kLinkTag;
extern const blink::HTMLQualifiedName &kListingTag;
extern const blink::HTMLQualifiedName &kMainTag;
extern const blink::HTMLQualifiedName &kMarqueeTag;
extern const blink::HTMLQualifiedName &kMenuTag;
extern const blink::HTMLQualifiedName &kMetaTag;
extern const blink::HTMLQualifiedName &kNavTag;
extern const blink::HTMLQualifiedName &kNobrTag;
extern const blink::HTMLQualifiedName &kNoembedTag;
extern const blink::HTMLQualifiedName &kNoframesTag;
extern const blink::HTMLQualifiedName &kNoscriptTag;
extern const blink::HTMLQualifiedName &kObjectTag;
extern const blink::HTMLQualifiedName &kOlTag;
extern const blink::HTMLQualifiedName &kOptgroupTag;
extern const blink::HTMLQualifiedName &kOptionTag;
extern const blink::HTMLQualifiedName &kPTag;
extern const blink::HTMLQualifiedName &kParamTag;
extern const blink::HTMLQualifiedName &kPlaintextTag;
extern const blink::HTMLQualifiedName &kPreTag;
extern const blink::HTMLQualifiedName &kRbTag;
extern const blink::HTMLQualifiedName &kRpTag;
extern const blink::HTMLQualifiedName &kRtTag;
extern const blink::HTMLQualifiedName &kRTCTag;
extern const blink::HTMLQualifiedName &kRubyTag;
extern const blink::HTMLQualifiedName &kSTag;
extern const blink::HTMLQualifiedName &kScriptTag;
extern const blink::HTMLQualifiedName &kSectionTag;
extern const blink::HTMLQualifiedName &kSelectTag;
extern const blink::HTMLQualifiedName &kSmallTag;
extern const blink::HTMLQualifiedName &kSourceTag;
extern const blink::HTMLQualifiedName &kSpanTag;
extern const blink::HTMLQualifiedName &kStrikeTag;
extern const blink::HTMLQualifiedName &kStrongTag;
extern const blink::HTMLQualifiedName &kStyleTag;
extern const blink::HTMLQualifiedName &kSubTag;
extern const blink::HTMLQualifiedName &kSummaryTag;
extern const blink::HTMLQualifiedName &kSupTag;
extern const blink::HTMLQualifiedName &kTableTag;
extern const blink::HTMLQualifiedName &kTbodyTag;
extern const blink::HTMLQualifiedName &kTdTag;
extern const blink::HTMLQualifiedName &kTemplateTag;
extern const blink::HTMLQualifiedName &kTextareaTag;
extern const blink::HTMLQualifiedName &kTfootTag;
extern const blink::HTMLQualifiedName &kThTag;
extern const blink::HTMLQualifiedName &kTheadTag;
extern const blink::HTMLQualifiedName &kTitleTag;
extern const blink::HTMLQualifiedName &kTrTag;
extern const blink::HTMLQualifiedName &kTrackTag;
extern const blink::HTMLQualifiedName &kTtTag;
extern const blink::HTMLQualifiedName &kUTag;
extern const blink::HTMLQualifiedName &kUlTag;
extern const blink::HTMLQualifiedName &kVarTag;
extern const blink::HTMLQualifiedName &kVideoTag;
extern const blink::HTMLQualifiedName &kWbrTag;
extern const blink::HTMLQualifiedName &kXmpTag;

constexpr unsigned kTagsCount = 112;

// Attributes
extern const blink::QualifiedName &kAcceptAttr;
extern const blink::QualifiedName &kAcceptCharsetAttr;
extern const blink::QualifiedName &kAlignAttr;
extern const blink::QualifiedName &kAlinkAttr;
extern const blink::QualifiedName &kAxisAttr;
extern const blink::QualifiedName &kBgcolorAttr;
extern const blink::QualifiedName &kCharsetAttr;
extern const blink::QualifiedName &kCheckedAttr;
extern const blink::QualifiedName &kClassAttr;
extern const blink::QualifiedName &kClearAttr;
extern const blink::QualifiedName &kCodetypeAttr;
extern const blink::QualifiedName &kColorAttr;
extern const blink::QualifiedName &kCompactAttr;
extern const blink::QualifiedName &kContentAttr;
extern const blink::QualifiedName &kDeclareAttr;
extern const blink::QualifiedName &kDeferAttr;
extern const blink::QualifiedName &kDirAttr;
extern const blink::QualifiedName &kDirectionAttr;
extern const blink::QualifiedName &kDisabledAttr;
extern const blink::QualifiedName &kEnctypeAttr;
extern const blink::QualifiedName &kEventAttr;
extern const blink::QualifiedName &kFaceAttr;
extern const blink::QualifiedName &kForAttr;
extern const blink::QualifiedName &kFormAttr;
extern const blink::QualifiedName &kFrameAttr;
extern const blink::QualifiedName &kHrefAttr;
extern const blink::QualifiedName &kHreflangAttr;
extern const blink::QualifiedName &kHttpEquivAttr;
extern const blink::QualifiedName &kIdAttr;
extern const blink::QualifiedName &kIsAttr;
extern const blink::QualifiedName &kLabelAttr;
extern const blink::QualifiedName &kLangAttr;
extern const blink::QualifiedName &kLanguageAttr;
extern const blink::QualifiedName &kLinkAttr;
extern const blink::QualifiedName &kMediaAttr;
extern const blink::QualifiedName &kMethodAttr;
extern const blink::QualifiedName &kMultipleAttr;
extern const blink::QualifiedName &kNameAttr;
extern const blink::QualifiedName &kNohrefAttr;
extern const blink::QualifiedName &kNomoduleAttr;
extern const blink::QualifiedName &kNoresizeAttr;
extern const blink::QualifiedName &kNoshadeAttr;
extern const blink::QualifiedName &kNowrapAttr;
extern const blink::QualifiedName &kObjectAttr;
extern const blink::QualifiedName &kOnloadAttr;
extern const blink::QualifiedName &kReadonlyAttr;
extern const blink::QualifiedName &kReferrerpolicyAttr;
extern const blink::QualifiedName &kRelAttr;
extern const blink::QualifiedName &kRevAttr;
extern const blink::QualifiedName &kRulesAttr;
extern const blink::QualifiedName &kScopeAttr;
extern const blink::QualifiedName &kScrollingAttr;
extern const blink::QualifiedName &kSelectAttr;
extern const blink::QualifiedName &kSelectedAttr;
extern const blink::QualifiedName &kShapeAttr;
extern const blink::QualifiedName &kSizeAttr;
extern const blink::QualifiedName &kSpanAttr;
extern const blink::QualifiedName &kSrcAttr;
extern const blink::QualifiedName &kStyleAttr;
extern const blink::QualifiedName &kSummaryAttr;
extern const blink::QualifiedName &kTargetAttr;
extern const blink::QualifiedName &kTextAttr;
extern const blink::QualifiedName &kTitleAttr;
extern const blink::QualifiedName &kTypeAttr;
extern const blink::QualifiedName &kValignAttr;
extern const blink::QualifiedName &kValuetypeAttr;
extern const blink::QualifiedName &kVlinkAttr;

constexpr unsigned kAttrsCount = 67;

void Init(void);

} // namespace html_names
} // namespace blink

#endif // BLINKIT_BLINK_HTML_NAMES_H
