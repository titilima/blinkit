// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "HTMLNames.h"

#include "wtf/StaticConstructors.h"
#include "wtf/StdLibExtras.h"

namespace blink {
namespace HTMLNames {

using namespace blink;

DEFINE_GLOBAL(AtomicString, xhtmlNamespaceURI)

// Tags

void* TagStorage[HTMLTagsCount * ((sizeof(HTMLQualifiedName) + sizeof(void *) - 1) / sizeof(void *))];
const HTMLQualifiedName& aTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[0];
const HTMLQualifiedName& addressTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[1];
const HTMLQualifiedName& areaTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[2];
const HTMLQualifiedName& articleTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[3];
const HTMLQualifiedName& asideTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[4];
const HTMLQualifiedName& bTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[5];
const HTMLQualifiedName& baseTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[6];
const HTMLQualifiedName& basefontTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[7];
const HTMLQualifiedName& bdiTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[8];
const HTMLQualifiedName& bigTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[9];
const HTMLQualifiedName& blockquoteTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[10];
const HTMLQualifiedName& bodyTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[11];
const HTMLQualifiedName& brTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[12];
const HTMLQualifiedName& buttonTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[13];
const HTMLQualifiedName& captionTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[14];
const HTMLQualifiedName& centerTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[15];
const HTMLQualifiedName& codeTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[16];
const HTMLQualifiedName& colTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[17];
const HTMLQualifiedName& colgroupTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[18];
const HTMLQualifiedName& commandTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[19];
const HTMLQualifiedName& contentTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[20];
const HTMLQualifiedName& datalistTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[21];
const HTMLQualifiedName& ddTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[22];
const HTMLQualifiedName& detailsTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[23];
const HTMLQualifiedName& dirTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[24];
const HTMLQualifiedName& divTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[25];
const HTMLQualifiedName& dlTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[26];
const HTMLQualifiedName& dtTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[27];
const HTMLQualifiedName& emTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[28];
const HTMLQualifiedName& fieldsetTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[29];
const HTMLQualifiedName& figcaptionTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[30];
const HTMLQualifiedName& figureTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[31];
const HTMLQualifiedName& fontTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[32];
const HTMLQualifiedName& footerTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[33];
const HTMLQualifiedName& formTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[34];
const HTMLQualifiedName& h1Tag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[35];
const HTMLQualifiedName& h2Tag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[36];
const HTMLQualifiedName& h3Tag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[37];
const HTMLQualifiedName& h4Tag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[38];
const HTMLQualifiedName& h5Tag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[39];
const HTMLQualifiedName& h6Tag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[40];
const HTMLQualifiedName& headTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[41];
const HTMLQualifiedName& headerTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[42];
const HTMLQualifiedName& hgroupTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[43];
const HTMLQualifiedName& hrTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[44];
const HTMLQualifiedName& htmlTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[45];
const HTMLQualifiedName& iTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[46];
const HTMLQualifiedName& imageTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[47];
const HTMLQualifiedName& imgTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[48];
const HTMLQualifiedName& inputTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[49];
const HTMLQualifiedName& labelTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[50];
const HTMLQualifiedName& legendTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[51];
const HTMLQualifiedName& liTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[52];
const HTMLQualifiedName& linkTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[53];
const HTMLQualifiedName& listingTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[54];
const HTMLQualifiedName& mainTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[55];
const HTMLQualifiedName& marqueeTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[56];
const HTMLQualifiedName& menuTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[57];
const HTMLQualifiedName& menuitemTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[58];
const HTMLQualifiedName& metaTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[59];
const HTMLQualifiedName& meterTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[60];
const HTMLQualifiedName& navTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[61];
const HTMLQualifiedName& nobrTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[62];
const HTMLQualifiedName& olTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[63];
const HTMLQualifiedName& optgroupTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[64];
const HTMLQualifiedName& optionTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[65];
const HTMLQualifiedName& pTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[66];
const HTMLQualifiedName& pictureTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[67];
const HTMLQualifiedName& plaintextTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[68];
const HTMLQualifiedName& preTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[69];
const HTMLQualifiedName& progressTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[70];
const HTMLQualifiedName& qTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[71];
const HTMLQualifiedName& sTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[72];
const HTMLQualifiedName& scriptTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[73];
const HTMLQualifiedName& sectionTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[74];
const HTMLQualifiedName& selectTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[75];
const HTMLQualifiedName& shadowTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[76];
const HTMLQualifiedName& slotTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[77];
const HTMLQualifiedName& smallTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[78];
const HTMLQualifiedName& spanTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[79];
const HTMLQualifiedName& strikeTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[80];
const HTMLQualifiedName& strongTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[81];
const HTMLQualifiedName& styleTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[82];
const HTMLQualifiedName& subTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[83];
const HTMLQualifiedName& summaryTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[84];
const HTMLQualifiedName& supTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[85];
const HTMLQualifiedName& tableTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[86];
const HTMLQualifiedName& tbodyTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[87];
const HTMLQualifiedName& tdTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[88];
const HTMLQualifiedName& templateTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[89];
const HTMLQualifiedName& textareaTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[90];
const HTMLQualifiedName& tfootTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[91];
const HTMLQualifiedName& thTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[92];
const HTMLQualifiedName& theadTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[93];
const HTMLQualifiedName& titleTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[94];
const HTMLQualifiedName& trTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[95];
const HTMLQualifiedName& ttTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[96];
const HTMLQualifiedName& uTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[97];
const HTMLQualifiedName& ulTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[98];
const HTMLQualifiedName& varTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[99];
const HTMLQualifiedName& wbrTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[100];
const HTMLQualifiedName& xmpTag = reinterpret_cast<HTMLQualifiedName*>(&TagStorage)[101];


PassOwnPtr<const HTMLQualifiedName*[]> getHTMLTags()
{
    OwnPtr<const HTMLQualifiedName*[]> tags = adoptArrayPtr(new const HTMLQualifiedName*[HTMLTagsCount]);
    for (size_t i = 0; i < HTMLTagsCount; i++)
        tags[i] = reinterpret_cast<HTMLQualifiedName*>(&TagStorage) + i;
    return tags.release();
}

// Attributes

void* AttrStorage[HTMLAttrsCount * ((sizeof(QualifiedName) + sizeof(void *) - 1) / sizeof(void *))];

const QualifiedName& abbrAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[0];
const QualifiedName& acceptAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[1];
const QualifiedName& accesskeyAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[2];
const QualifiedName& actionAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[3];
const QualifiedName& alignAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[4];
const QualifiedName& alinkAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[5];
const QualifiedName& altAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[6];
const QualifiedName& autocapitalizeAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[7];
const QualifiedName& autofocusAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[8];
const QualifiedName& axisAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[9];
const QualifiedName& backgroundAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[10];
const QualifiedName& bgcolorAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[11];
const QualifiedName& borderAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[12];
const QualifiedName& bordercolorAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[13];
const QualifiedName& cellpaddingAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[14];
const QualifiedName& cellspacingAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[15];
const QualifiedName& charsetAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[16];
const QualifiedName& checkedAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[17];
const QualifiedName& classAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[18];
const QualifiedName& clearAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[19];
const QualifiedName& colorAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[20];
const QualifiedName& colsAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[21];
const QualifiedName& colspanAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[22];
const QualifiedName& contentAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[23];
const QualifiedName& contenteditableAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[24];
const QualifiedName& contextmenuAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[25];
const QualifiedName& dirAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[26];
const QualifiedName& dirnameAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[27];
const QualifiedName& disabledAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[28];
const QualifiedName& draggableAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[29];
const QualifiedName& faceAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[30];
const QualifiedName& forAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[31];
const QualifiedName& formAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[32];
const QualifiedName& formactionAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[33];
const QualifiedName& formenctypeAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[34];
const QualifiedName& formmethodAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[35];
const QualifiedName& formnovalidateAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[36];
const QualifiedName& formtargetAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[37];
const QualifiedName& frameAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[38];
const QualifiedName& headersAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[39];
const QualifiedName& heightAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[40];
const QualifiedName& highAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[41];
const QualifiedName& hrefAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[42];
const QualifiedName& hspaceAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[43];
const QualifiedName& idAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[44];
const QualifiedName& incrementalAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[45];
const QualifiedName& isAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[46];
const QualifiedName& labelAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[47];
const QualifiedName& langAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[48];
const QualifiedName& languageAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[49];
const QualifiedName& leftmarginAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[50];
const QualifiedName& linkAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[51];
const QualifiedName& listAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[52];
const QualifiedName& lowAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[53];
const QualifiedName& marginheightAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[54];
const QualifiedName& marginwidthAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[55];
const QualifiedName& maxAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[56];
const QualifiedName& maxlengthAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[57];
const QualifiedName& mediaAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[58];
const QualifiedName& methodAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[59];
const QualifiedName& minAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[60];
const QualifiedName& minlengthAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[61];
const QualifiedName& multipleAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[62];
const QualifiedName& nameAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[63];
const QualifiedName& noshadeAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[64];
const QualifiedName& nowrapAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[65];
const QualifiedName& openAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[66];
const QualifiedName& optimumAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[67];
const QualifiedName& placeholderAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[68];
const QualifiedName& pseudoAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[69];
const QualifiedName& radiogroupAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[70];
const QualifiedName& readonlyAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[71];
const QualifiedName& relAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[72];
const QualifiedName& requiredAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[73];
const QualifiedName& reversedAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[74];
const QualifiedName& rowsAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[75];
const QualifiedName& rowspanAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[76];
const QualifiedName& rulesAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[77];
const QualifiedName& scopeAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[78];
const QualifiedName& selectAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[79];
const QualifiedName& selectedAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[80];
const QualifiedName& sizeAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[81];
const QualifiedName& slotAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[82];
const QualifiedName& spanAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[83];
const QualifiedName& srcAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[84];
const QualifiedName& srcsetAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[85];
const QualifiedName& startAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[86];
const QualifiedName& stepAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[87];
const QualifiedName& styleAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[88];
const QualifiedName& summaryAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[89];
const QualifiedName& tabindexAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[90];
const QualifiedName& targetAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[91];
const QualifiedName& textAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[92];
const QualifiedName& titleAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[93];
const QualifiedName& topmarginAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[94];
const QualifiedName& typeAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[95];
const QualifiedName& valignAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[96];
const QualifiedName& valueAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[97];
const QualifiedName& vlinkAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[98];
const QualifiedName& vspaceAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[99];
const QualifiedName& widthAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[100];
const QualifiedName& wrapAttr = reinterpret_cast<QualifiedName*>(&AttrStorage)[101];



void init()
{
    struct NameEntry {
        const char* name;
        unsigned hash;
        unsigned char length;
        unsigned char isTag;
        unsigned char isAttr;
    };

    // Use placement new to initialize the globals.
    AtomicString xhtmlNS("http://www.w3.org/1999/xhtml", AtomicString::ConstructFromLiteral);

    // Namespace
    new ((void*)&xhtmlNamespaceURI) AtomicString(xhtmlNS);
    static const NameEntry kNames[] = {
        { "a", 9778235, 1, 1, 0 },
        { "abbr", 650525, 4, 0, 1 },
        { "accept", 4839857, 6, 0, 1 },
        { "accesskey", 15931860, 9, 0, 1 },
        { "action", 14878034, 6, 0, 1 },
        { "address", 10008206, 7, 1, 0 },
        { "align", 10094397, 5, 0, 1 },
        { "alink", 2408650, 5, 0, 1 },
        { "alt", 15335609, 3, 0, 1 },
        { "area", 7355486, 4, 1, 0 },
        { "article", 2968800, 7, 1, 0 },
        { "aside", 10128566, 5, 1, 0 },
        { "autocapitalize", 15613526, 14, 0, 1 },
        { "autofocus", 16282013, 9, 0, 1 },
        { "axis", 14347904, 4, 0, 1 },
        { "b", 7170995, 1, 1, 0 },
        { "background", 8107606, 10, 0, 1 },
        { "base", 4376626, 4, 1, 0 },
        { "basefont", 5439393, 8, 1, 0 },
        { "bdi", 14809812, 3, 1, 0 },
        { "bgcolor", 11074228, 7, 0, 1 },
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
        { "col", 12850806, 3, 1, 0 },
        { "colgroup", 3733719, 8, 1, 0 },
        { "color", 2734929, 5, 0, 1 },
        { "cols", 4921975, 4, 0, 1 },
        { "colspan", 9715319, 7, 0, 1 },
        { "command", 2099501, 7, 1, 0 },
        { "content", 6894765, 7, 1, 1 },
        { "contenteditable", 1577657, 15, 0, 1 },
        { "contextmenu", 14578063, 11, 0, 1 },
        { "datalist", 7058973, 8, 1, 0 },
        { "dd", 16480447, 2, 1, 0 },
        { "details", 4653180, 7, 1, 0 },
        { "dir", 5122803, 3, 1, 1 },
        { "dirname", 9899348, 7, 0, 1 },
        { "disabled", 12237545, 8, 0, 1 },
        { "div", 9762591, 3, 1, 0 },
        { "dl", 5117854, 2, 1, 0 },
        { "draggable", 9281278, 9, 0, 1 },
        { "dt", 8177126, 2, 1, 0 },
        { "em", 2724661, 2, 1, 0 },
        { "face", 12427611, 4, 0, 1 },
        { "fieldset", 37097, 8, 1, 0 },
        { "figcaption", 13388641, 10, 1, 0 },
        { "figure", 13776479, 6, 1, 0 },
        { "font", 4588695, 4, 1, 0 },
        { "footer", 13359933, 6, 1, 0 },
        { "for", 4145292, 3, 0, 1 },
        { "form", 10060301, 4, 1, 1 },
        { "formaction", 12667227, 10, 0, 1 },
        { "formenctype", 6334991, 11, 0, 1 },
        { "formmethod", 7653832, 10, 0, 1 },
        { "formnovalidate", 5722173, 14, 0, 1 },
        { "formtarget", 15594141, 10, 0, 1 },
        { "frame", 3408403, 5, 0, 1 },
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
        { "high", 11748844, 4, 0, 1 },
        { "hr", 7182703, 2, 1, 0 },
        { "href", 5797448, 4, 0, 1 },
        { "hspace", 4182216, 6, 0, 1 },
        { "html", 7123128, 4, 1, 0 },
        { "i", 16149244, 1, 1, 0 },
        { "id", 6145549, 2, 0, 1 },
        { "image", 10287573, 5, 1, 0 },
        { "img", 11398162, 3, 1, 0 },
        { "incremental", 9277578, 11, 0, 1 },
        { "input", 10365436, 5, 1, 0 },
        { "is", 15617733, 2, 0, 1 },
        { "label", 15092473, 5, 1, 1 },
        { "lang", 3702417, 4, 0, 1 },
        { "language", 4109061, 8, 0, 1 },
        { "leftmargin", 10359506, 10, 0, 1 },
        { "legend", 16417202, 6, 1, 0 },
        { "li", 11097939, 2, 1, 0 },
        { "link", 7010491, 4, 1, 1 },
        { "list", 15078159, 4, 0, 1 },
        { "listing", 15674586, 7, 1, 0 },
        { "low", 9523344, 3, 0, 1 },
        { "main", 13501767, 4, 1, 0 },
        { "marginheight", 14117911, 12, 0, 1 },
        { "marginwidth", 5458369, 11, 0, 1 },
        { "marquee", 1416619, 7, 1, 0 },
        { "max", 3449166, 3, 0, 1 },
        { "maxlength", 5985164, 9, 0, 1 },
        { "media", 13905581, 5, 0, 1 },
        { "menu", 13428288, 4, 1, 0 },
        { "menuitem", 3687436, 8, 1, 0 },
        { "meta", 14067019, 4, 1, 0 },
        { "meter", 5815470, 5, 1, 0 },
        { "method", 14679268, 6, 0, 1 },
        { "min", 2196278, 3, 0, 1 },
        { "minlength", 4633736, 9, 0, 1 },
        { "multiple", 6185486, 8, 0, 1 },
        { "name", 8774809, 4, 0, 1 },
        { "nav", 15269619, 3, 1, 0 },
        { "nobr", 419763, 4, 1, 0 },
        { "noshade", 12172860, 7, 0, 1 },
        { "nowrap", 15804466, 6, 0, 1 },
        { "ol", 6672076, 2, 1, 0 },
        { "open", 13703631, 4, 0, 1 },
        { "optgroup", 10180907, 8, 1, 0 },
        { "optimum", 3350287, 7, 0, 1 },
        { "option", 2998548, 6, 1, 0 },
        { "p", 587733, 1, 1, 0 },
        { "picture", 7136372, 7, 1, 0 },
        { "placeholder", 6132349, 11, 0, 1 },
        { "plaintext", 2551274, 9, 1, 0 },
        { "pre", 16061734, 3, 1, 0 },
        { "progress", 11461517, 8, 1, 0 },
        { "pseudo", 6923620, 6, 0, 1 },
        { "q", 12607837, 1, 1, 0 },
        { "radiogroup", 3065371, 10, 0, 1 },
        { "readonly", 4471832, 8, 0, 1 },
        { "rel", 8963158, 3, 0, 1 },
        { "required", 4894573, 8, 0, 1 },
        { "reversed", 15723650, 8, 0, 1 },
        { "rows", 11770835, 4, 0, 1 },
        { "rowspan", 13630419, 7, 0, 1 },
        { "rules", 8604373, 5, 0, 1 },
        { "s", 11249530, 1, 1, 0 },
        { "scope", 5624816, 5, 0, 1 },
        { "script", 7137273, 6, 1, 0 },
        { "section", 8056235, 7, 1, 0 },
        { "select", 210571, 6, 1, 1 },
        { "selected", 15762609, 8, 0, 1 },
        { "shadow", 1972219, 6, 1, 0 },
        { "size", 12715024, 4, 0, 1 },
        { "slot", 8744197, 4, 1, 1 },
        { "small", 14362904, 5, 1, 0 },
        { "span", 11168892, 4, 1, 1 },
        { "src", 11517827, 3, 0, 1 },
        { "srcset", 6236095, 6, 0, 1 },
        { "start", 1021290, 5, 0, 1 },
        { "step", 5337026, 4, 0, 1 },
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
        { "topmargin", 10843382, 9, 0, 1 },
        { "tr", 16624306, 2, 1, 0 },
        { "tt", 9170852, 2, 1, 0 },
        { "type", 1916283, 4, 0, 1 },
        { "u", 13317144, 1, 1, 0 },
        { "ul", 12324505, 2, 1, 0 },
        { "valign", 8345445, 6, 0, 1 },
        { "value", 10871467, 5, 0, 1 },
        { "var", 9573901, 3, 1, 0 },
        { "vlink", 12743618, 5, 0, 1 },
        { "vspace", 9862276, 6, 0, 1 },
        { "wbr", 8290101, 3, 1, 0 },
        { "width", 12902275, 5, 0, 1 },
        { "wrap", 7601899, 4, 0, 1 },
        { "xmp", 8764488, 3, 1, 0 },
    };

    size_t tag_i = 0;
    size_t attr_i = 0;
    for (size_t i = 0; i < WTF_ARRAY_LENGTH(kNames); i++) {
        StringImpl* stringImpl = StringImpl::createStatic(kNames[i].name, kNames[i].length, kNames[i].hash);
        if (kNames[i].isTag) {
            void* address = reinterpret_cast<HTMLQualifiedName*>(&TagStorage) + tag_i;
            QualifiedName::createStatic(address, stringImpl, xhtmlNS);
            tag_i++;
        }

        if (!kNames[i].isAttr)
            continue;
        void* address = reinterpret_cast<QualifiedName*>(&AttrStorage) + attr_i;
        QualifiedName::createStatic(address, stringImpl);
        attr_i++;
    }
    ASSERT(tag_i == HTMLTagsCount);
    ASSERT(attr_i == HTMLAttrsCount);
}

} // HTML
} // namespace blink
