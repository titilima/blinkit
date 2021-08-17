// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "HTMLElementFactory.h"

#include "HTMLNames.h"
#include "core/html/HTMLAnchorElement.h"
#include "core/html/HTMLAreaElement.h"
#include "core/html/HTMLBDIElement.h"
#include "core/html/HTMLBRElement.h"
#include "core/html/HTMLBaseElement.h"
#include "core/html/HTMLBodyElement.h"
#include "core/html/HTMLButtonElement.h"
#include "core/html/HTMLContentElement.h"
#include "core/html/HTMLDListElement.h"
#include "core/html/HTMLDataListElement.h"
#include "core/html/HTMLDetailsElement.h"
#include "core/html/HTMLDirectoryElement.h"
#include "core/html/HTMLDivElement.h"
#include "core/html/HTMLElement.h"
#include "core/html/HTMLFieldSetElement.h"
#include "core/html/HTMLFontElement.h"
#include "core/html/HTMLFormElement.h"
#include "core/html/HTMLHRElement.h"
#include "core/html/HTMLHeadElement.h"
#include "core/html/HTMLHeadingElement.h"
#include "core/html/HTMLHtmlElement.h"
#include "core/html/HTMLImageElement.h"
#include "core/html/HTMLInputElement.h"
#include "core/html/HTMLLIElement.h"
#include "core/html/HTMLLabelElement.h"
#include "core/html/HTMLLegendElement.h"
#include "core/html/HTMLLinkElement.h"
#include "core/html/HTMLMarqueeElement.h"
#include "core/html/HTMLMenuElement.h"
#include "core/html/HTMLMenuItemElement.h"
#include "core/html/HTMLMetaElement.h"
#include "core/html/HTMLMeterElement.h"
#include "core/html/HTMLOListElement.h"
#include "core/html/HTMLOptGroupElement.h"
#include "core/html/HTMLOptionElement.h"
#include "core/html/HTMLParagraphElement.h"
#include "core/html/HTMLPictureElement.h"
#include "core/html/HTMLPreElement.h"
#include "core/html/HTMLProgressElement.h"
#include "core/html/HTMLQuoteElement.h"
#include "core/html/HTMLScriptElement.h"
#include "core/html/HTMLSelectElement.h"
#include "core/html/HTMLShadowElement.h"
#include "core/html/HTMLSlotElement.h"
#include "core/html/HTMLSpanElement.h"
#include "core/html/HTMLStyleElement.h"
#include "core/html/HTMLSummaryElement.h"
#include "core/html/HTMLTableCaptionElement.h"
#include "core/html/HTMLTableCellElement.h"
#include "core/html/HTMLTableColElement.h"
#include "core/html/HTMLTableElement.h"
#include "core/html/HTMLTableRowElement.h"
#include "core/html/HTMLTableSectionElement.h"
#include "core/html/HTMLTemplateElement.h"
#include "core/html/HTMLTextAreaElement.h"
#include "core/html/HTMLTitleElement.h"
#include "core/html/HTMLUListElement.h"
#include "core/html/HTMLUnknownElement.h"
#include "core/html/HTMLWBRElement.h"
#include "core/html/HTMLUnknownElement.h"
#include "core/dom/custom/CustomElement.h"
#include "core/dom/custom/CustomElementRegistrationContext.h"
#include "core/dom/Document.h"
#include "core/frame/Settings.h"
#include "platform/RuntimeEnabledFeatures.h"
#include "wtf/HashMap.h"

