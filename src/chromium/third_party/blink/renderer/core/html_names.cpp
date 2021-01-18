// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_names.cpp
// Description: HTML Names
//      Author: Ziming Li
//     Created: 2021-01-17
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "html_names.h"

#include <iterator>

namespace blink {
namespace html_names {

static void* ns_uri_storage[(sizeof(AtomicString) + sizeof(void *) - 1) / sizeof(void *)];
const AtomicString &xhtmlNamespaceURI = *reinterpret_cast<AtomicString *>(&ns_uri_storage);

// Tags
static void* tag_storage[kTagsCount * ((sizeof(HTMLQualifiedName) + sizeof(void *) - 1) / sizeof(void *))];
const HTMLQualifiedName &kATag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[0];
const HTMLQualifiedName &kAbbrTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[1];
const HTMLQualifiedName &kAddressTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[2];
const HTMLQualifiedName &kAppletTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[3];
const HTMLQualifiedName &kAreaTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[4];
const HTMLQualifiedName &kArticleTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[5];
const HTMLQualifiedName &kAsideTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[6];
const HTMLQualifiedName &kBTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[7];
const HTMLQualifiedName &kBaseTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[8];
const HTMLQualifiedName &kBasefontTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[9];
const HTMLQualifiedName &kBgsoundTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[10];
const HTMLQualifiedName &kBigTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[11];
const HTMLQualifiedName &kBlockquoteTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[12];
const HTMLQualifiedName &kBodyTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[13];
const HTMLQualifiedName &kBrTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[14];
const HTMLQualifiedName &kButtonTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[15];
const HTMLQualifiedName &kCaptionTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[16];
const HTMLQualifiedName &kCenterTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[17];
const HTMLQualifiedName &kCodeTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[18];
const HTMLQualifiedName &kColTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[19];
const HTMLQualifiedName &kColgroupTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[20];
const HTMLQualifiedName &kCommandTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[21];
const HTMLQualifiedName &kContentTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[22];
const HTMLQualifiedName &kDatalistTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[23];
const HTMLQualifiedName &kDdTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[24];
const HTMLQualifiedName &kDetailsTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[25];
const HTMLQualifiedName &kDialogTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[26];
const HTMLQualifiedName &kDirTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[27];
const HTMLQualifiedName &kDivTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[28];
const HTMLQualifiedName &kDlTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[29];
const HTMLQualifiedName &kDtTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[30];
const HTMLQualifiedName &kEmTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[31];
const HTMLQualifiedName &kEmbedTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[32];
const HTMLQualifiedName &kFieldsetTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[33];
const HTMLQualifiedName &kFigcaptionTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[34];
const HTMLQualifiedName &kFigureTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[35];
const HTMLQualifiedName &kFontTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[36];
const HTMLQualifiedName &kFooterTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[37];
const HTMLQualifiedName &kFormTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[38];
const HTMLQualifiedName &kFrameTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[39];
const HTMLQualifiedName &kFramesetTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[40];
const HTMLQualifiedName &kH1Tag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[41];
const HTMLQualifiedName &kH2Tag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[42];
const HTMLQualifiedName &kH3Tag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[43];
const HTMLQualifiedName &kH4Tag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[44];
const HTMLQualifiedName &kH5Tag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[45];
const HTMLQualifiedName &kH6Tag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[46];
const HTMLQualifiedName &kHeadTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[47];
const HTMLQualifiedName &kHeaderTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[48];
const HTMLQualifiedName &kHgroupTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[49];
const HTMLQualifiedName &kHrTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[50];
const HTMLQualifiedName &kHTMLTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[51];
const HTMLQualifiedName &kITag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[52];
const HTMLQualifiedName &kIFrameTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[53];
const HTMLQualifiedName &kImageTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[54];
const HTMLQualifiedName &kImgTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[55];
const HTMLQualifiedName &kInputTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[56];
const HTMLQualifiedName &kKeygenTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[57];
const HTMLQualifiedName &kLabelTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[58];
const HTMLQualifiedName &kLegendTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[59];
const HTMLQualifiedName &kLiTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[60];
const HTMLQualifiedName &kLinkTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[61];
const HTMLQualifiedName &kListingTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[62];
const HTMLQualifiedName &kMainTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[63];
const HTMLQualifiedName &kMarqueeTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[64];
const HTMLQualifiedName &kMenuTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[65];
const HTMLQualifiedName &kMetaTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[66];
const HTMLQualifiedName &kNavTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[67];
const HTMLQualifiedName &kNobrTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[68];
const HTMLQualifiedName &kNoembedTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[69];
const HTMLQualifiedName &kNoframesTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[70];
const HTMLQualifiedName &kNoscriptTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[71];
const HTMLQualifiedName &kObjectTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[72];
const HTMLQualifiedName &kOlTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[73];
const HTMLQualifiedName &kOptgroupTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[74];
const HTMLQualifiedName &kOptionTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[75];
const HTMLQualifiedName &kPTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[76];
const HTMLQualifiedName &kParamTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[77];
const HTMLQualifiedName &kPictureTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[78];
const HTMLQualifiedName &kPlaintextTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[79];
const HTMLQualifiedName &kPreTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[80];
const HTMLQualifiedName &kRbTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[81];
const HTMLQualifiedName &kRpTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[82];
const HTMLQualifiedName &kRtTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[83];
const HTMLQualifiedName &kRTCTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[84];
const HTMLQualifiedName &kRubyTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[85];
const HTMLQualifiedName &kSTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[86];
const HTMLQualifiedName &kScriptTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[87];
const HTMLQualifiedName &kSectionTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[88];
const HTMLQualifiedName &kSelectTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[89];
const HTMLQualifiedName &kSlotTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[90];
const HTMLQualifiedName &kSmallTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[91];
const HTMLQualifiedName &kSourceTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[92];
const HTMLQualifiedName &kSpanTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[93];
const HTMLQualifiedName &kStrikeTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[94];
const HTMLQualifiedName &kStrongTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[95];
const HTMLQualifiedName &kStyleTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[96];
const HTMLQualifiedName &kSubTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[97];
const HTMLQualifiedName &kSummaryTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[98];
const HTMLQualifiedName &kSupTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[99];
const HTMLQualifiedName &kTableTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[100];
const HTMLQualifiedName &kTbodyTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[101];
const HTMLQualifiedName &kTdTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[102];
const HTMLQualifiedName &kTemplateTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[103];
const HTMLQualifiedName &kTextareaTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[104];
const HTMLQualifiedName &kTfootTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[105];
const HTMLQualifiedName &kThTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[106];
const HTMLQualifiedName &kTheadTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[107];
const HTMLQualifiedName &kTitleTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[108];
const HTMLQualifiedName &kTrTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[109];
const HTMLQualifiedName &kTrackTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[110];
const HTMLQualifiedName &kTtTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[111];
const HTMLQualifiedName &kUTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[112];
const HTMLQualifiedName &kUlTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[113];
const HTMLQualifiedName &kVarTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[114];
const HTMLQualifiedName &kVideoTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[115];
const HTMLQualifiedName &kWbrTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[116];
const HTMLQualifiedName &kXmpTag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[117];

// Attributes
static void* attr_storage[kAttrsCount * ((sizeof(QualifiedName) + sizeof(void *) - 1) / sizeof(void *))];
const QualifiedName &kAbbrAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[0];
const QualifiedName &kAcceptAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[1];
const QualifiedName &kAcceptCharsetAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[2];
const QualifiedName &kAlignAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[3];
const QualifiedName &kAlinkAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[4];
const QualifiedName &kAltAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[5];
const QualifiedName &kAutofocusAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[6];
const QualifiedName &kAxisAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[7];
const QualifiedName &kBackgroundAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[8];
const QualifiedName &kBgcolorAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[9];
const QualifiedName &kBorderAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[10];
const QualifiedName &kBordercolorAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[11];
const QualifiedName &kCellpaddingAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[12];
const QualifiedName &kCellspacingAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[13];
const QualifiedName &kCharsetAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[14];
const QualifiedName &kCheckedAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[15];
const QualifiedName &kClassAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[16];
const QualifiedName &kClearAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[17];
const QualifiedName &kCodetypeAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[18];
const QualifiedName &kColsAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[19];
const QualifiedName &kColorAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[20];
const QualifiedName &kColspanAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[21];
const QualifiedName &kCompactAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[22];
const QualifiedName &kContentAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[23];
const QualifiedName &kContenteditableAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[24];
const QualifiedName &kDeclareAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[25];
const QualifiedName &kDeferAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[26];
const QualifiedName &kDirAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[27];
const QualifiedName &kDirectionAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[28];
const QualifiedName &kDisabledAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[29];
const QualifiedName &kDraggableAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[30];
const QualifiedName &kEnctypeAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[31];
const QualifiedName &kEventAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[32];
const QualifiedName &kFaceAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[33];
const QualifiedName &kForAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[34];
const QualifiedName &kFormAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[35];
const QualifiedName &kFrameAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[36];
const QualifiedName &kHeadersAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[37];
const QualifiedName &kHeightAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[38];
const QualifiedName &kHiddenAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[39];
const QualifiedName &kHrefAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[40];
const QualifiedName &kHreflangAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[41];
const QualifiedName &kHspaceAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[42];
const QualifiedName &kHttpEquivAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[43];
const QualifiedName &kIdAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[44];
const QualifiedName &kInvisibleAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[45];
const QualifiedName &kIsAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[46];
const QualifiedName &kLabelAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[47];
const QualifiedName &kLangAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[48];
const QualifiedName &kLanguageAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[49];
const QualifiedName &kLinkAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[50];
const QualifiedName &kMediaAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[51];
const QualifiedName &kMethodAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[52];
const QualifiedName &kMultipleAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[53];
const QualifiedName &kNameAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[54];
const QualifiedName &kNohrefAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[55];
const QualifiedName &kNomoduleAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[56];
const QualifiedName &kNoresizeAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[57];
const QualifiedName &kNoshadeAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[58];
const QualifiedName &kNowrapAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[59];
const QualifiedName &kObjectAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[60];
const QualifiedName &kOnloadAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[61];
const QualifiedName &kPartAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[62];
const QualifiedName &kPseudoAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[63];
const QualifiedName &kReadonlyAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[64];
const QualifiedName &kReferrerpolicyAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[65];
const QualifiedName &kRelAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[66];
const QualifiedName &kRevAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[67];
const QualifiedName &kReversedAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[68];
const QualifiedName &kRowspanAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[69];
const QualifiedName &kRulesAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[70];
const QualifiedName &kScopeAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[71];
const QualifiedName &kScrollingAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[72];
const QualifiedName &kSelectAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[73];
const QualifiedName &kSelectedAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[74];
const QualifiedName &kShapeAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[75];
const QualifiedName &kSizeAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[76];
const QualifiedName &kSizesAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[77];
const QualifiedName &kSlotAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[78];
const QualifiedName &kSpanAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[79];
const QualifiedName &kSrcAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[80];
const QualifiedName &kSrcsetAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[81];
const QualifiedName &kStartAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[82];
const QualifiedName &kStyleAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[83];
const QualifiedName &kSummaryAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[84];
const QualifiedName &kTabindexAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[85];
const QualifiedName &kTargetAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[86];
const QualifiedName &kTextAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[87];
const QualifiedName &kTitleAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[88];
const QualifiedName &kTypeAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[89];
const QualifiedName &kUsemapAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[90];
const QualifiedName &kValignAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[91];
const QualifiedName &kValueAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[92];
const QualifiedName &kValuetypeAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[93];
const QualifiedName &kVlinkAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[94];
const QualifiedName &kVspaceAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[95];
const QualifiedName &kWidthAttr = reinterpret_cast<QualifiedName *>(&attr_storage)[96];

void Init(void)
{
    struct NameEntry {
        const char* name;
        unsigned hash;
        unsigned char length;
        unsigned char isTag;
        unsigned char isAttr;
    };

    // Namespace
    // Use placement new to initialize the globals.
    AtomicString ns_uri("http://www.w3.org/1999/xhtml");
    void *dst = const_cast<AtomicString *>(&xhtmlNamespaceURI);
    new (dst) AtomicString(ns_uri);

    static const NameEntry kNames[] = {
        { "a", 9778235, 1, 1, 0 },
        { "abbr", 650525, 4, 1, 1 },
        { "accept", 4839857, 6, 0, 1 },
        { "accept-charset", 5192676, 14, 0, 1 },
        { "address", 10008206, 7, 1, 0 },
        { "align", 10094397, 5, 0, 1 },
        { "alink", 2408650, 5, 0, 1 },
        { "alt", 15335609, 3, 0, 1 },
        { "applet", 7823075, 6, 1, 0 },
        { "area", 7355486, 4, 1, 0 },
        { "article", 2968800, 7, 1, 0 },
        { "aside", 10128566, 5, 1, 0 },
        { "autofocus", 16282013, 9, 0, 1 },
        { "axis", 14347904, 4, 0, 1 },
        { "b", 7170995, 1, 1, 0 },
        { "background", 8107606, 10, 0, 1 },
        { "base", 4376626, 4, 1, 0 },
        { "basefont", 5439393, 8, 1, 0 },
        { "bgcolor", 11074228, 7, 0, 1 },
        { "bgsound", 9424179, 7, 1, 0 },
        { "big", 2330122, 3, 1, 0 },
        { "blockquote", 6047050, 10, 1, 0 },
        { "body", 4862692, 4, 1, 0 },
        { "border", 4389816, 6, 0, 1 },
        { "bordercolor", 12757423, 11, 0, 1 },
        { "br", 5574895, 2, 1, 0 },
        { "button", 679691, 6, 1, 0 },
        { "caption", 2437650, 7, 1, 0 },
        { "cellpadding", 3763331, 11, 0, 1 },
        { "cellspacing", 7849745, 11, 0, 1 },
        { "center", 909285, 6, 1, 0 },
        { "charset", 14273970, 7, 0, 1 },
        { "checked", 10213523, 7, 0, 1 },
        { "class", 1023741, 5, 0, 1 },
        { "clear", 12832795, 5, 0, 1 },
        { "code", 11588001, 4, 1, 0 },
        { "codetype", 7482920, 8, 0, 1 },
        { "col", 12850806, 3, 1, 0 },
        { "cols", 4921975, 4, 0, 1 },
        { "colgroup", 3733719, 8, 1, 0 },
        { "color", 2734929, 5, 0, 1 },
        { "colspan", 9715319, 7, 0, 1 },
        { "command", 2099501, 7, 1, 0 },
        { "compact", 369367, 7, 0, 1 },
        { "content", 6894765, 7, 1, 1 },
        { "contenteditable", 1577657, 15, 0, 1 },
        { "datalist", 7058973, 8, 1, 0 },
        { "dd", 16480447, 2, 1, 0 },
        { "declare", 13440114, 7, 0, 1 },
        { "defer", 13832204, 5, 0, 1 },
        { "details", 4653180, 7, 1, 0 },
        { "dialog", 1232842, 6, 1, 0 },
        { "dir", 5122803, 3, 1, 1 },
        { "direction", 16092904, 9, 0, 1 },
        { "disabled", 12237545, 8, 0, 1 },
        { "div", 9762591, 3, 1, 0 },
        { "dl", 5117854, 2, 1, 0 },
        { "draggable", 9281278, 9, 0, 1 },
        { "dt", 8177126, 2, 1, 0 },
        { "em", 2724661, 2, 1, 0 },
        { "embed", 855371, 5, 1, 0 },
        { "enctype", 11388071, 7, 0, 1 },
        { "event", 4205160, 5, 0, 1 },
        { "face", 12427611, 4, 0, 1 },
        { "fieldset", 37097, 8, 1, 0 },
        { "figcaption", 13388641, 10, 1, 0 },
        { "figure", 13776479, 6, 1, 0 },
        { "font", 4588695, 4, 1, 0 },
        { "footer", 13359933, 6, 1, 0 },
        { "for", 4145292, 3, 0, 1 },
        { "form", 10060301, 4, 1, 1 },
        { "frame", 3408403, 5, 1, 1 },
        { "frameset", 11229107, 8, 1, 0 },
        { "h1", 3953979, 2, 1, 0 },
        { "h2", 9378819, 2, 1, 0 },
        { "h3", 1803164, 2, 1, 0 },
        { "h4", 15343268, 2, 1, 0 },
        { "h5", 8065295, 2, 1, 0 },
        { "h6", 9566891, 2, 1, 0 },
        { "head", 11457121, 4, 1, 0 },
        { "header", 5896178, 6, 1, 0 },
        { "headers", 2627673, 7, 0, 1 },
        { "height", 6697833, 6, 0, 1 },
        { "hgroup", 8927907, 6, 1, 0 },
        { "hidden", 12930326, 6, 0, 1 },
        { "hr", 7182703, 2, 1, 0 },
        { "href", 5797448, 4, 0, 1 },
        { "hreflang", 12582042, 8, 0, 1 },
        { "hspace", 4182216, 6, 0, 1 },
        { "html", 7123128, 4, 1, 0 },
        { "http-equiv", 12415310, 10, 0, 1 },
        { "i", 16149244, 1, 1, 0 },
        { "id", 6145549, 2, 0, 1 },
        { "iframe", 16643553, 6, 1, 0 },
        { "image", 10287573, 5, 1, 0 },
        { "img", 11398162, 3, 1, 0 },
        { "input", 10365436, 5, 1, 0 },
        { "invisible", 2501496, 9, 0, 1 },
        { "is", 15617733, 2, 0, 1 },
        { "keygen", 5603105, 6, 1, 0 },
        { "label", 15092473, 5, 1, 1 },
        { "lang", 3702417, 4, 0, 1 },
        { "language", 4109061, 8, 0, 1 },
        { "legend", 16417202, 6, 1, 0 },
        { "li", 11097939, 2, 1, 0 },
        { "link", 7010491, 4, 1, 1 },
        { "listing", 15674586, 7, 1, 0 },
        { "main", 13501767, 4, 1, 0 },
        { "marquee", 1416619, 7, 1, 0 },
        { "media", 13905581, 5, 0, 1 },
        { "menu", 13428288, 4, 1, 0 },
        { "meta", 14067019, 4, 1, 0 },
        { "method", 14679268, 6, 0, 1 },
        { "multiple", 6185486, 8, 0, 1 },
        { "name", 8774809, 4, 0, 1 },
        { "nav", 15269619, 3, 1, 0 },
        { "nobr", 419763, 4, 1, 0 },
        { "noembed", 11476379, 7, 1, 0 },
        { "noframes", 13751064, 8, 1, 0 },
        { "nohref", 2672291, 6, 0, 1 },
        { "nomodule", 10903441, 8, 0, 1 },
        { "noresize", 11098542, 8, 0, 1 },
        { "noscript", 10168152, 8, 1, 0 },
        { "noshade", 12172860, 7, 0, 1 },
        { "nowrap", 15804466, 6, 0, 1 },
        { "object", 6907086, 6, 1, 1 },
        { "ol", 6672076, 2, 1, 0 },
        { "onload", 1245164, 6, 0, 1 },
        { "optgroup", 10180907, 8, 1, 0 },
        { "option", 2998548, 6, 1, 0 },
        { "p", 587733, 1, 1, 0 },
        { "param", 15210019, 5, 1, 0 },
        { "part", 10491608, 4, 0, 1 },
        { "picture", 7136372, 7, 1, 0 },
        { "plaintext", 2551274, 9, 1, 0 },
        { "pre", 16061734, 3, 1, 0 },
        { "pseudo", 6923620, 6, 0, 1 },
        { "rb", 4749743, 2, 1, 0 },
        { "readonly", 4471832, 8, 0, 1 },
        { "referrerpolicy", 13927145, 14, 0, 1 },
        { "rel", 8963158, 3, 0, 1 },
        { "rev", 12217053, 3, 0, 1 },
        { "reversed", 15723650, 8, 0, 1 },
        { "rowspan", 13630419, 7, 0, 1 },
        { "rp", 4502666, 2, 1, 0 },
        { "rt", 8494892, 2, 1, 0 },
        { "rtc", 4350543, 3, 1, 0 },
        { "ruby", 12793227, 4, 1, 0 },
        { "rules", 8604373, 5, 0, 1 },
        { "s", 11249530, 1, 1, 0 },
        { "scope", 5624816, 5, 0, 1 },
        { "script", 7137273, 6, 1, 0 },
        { "scrolling", 4321742, 9, 0, 1 },
        { "section", 8056235, 7, 1, 0 },
        { "select", 210571, 6, 1, 1 },
        { "selected", 15762609, 8, 0, 1 },
        { "shape", 15210140, 5, 0, 1 },
        { "size", 12715024, 4, 0, 1 },
        { "sizes", 6795010, 5, 0, 1 },
        { "slot", 8744197, 4, 1, 1 },
        { "small", 14362904, 5, 1, 0 },
        { "source", 341674, 6, 1, 0 },
        { "span", 11168892, 4, 1, 1 },
        { "src", 11517827, 3, 0, 1 },
        { "srcset", 6236095, 6, 0, 1 },
        { "start", 1021290, 5, 0, 1 },
        { "strike", 15072495, 6, 1, 0 },
        { "strong", 13282129, 6, 1, 0 },
        { "style", 10993676, 5, 1, 1 },
        { "sub", 15782866, 3, 1, 0 },
        { "summary", 1148260, 7, 1, 1 },
        { "sup", 8741418, 3, 1, 0 },
        { "tabindex", 10415591, 8, 0, 1 },
        { "table", 8181317, 5, 1, 0 },
        { "target", 1752822, 6, 0, 1 },
        { "tbody", 14028375, 5, 1, 0 },
        { "td", 10309076, 2, 1, 0 },
        { "template", 12693000, 8, 1, 0 },
        { "text", 2784654, 4, 0, 1 },
        { "textarea", 10510497, 8, 1, 0 },
        { "tfoot", 13009441, 5, 1, 0 },
        { "th", 3915297, 2, 1, 0 },
        { "thead", 13349519, 5, 1, 0 },
        { "title", 2337488, 5, 1, 1 },
        { "tr", 16624306, 2, 1, 0 },
        { "track", 10988706, 5, 1, 0 },
        { "tt", 9170852, 2, 1, 0 },
        { "type", 1916283, 4, 0, 1 },
        { "u", 13317144, 1, 1, 0 },
        { "ul", 12324505, 2, 1, 0 },
        { "usemap", 16643238, 6, 0, 1 },
        { "valign", 8345445, 6, 0, 1 },
        { "value", 10871467, 5, 0, 1 },
        { "valuetype", 4808367, 9, 0, 1 },
        { "var", 9573901, 3, 1, 0 },
        { "video", 16765639, 5, 1, 0 },
        { "vlink", 12743618, 5, 0, 1 },
        { "vspace", 9862276, 6, 0, 1 },
        { "wbr", 8290101, 3, 1, 0 },
        { "width", 12902275, 5, 0, 1 },
        { "xmp", 8764488, 3, 1, 0 },
    };

    size_t tag_i = 0, attr_i = 0;
    for (size_t i = 0; i < std::size(kNames); ++i)
    {
        StringImpl *impl = StringImpl::CreateStatic(kNames[i].name, kNames[i].length, kNames[i].hash);
        if (kNames[i].isTag)
        {
            void *address = reinterpret_cast<HTMLQualifiedName *>(&tag_storage) + tag_i;
            QualifiedName::CreateStatic(address, impl, ns_uri);
            ++tag_i;
        }

        if (!kNames[i].isAttr)
            continue;

        void *address = reinterpret_cast<QualifiedName *>(&attr_storage) + attr_i;
        QualifiedName::CreateStatic(address, impl);
        ++attr_i;
    }

    ASSERT(kTagsCount == tag_i);
    ASSERT(kAttrsCount == attr_i);
}

} // namespace html_names
} // namespace blink
