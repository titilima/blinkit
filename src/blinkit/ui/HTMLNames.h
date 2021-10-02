// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef HTMLNames_h
#define HTMLNames_h

#include "core/CoreExport.h"
#include "core/dom/QualifiedName.h"
#include "wtf/PassOwnPtr.h"

namespace blink {

class HTMLQualifiedName : public QualifiedName { };

namespace HTMLNames {

// Namespace
CORE_EXPORT extern const WTF::AtomicString& xhtmlNamespaceURI;

// Tags
CORE_EXPORT extern const blink::HTMLQualifiedName& addressTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& articleTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& asideTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& bTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& basefontTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& bigTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& centerTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& codeTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& commandTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& ddTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& dtTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& emTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& figcaptionTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& figureTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& footerTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& headerTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& hgroupTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& iTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& mainTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& navTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& nobrTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& plaintextTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& sTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& sectionTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& smallTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& strikeTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& strongTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& subTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& supTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& ttTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& uTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& varTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& aTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& areaTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& brTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& baseTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& bodyTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& contentTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& dlTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& datalistTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& detailsTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& dirTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& divTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& fontTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& formTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& hrTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& headTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& h1Tag;
CORE_EXPORT extern const blink::HTMLQualifiedName& h2Tag;
CORE_EXPORT extern const blink::HTMLQualifiedName& h3Tag;
CORE_EXPORT extern const blink::HTMLQualifiedName& h4Tag;
CORE_EXPORT extern const blink::HTMLQualifiedName& h5Tag;
CORE_EXPORT extern const blink::HTMLQualifiedName& h6Tag;
CORE_EXPORT extern const blink::HTMLQualifiedName& htmlTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& liTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& legendTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& marqueeTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& menuTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& menuitemTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& metaTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& meterTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& olTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& optgroupTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& optionTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& pTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& pictureTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& preTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& listingTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& xmpTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& progressTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& blockquoteTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& qTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& shadowTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& slotTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& spanTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& captionTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& tdTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& thTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& colTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& colgroupTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& tableTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& trTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& tbodyTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& tfootTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& theadTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& templateTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& titleTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& ulTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& imageTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& buttonTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& fieldsetTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& labelTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& selectTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& textareaTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& linkTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& scriptTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& styleTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& imgTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& inputTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& bdiTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& summaryTag;
CORE_EXPORT extern const blink::HTMLQualifiedName& wbrTag;

// Attributes
CORE_EXPORT extern const blink::QualifiedName& abbrAttr;
CORE_EXPORT extern const blink::QualifiedName& acceptAttr;
CORE_EXPORT extern const blink::QualifiedName& accesskeyAttr;
CORE_EXPORT extern const blink::QualifiedName& actionAttr;
CORE_EXPORT extern const blink::QualifiedName& alignAttr;
CORE_EXPORT extern const blink::QualifiedName& alinkAttr;
CORE_EXPORT extern const blink::QualifiedName& altAttr;
CORE_EXPORT extern const blink::QualifiedName& autocapitalizeAttr;
CORE_EXPORT extern const blink::QualifiedName& autofocusAttr;
CORE_EXPORT extern const blink::QualifiedName& axisAttr;
CORE_EXPORT extern const blink::QualifiedName& backgroundAttr;
CORE_EXPORT extern const blink::QualifiedName& bgcolorAttr;
CORE_EXPORT extern const blink::QualifiedName& borderAttr;
CORE_EXPORT extern const blink::QualifiedName& bordercolorAttr;
CORE_EXPORT extern const blink::QualifiedName& cellpaddingAttr;
CORE_EXPORT extern const blink::QualifiedName& cellspacingAttr;
CORE_EXPORT extern const blink::QualifiedName& charsetAttr;
CORE_EXPORT extern const blink::QualifiedName& checkedAttr;
CORE_EXPORT extern const blink::QualifiedName& classAttr;
CORE_EXPORT extern const blink::QualifiedName& clearAttr;
CORE_EXPORT extern const blink::QualifiedName& colorAttr;
CORE_EXPORT extern const blink::QualifiedName& colsAttr;
CORE_EXPORT extern const blink::QualifiedName& colspanAttr;
CORE_EXPORT extern const blink::QualifiedName& contentAttr;
CORE_EXPORT extern const blink::QualifiedName& contenteditableAttr;
CORE_EXPORT extern const blink::QualifiedName& contextmenuAttr;
CORE_EXPORT extern const blink::QualifiedName& dirAttr;
CORE_EXPORT extern const blink::QualifiedName& dirnameAttr;
CORE_EXPORT extern const blink::QualifiedName& disabledAttr;
CORE_EXPORT extern const blink::QualifiedName& draggableAttr;
CORE_EXPORT extern const blink::QualifiedName& faceAttr;
CORE_EXPORT extern const blink::QualifiedName& forAttr;
CORE_EXPORT extern const blink::QualifiedName& formAttr;
CORE_EXPORT extern const blink::QualifiedName& formactionAttr;
CORE_EXPORT extern const blink::QualifiedName& formenctypeAttr;
CORE_EXPORT extern const blink::QualifiedName& formmethodAttr;
CORE_EXPORT extern const blink::QualifiedName& formnovalidateAttr;
CORE_EXPORT extern const blink::QualifiedName& formtargetAttr;
CORE_EXPORT extern const blink::QualifiedName& frameAttr;
CORE_EXPORT extern const blink::QualifiedName& headersAttr;
CORE_EXPORT extern const blink::QualifiedName& heightAttr;
CORE_EXPORT extern const blink::QualifiedName& hiddenAttr;
CORE_EXPORT extern const blink::QualifiedName& highAttr;
CORE_EXPORT extern const blink::QualifiedName& hrefAttr;
CORE_EXPORT extern const blink::QualifiedName& hspaceAttr;
CORE_EXPORT extern const blink::QualifiedName& http_equivAttr;
CORE_EXPORT extern const blink::QualifiedName& idAttr;
CORE_EXPORT extern const blink::QualifiedName& incrementalAttr;
CORE_EXPORT extern const blink::QualifiedName& isAttr;
CORE_EXPORT extern const blink::QualifiedName& labelAttr;
CORE_EXPORT extern const blink::QualifiedName& langAttr;
CORE_EXPORT extern const blink::QualifiedName& languageAttr;
CORE_EXPORT extern const blink::QualifiedName& leftmarginAttr;
CORE_EXPORT extern const blink::QualifiedName& linkAttr;
CORE_EXPORT extern const blink::QualifiedName& listAttr;
CORE_EXPORT extern const blink::QualifiedName& lowAttr;
CORE_EXPORT extern const blink::QualifiedName& marginheightAttr;
CORE_EXPORT extern const blink::QualifiedName& marginwidthAttr;
CORE_EXPORT extern const blink::QualifiedName& maxAttr;
CORE_EXPORT extern const blink::QualifiedName& maxlengthAttr;
CORE_EXPORT extern const blink::QualifiedName& mediaAttr;
CORE_EXPORT extern const blink::QualifiedName& methodAttr;
CORE_EXPORT extern const blink::QualifiedName& minAttr;
CORE_EXPORT extern const blink::QualifiedName& minlengthAttr;
CORE_EXPORT extern const blink::QualifiedName& multipleAttr;
CORE_EXPORT extern const blink::QualifiedName& nameAttr;
CORE_EXPORT extern const blink::QualifiedName& noshadeAttr;
CORE_EXPORT extern const blink::QualifiedName& nowrapAttr;
CORE_EXPORT extern const blink::QualifiedName& openAttr;
CORE_EXPORT extern const blink::QualifiedName& optimumAttr;
CORE_EXPORT extern const blink::QualifiedName& patternAttr;
CORE_EXPORT extern const blink::QualifiedName& placeholderAttr;
CORE_EXPORT extern const blink::QualifiedName& pseudoAttr;
CORE_EXPORT extern const blink::QualifiedName& radiogroupAttr;
CORE_EXPORT extern const blink::QualifiedName& readonlyAttr;
CORE_EXPORT extern const blink::QualifiedName& relAttr;
CORE_EXPORT extern const blink::QualifiedName& requiredAttr;
CORE_EXPORT extern const blink::QualifiedName& reversedAttr;
CORE_EXPORT extern const blink::QualifiedName& rowsAttr;
CORE_EXPORT extern const blink::QualifiedName& rowspanAttr;
CORE_EXPORT extern const blink::QualifiedName& rulesAttr;
CORE_EXPORT extern const blink::QualifiedName& scopeAttr;
CORE_EXPORT extern const blink::QualifiedName& selectAttr;
CORE_EXPORT extern const blink::QualifiedName& selectedAttr;
CORE_EXPORT extern const blink::QualifiedName& sizeAttr;
CORE_EXPORT extern const blink::QualifiedName& sizesAttr;
CORE_EXPORT extern const blink::QualifiedName& slotAttr;
CORE_EXPORT extern const blink::QualifiedName& spanAttr;
CORE_EXPORT extern const blink::QualifiedName& srcAttr;
CORE_EXPORT extern const blink::QualifiedName& srcsetAttr;
CORE_EXPORT extern const blink::QualifiedName& startAttr;
CORE_EXPORT extern const blink::QualifiedName& stepAttr;
CORE_EXPORT extern const blink::QualifiedName& styleAttr;
CORE_EXPORT extern const blink::QualifiedName& summaryAttr;
CORE_EXPORT extern const blink::QualifiedName& tabindexAttr;
CORE_EXPORT extern const blink::QualifiedName& targetAttr;
CORE_EXPORT extern const blink::QualifiedName& textAttr;
CORE_EXPORT extern const blink::QualifiedName& titleAttr;
CORE_EXPORT extern const blink::QualifiedName& topmarginAttr;
CORE_EXPORT extern const blink::QualifiedName& typeAttr;
CORE_EXPORT extern const blink::QualifiedName& usemapAttr;
CORE_EXPORT extern const blink::QualifiedName& valignAttr;
CORE_EXPORT extern const blink::QualifiedName& valueAttr;
CORE_EXPORT extern const blink::QualifiedName& vlinkAttr;
CORE_EXPORT extern const blink::QualifiedName& vspaceAttr;
CORE_EXPORT extern const blink::QualifiedName& webkitdirectoryAttr;
CORE_EXPORT extern const blink::QualifiedName& widthAttr;
CORE_EXPORT extern const blink::QualifiedName& wrapAttr;

const unsigned HTMLTagsCount = 102;
CORE_EXPORT PassOwnPtr<const HTMLQualifiedName*[]> getHTMLTags();

const unsigned HTMLAttrsCount = 108;

void init();

} // HTMLNames
} // namespace blink

#endif