namespace blink {

using namespace HTMLNames;

typedef PassRefPtrWillBeRawPtr<HTMLElement> (*ConstructorFunction)(
    Document&,
    HTMLFormElement*,
    bool createdByParser);

typedef HashMap<AtomicString, ConstructorFunction> FunctionMap;

static FunctionMap* g_constructors = 0;

static PassRefPtrWillBeRawPtr<HTMLElement> addressConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(addressTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> articleConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(articleTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> asideConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(asideTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> bConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(bTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> basefontConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(basefontTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> bigConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(bigTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> centerConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(centerTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> codeConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(codeTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> commandConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(commandTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> ddConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(ddTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> dtConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(dtTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> emConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(emTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> figcaptionConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(figcaptionTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> figureConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(figureTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> footerConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(footerTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> headerConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(headerTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> hgroupConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(hgroupTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> iConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(iTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> mainConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(mainTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> navConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(navTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> nobrConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(nobrTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> plaintextConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(plaintextTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> sConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(sTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> sectionConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(sectionTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> smallConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(smallTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> strikeConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(strikeTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> strongConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(strongTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> subConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(subTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> supConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(supTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> ttConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(ttTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> uConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(uTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> varConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(varTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> aConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLAnchorElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> areaConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLAreaElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> brConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLBRElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> baseConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLBaseElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> bodyConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLBodyElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> contentConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLContentElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> dlConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLDListElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> datalistConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLDataListElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> detailsConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLDetailsElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> dirConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLDirectoryElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> divConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLDivElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> fontConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLFontElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> formConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLFormElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> hrConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLHRElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> headConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLHeadElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> h1Constructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLHeadingElement::create(h1Tag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> h2Constructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLHeadingElement::create(h2Tag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> h3Constructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLHeadingElement::create(h3Tag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> h4Constructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLHeadingElement::create(h4Tag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> h5Constructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLHeadingElement::create(h5Tag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> h6Constructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLHeadingElement::create(h6Tag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> htmlConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLHtmlElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> liConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLLIElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> legendConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLLegendElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> marqueeConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLMarqueeElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> menuConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLMenuElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> menuitemConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    if (!RuntimeEnabledFeatures::contextMenuEnabled())
        return HTMLUnknownElement::create(menuitemTag, document);
    return HTMLMenuItemElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> metaConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLMetaElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> meterConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLMeterElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> olConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLOListElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> optgroupConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLOptGroupElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> optionConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLOptionElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> pConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLParagraphElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> pictureConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLPictureElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> preConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLPreElement::create(preTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> listingConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLPreElement::create(listingTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> xmpConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLPreElement::create(xmpTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> progressConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLProgressElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> blockquoteConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLQuoteElement::create(blockquoteTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> qConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLQuoteElement::create(qTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> shadowConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLShadowElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> slotConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLSlotElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> spanConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLSpanElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> captionConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLTableCaptionElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> tdConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLTableCellElement::create(tdTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> thConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLTableCellElement::create(thTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> colConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLTableColElement::create(colTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> colgroupConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLTableColElement::create(colgroupTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> tableConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLTableElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> trConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLTableRowElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> tbodyConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLTableSectionElement::create(tbodyTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> tfootConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLTableSectionElement::create(tfootTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> theadConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLTableSectionElement::create(theadTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> templateConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLTemplateElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> titleConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLTitleElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> ulConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLUListElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> imageConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLUnknownElement::create(imageTag, document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> buttonConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLButtonElement::create(document, formElement);
}
static PassRefPtrWillBeRawPtr<HTMLElement> fieldsetConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLFieldSetElement::create(document, formElement);
}
static PassRefPtrWillBeRawPtr<HTMLElement> labelConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLLabelElement::create(document, formElement);
}
static PassRefPtrWillBeRawPtr<HTMLElement> selectConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLSelectElement::create(document, formElement);
}
static PassRefPtrWillBeRawPtr<HTMLElement> textareaConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLTextAreaElement::create(document, formElement);
}
static PassRefPtrWillBeRawPtr<HTMLElement> linkConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLLinkElement::create(document, createdByParser);
}
static PassRefPtrWillBeRawPtr<HTMLElement> scriptConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLScriptElement::create(document, createdByParser);
}
static PassRefPtrWillBeRawPtr<HTMLElement> styleConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLStyleElement::create(document, createdByParser);
}
static PassRefPtrWillBeRawPtr<HTMLElement> imgConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLImageElement::create(document, formElement, createdByParser);
}
static PassRefPtrWillBeRawPtr<HTMLElement> inputConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLInputElement::create(document, formElement, createdByParser);
}
static PassRefPtrWillBeRawPtr<HTMLElement> bdiConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLBDIElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> summaryConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLSummaryElement::create(document);
}
static PassRefPtrWillBeRawPtr<HTMLElement> wbrConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLWBRElement::create(document);
}

struct CreateHTMLFunctionMapData {
  const QualifiedName& tag;
  ConstructorFunction func;
};

static void createHTMLFunctionMap()
{
    ASSERT(!g_constructors);
    g_constructors = new FunctionMap;
    // Empty array initializer lists are illegal [dcl.init.aggr] and will not
    // compile in MSVC. If tags list is empty, add check to skip this.
    static const CreateHTMLFunctionMapData data[] = {
        { addressTag, addressConstructor },
        { articleTag, articleConstructor },
        { asideTag, asideConstructor },
        { bTag, bConstructor },
        { basefontTag, basefontConstructor },
        { bigTag, bigConstructor },
        { centerTag, centerConstructor },
        { codeTag, codeConstructor },
        { commandTag, commandConstructor },
        { ddTag, ddConstructor },
        { dtTag, dtConstructor },
        { emTag, emConstructor },
        { figcaptionTag, figcaptionConstructor },
        { figureTag, figureConstructor },
        { footerTag, footerConstructor },
        { headerTag, headerConstructor },
        { hgroupTag, hgroupConstructor },
        { iTag, iConstructor },
        { mainTag, mainConstructor },
        { navTag, navConstructor },
        { nobrTag, nobrConstructor },
        { plaintextTag, plaintextConstructor },
        { sTag, sConstructor },
        { sectionTag, sectionConstructor },
        { smallTag, smallConstructor },
        { strikeTag, strikeConstructor },
        { strongTag, strongConstructor },
        { subTag, subConstructor },
        { supTag, supConstructor },
        { ttTag, ttConstructor },
        { uTag, uConstructor },
        { varTag, varConstructor },
        { aTag, aConstructor },
        { areaTag, areaConstructor },
        { brTag, brConstructor },
        { baseTag, baseConstructor },
        { bodyTag, bodyConstructor },
        { contentTag, contentConstructor },
        { dlTag, dlConstructor },
        { datalistTag, datalistConstructor },
        { detailsTag, detailsConstructor },
        { dirTag, dirConstructor },
        { divTag, divConstructor },
        { fontTag, fontConstructor },
        { formTag, formConstructor },
        { hrTag, hrConstructor },
        { headTag, headConstructor },
        { h1Tag, h1Constructor },
        { h2Tag, h2Constructor },
        { h3Tag, h3Constructor },
        { h4Tag, h4Constructor },
        { h5Tag, h5Constructor },
        { h6Tag, h6Constructor },
        { htmlTag, htmlConstructor },
        { liTag, liConstructor },
        { legendTag, legendConstructor },
        { marqueeTag, marqueeConstructor },
        { menuTag, menuConstructor },
        { menuitemTag, menuitemConstructor },
        { metaTag, metaConstructor },
        { meterTag, meterConstructor },
        { olTag, olConstructor },
        { optgroupTag, optgroupConstructor },
        { optionTag, optionConstructor },
        { pTag, pConstructor },
        { pictureTag, pictureConstructor },
        { preTag, preConstructor },
        { listingTag, listingConstructor },
        { xmpTag, xmpConstructor },
        { progressTag, progressConstructor },
        { blockquoteTag, blockquoteConstructor },
        { qTag, qConstructor },
        { shadowTag, shadowConstructor },
        { slotTag, slotConstructor },
        { spanTag, spanConstructor },
        { captionTag, captionConstructor },
        { tdTag, tdConstructor },
        { thTag, thConstructor },
        { colTag, colConstructor },
        { colgroupTag, colgroupConstructor },
        { tableTag, tableConstructor },
        { trTag, trConstructor },
        { tbodyTag, tbodyConstructor },
        { tfootTag, tfootConstructor },
        { theadTag, theadConstructor },
        { templateTag, templateConstructor },
        { titleTag, titleConstructor },
        { ulTag, ulConstructor },
        { imageTag, imageConstructor },
        { buttonTag, buttonConstructor },
        { fieldsetTag, fieldsetConstructor },
        { labelTag, labelConstructor },
        { selectTag, selectConstructor },
        { textareaTag, textareaConstructor },
        { linkTag, linkConstructor },
        { scriptTag, scriptConstructor },
        { styleTag, styleConstructor },
        { imgTag, imgConstructor },
        { inputTag, inputConstructor },
        { bdiTag, bdiConstructor },
        { summaryTag, summaryConstructor },
        { wbrTag, wbrConstructor },
    };
    for (size_t i = 0; i < WTF_ARRAY_LENGTH(data); i++)
        g_constructors->set(data[i].tag.localName(), data[i].func);
}

PassRefPtrWillBeRawPtr<HTMLElement> HTMLElementFactory::createHTMLElement(
    const AtomicString& localName,
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    if (!g_constructors)
        createHTMLFunctionMap();
    if (ConstructorFunction function = g_constructors->get(localName))
        return function(document, formElement, createdByParser);

    if (document.registrationContext() && CustomElement::isValidName(localName)) {
        RefPtrWillBeRawPtr<Element> element = document.registrationContext()->createCustomTagElement(document, QualifiedName(nullAtom, localName, xhtmlNamespaceURI));
        ASSERT_WITH_SECURITY_IMPLICATION(element->isHTMLElement());
        return static_pointer_cast<HTMLElement>(element.release());
    }

    return HTMLUnknownElement::create(QualifiedName(nullAtom, localName, xhtmlNamespaceURI), document);
}

} // namespace blink
