// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "./HTMLElementFactory.h"

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

namespace blink {

using namespace HTMLNames;

typedef GCRefPtr<HTMLElement> (*ConstructorFunction)(
    Document&,
    HTMLFormElement*,
    bool createdByParser);

typedef std::unordered_map<AtomicString, ConstructorFunction> FunctionMap;

static FunctionMap* g_constructors = 0;

static GCRefPtr<HTMLElement> addressConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(addressTag, document);
}
static GCRefPtr<HTMLElement> articleConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(articleTag, document);
}
static GCRefPtr<HTMLElement> asideConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(asideTag, document);
}
static GCRefPtr<HTMLElement> bConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(bTag, document);
}
static GCRefPtr<HTMLElement> basefontConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(basefontTag, document);
}
static GCRefPtr<HTMLElement> bigConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(bigTag, document);
}
static GCRefPtr<HTMLElement> centerConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(centerTag, document);
}
static GCRefPtr<HTMLElement> codeConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(codeTag, document);
}
static GCRefPtr<HTMLElement> commandConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(commandTag, document);
}
static GCRefPtr<HTMLElement> ddConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(ddTag, document);
}
static GCRefPtr<HTMLElement> dtConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(dtTag, document);
}
static GCRefPtr<HTMLElement> emConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(emTag, document);
}
static GCRefPtr<HTMLElement> figcaptionConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(figcaptionTag, document);
}
static GCRefPtr<HTMLElement> figureConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(figureTag, document);
}
static GCRefPtr<HTMLElement> footerConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(footerTag, document);
}
static GCRefPtr<HTMLElement> headerConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(headerTag, document);
}
static GCRefPtr<HTMLElement> hgroupConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(hgroupTag, document);
}
static GCRefPtr<HTMLElement> iConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(iTag, document);
}
static GCRefPtr<HTMLElement> mainConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(mainTag, document);
}
static GCRefPtr<HTMLElement> navConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(navTag, document);
}
static GCRefPtr<HTMLElement> nobrConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(nobrTag, document);
}
static GCRefPtr<HTMLElement> plaintextConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(plaintextTag, document);
}
static GCRefPtr<HTMLElement> sConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(sTag, document);
}
static GCRefPtr<HTMLElement> sectionConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(sectionTag, document);
}
static GCRefPtr<HTMLElement> smallConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(smallTag, document);
}
static GCRefPtr<HTMLElement> strikeConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(strikeTag, document);
}
static GCRefPtr<HTMLElement> strongConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(strongTag, document);
}
static GCRefPtr<HTMLElement> subConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(subTag, document);
}
static GCRefPtr<HTMLElement> supConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(supTag, document);
}
static GCRefPtr<HTMLElement> ttConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(ttTag, document);
}
static GCRefPtr<HTMLElement> uConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(uTag, document);
}
static GCRefPtr<HTMLElement> varConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLElement::create(varTag, document);
}
static GCRefPtr<HTMLElement> aConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLAnchorElement::create(document);
}
static GCRefPtr<HTMLElement> areaConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLAreaElement::create(document);
}
static GCRefPtr<HTMLElement> brConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLBRElement::create(document);
}
static GCRefPtr<HTMLElement> baseConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLBaseElement::create(document);
}
static GCRefPtr<HTMLElement> bodyConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLBodyElement::create(document);
}
static GCRefPtr<HTMLElement> contentConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLContentElement::create(document);
}
static GCRefPtr<HTMLElement> dlConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLDListElement::create(document);
}
static GCRefPtr<HTMLElement> datalistConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLDataListElement::create(document);
}
static GCRefPtr<HTMLElement> detailsConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLDetailsElement::create(document);
}
static GCRefPtr<HTMLElement> dirConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLDirectoryElement::create(document);
}
static GCRefPtr<HTMLElement> divConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLDivElement::create(document);
}
static GCRefPtr<HTMLElement> fontConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLFontElement::create(document);
}
static GCRefPtr<HTMLElement> formConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLFormElement::create(document);
}
static GCRefPtr<HTMLElement> hrConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLHRElement::create(document);
}
static GCRefPtr<HTMLElement> headConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLHeadElement::create(document);
}
static GCRefPtr<HTMLElement> h1Constructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLHeadingElement::create(h1Tag, document);
}
static GCRefPtr<HTMLElement> h2Constructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLHeadingElement::create(h2Tag, document);
}
static GCRefPtr<HTMLElement> h3Constructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLHeadingElement::create(h3Tag, document);
}
static GCRefPtr<HTMLElement> h4Constructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLHeadingElement::create(h4Tag, document);
}
static GCRefPtr<HTMLElement> h5Constructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLHeadingElement::create(h5Tag, document);
}
static GCRefPtr<HTMLElement> h6Constructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLHeadingElement::create(h6Tag, document);
}
static GCRefPtr<HTMLElement> htmlConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLHtmlElement::create(document);
}
static GCRefPtr<HTMLElement> liConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLLIElement::create(document);
}
static GCRefPtr<HTMLElement> legendConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLLegendElement::create(document);
}
static GCRefPtr<HTMLElement> marqueeConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLMarqueeElement::create(document);
}
static GCRefPtr<HTMLElement> menuConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLMenuElement::create(document);
}
static GCRefPtr<HTMLElement> menuitemConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    if (!RuntimeEnabledFeatures::contextMenuEnabled())
        return HTMLUnknownElement::create(menuitemTag, document);
    return HTMLMenuItemElement::create(document);
}
static GCRefPtr<HTMLElement> metaConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLMetaElement::create(document);
}
static GCRefPtr<HTMLElement> meterConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLMeterElement::create(document);
}
static GCRefPtr<HTMLElement> olConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLOListElement::create(document);
}
static GCRefPtr<HTMLElement> optgroupConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLOptGroupElement::create(document);
}
static GCRefPtr<HTMLElement> optionConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLOptionElement::create(document);
}
static GCRefPtr<HTMLElement> pConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLParagraphElement::create(document);
}
static GCRefPtr<HTMLElement> pictureConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLPictureElement::create(document);
}
static GCRefPtr<HTMLElement> preConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLPreElement::create(preTag, document);
}
static GCRefPtr<HTMLElement> listingConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLPreElement::create(listingTag, document);
}
static GCRefPtr<HTMLElement> xmpConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLPreElement::create(xmpTag, document);
}
static GCRefPtr<HTMLElement> progressConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLProgressElement::create(document);
}
static GCRefPtr<HTMLElement> blockquoteConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLQuoteElement::create(blockquoteTag, document);
}
static GCRefPtr<HTMLElement> qConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLQuoteElement::create(qTag, document);
}
static GCRefPtr<HTMLElement> shadowConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLShadowElement::create(document);
}
static GCRefPtr<HTMLElement> slotConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLSlotElement::create(document);
}
static GCRefPtr<HTMLElement> spanConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLSpanElement::create(document);
}
static GCRefPtr<HTMLElement> captionConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLTableCaptionElement::create(document);
}
static GCRefPtr<HTMLElement> tdConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLTableCellElement::create(tdTag, document);
}
static GCRefPtr<HTMLElement> thConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLTableCellElement::create(thTag, document);
}
static GCRefPtr<HTMLElement> colConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLTableColElement::create(colTag, document);
}
static GCRefPtr<HTMLElement> colgroupConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLTableColElement::create(colgroupTag, document);
}
static GCRefPtr<HTMLElement> tableConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLTableElement::create(document);
}
static GCRefPtr<HTMLElement> trConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLTableRowElement::create(document);
}
static GCRefPtr<HTMLElement> tbodyConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLTableSectionElement::create(tbodyTag, document);
}
static GCRefPtr<HTMLElement> tfootConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLTableSectionElement::create(tfootTag, document);
}
static GCRefPtr<HTMLElement> theadConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLTableSectionElement::create(theadTag, document);
}
static GCRefPtr<HTMLElement> templateConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLTemplateElement::create(document);
}
static GCRefPtr<HTMLElement> titleConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLTitleElement::create(document);
}
static GCRefPtr<HTMLElement> ulConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLUListElement::create(document);
}
static GCRefPtr<HTMLElement> imageConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLUnknownElement::create(imageTag, document);
}
static GCRefPtr<HTMLElement> buttonConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLButtonElement::create(document, formElement);
}
static GCRefPtr<HTMLElement> fieldsetConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLFieldSetElement::create(document, formElement);
}
static GCRefPtr<HTMLElement> labelConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLLabelElement::create(document, formElement);
}
static GCRefPtr<HTMLElement> selectConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLSelectElement::create(document, formElement);
}
static GCRefPtr<HTMLElement> textareaConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLTextAreaElement::create(document, formElement);
}
static GCRefPtr<HTMLElement> linkConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLLinkElement::create(document, createdByParser);
}
static GCRefPtr<HTMLElement> scriptConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLScriptElement::create(document, createdByParser);
}
static GCRefPtr<HTMLElement> styleConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLStyleElement::create(document, createdByParser);
}
static GCRefPtr<HTMLElement> imgConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLImageElement::create(document, formElement, createdByParser);
}
static GCRefPtr<HTMLElement> inputConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLInputElement::create(document, formElement, createdByParser);
}
static GCRefPtr<HTMLElement> bdiConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLBDIElement::create(document);
}
static GCRefPtr<HTMLElement> summaryConstructor(
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    return HTMLSummaryElement::create(document);
}
static GCRefPtr<HTMLElement> wbrConstructor(
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
        g_constructors->emplace(data[i].tag.localName(), data[i].func);
}

GCRefPtr<HTMLElement> HTMLElementFactory::createHTMLElement(
    const AtomicString& localName,
    Document& document,
    HTMLFormElement* formElement,
    bool createdByParser)
{
    if (!g_constructors)
        createHTMLFunctionMap();
    if (ConstructorFunction function = zed::query_value(*g_constructors, localName, nullptr))
        return function(document, formElement, createdByParser);

    if (document.registrationContext() && CustomElement::isValidName(localName)) {
        RefPtrWillBeRawPtr<Element> element = document.registrationContext()->createCustomTagElement(document, QualifiedName(nullAtom, localName, xhtmlNamespaceURI));
        ASSERT_WITH_SECURITY_IMPLICATION(element->isHTMLElement());
        return static_pointer_cast<HTMLElement>(element.release());
    }

    return HTMLUnknownElement::create(QualifiedName(nullAtom, localName, xhtmlNamespaceURI), document);
}

} // namespace blink
