// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: HTMLImageElement.cpp
// Description: HTMLImageElement Class
//      Author: Ziming Li
//     Created: 2021-07-30
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2010 Apple Inc. All rights reserved.
 * Copyright (C) 2010 Google Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "./HTMLImageElement.h"

#include "blinkit/blink/renderer/core/CSSPropertyNames.h"
#include "blinkit/blink/renderer/core/HTMLNames.h"
// BKTODO: #include "core/MediaTypeNames.h"
#include "blinkit/blink/renderer/core/css/MediaQueryMatcher.h"
#include "blinkit/blink/renderer/core/css/MediaValuesDynamic.h"
#include "blinkit/blink/renderer/core/css/parser/SizesAttributeParser.h"
#include "blinkit/blink/renderer/core/dom/Attribute.h"
#include "blinkit/blink/renderer/core/dom/node_traversal.h"
#include "blinkit/blink/renderer/core/dom/shadow/ShadowRoot.h"
#include "blinkit/blink/renderer/core/fetch/ImageResource.h"
#include "blinkit/blink/renderer/core/frame/ImageBitmap.h"
#include "blinkit/blink/renderer/core/frame/UseCounter.h"
#include "blinkit/blink/renderer/core/html/HTMLAnchorElement.h"
// BKTODO: #include "core/html/HTMLCanvasElement.h"
#include "blinkit/blink/renderer/core/html/HTMLFormElement.h"
#include "blinkit/blink/renderer/core/html/HTMLImageFallbackHelper.h"
// BKTODO: #include "core/html/HTMLSourceElement.h"
#include "blinkit/blink/renderer/core/html/parser/HTMLParserIdioms.h"
#include "blinkit/blink/renderer/core/html/parser/HTMLSrcsetParser.h"
// BKTODO: #include "core/inspector/ConsoleMessage.h"
#include "blinkit/blink/renderer/core/layout/LayoutBlockFlow.h"
#include "blinkit/blink/renderer/core/layout/LayoutImage.h"
#include "blinkit/blink/renderer/core/page/Page.h"
#include "blinkit/blink/renderer/core/style/ContentData.h"
// BKTODO: #include "core/svg/graphics/SVGImageForContainer.h"
#include "blinkit/blink/renderer/platform/ContentType.h"
#include "blinkit/blink/renderer/platform/EventDispatchForbiddenScope.h"
#include "blinkit/blink/renderer/platform/MIMETypeRegistry.h"
#include "blinkit/blink/renderer/platform/RuntimeEnabledFeatures.h"
// BKTODO: #include "platform/weborigin/SecurityPolicy.h"

namespace blink {

using namespace HTMLNames;

#if 0 // BKTODO:
class HTMLImageElement::ViewportChangeListener final : public MediaQueryListListener {
public:
    static RefPtrWillBeRawPtr<ViewportChangeListener> create(HTMLImageElement* element)
    {
        return adoptRefWillBeNoop(new ViewportChangeListener(element));
    }

    void notifyMediaQueryChanged() override
    {
        if (m_element)
            m_element->notifyViewportChanged();
    }

#if !ENABLE(OILPAN)
    void clearElement() { m_element = nullptr; }
#endif
    DEFINE_INLINE_VIRTUAL_TRACE()
    {
        visitor->trace(m_element);
        MediaQueryListListener::trace(visitor);
    }
private:
    explicit ViewportChangeListener(HTMLImageElement* element) : m_element(element) { }
    RawPtrWillBeMember<HTMLImageElement> m_element;
};
#endif

HTMLImageElement::HTMLImageElement(Document& document, HTMLFormElement* form, bool createdByParser)
    : HTMLElement(imgTag, document)
    , m_imageLoader(HTMLImageLoader::create(this))
    , m_imageDevicePixelRatio(1.0f)
    // BKTODO: , m_source(nullptr)
    , m_formWasSetByParser(false)
    , m_elementCreatedByParser(createdByParser)
    , m_intrinsicSizingViewportDependant(false)
    , m_useFallbackContent(false)
    , m_isFallbackImage(false)
    // BKTODO: , m_referrerPolicy(ReferrerPolicyDefault)
{
    setHasCustomStyleCallbacks();
    if (form && form->inDocument()) {
#if ENABLE(OILPAN)
        m_form = form;
#else
        m_form = form->createWeakPtr();
#endif
        m_formWasSetByParser = true;
        m_form->associate(*this);
        m_form->didAssociateByParser();
    }
}

PassRefPtrWillBeRawPtr<HTMLImageElement> HTMLImageElement::create(Document& document)
{
    return adoptRefWillBeNoop(new HTMLImageElement(document));
}

PassRefPtrWillBeRawPtr<HTMLImageElement> HTMLImageElement::create(Document& document, HTMLFormElement* form, bool createdByParser)
{
    return adoptRefWillBeNoop(new HTMLImageElement(document, form, createdByParser));
}

HTMLImageElement::~HTMLImageElement()
{
#if !ENABLE(OILPAN)
    if (m_listener) {
        document().mediaQueryMatcher().removeViewportListener(m_listener.get());
        m_listener->clearElement();
    }
    if (m_form)
        m_form->disassociate(*this);
#endif
}

DEFINE_TRACE(HTMLImageElement)
{
    visitor->trace(m_imageLoader);
    visitor->trace(m_listener);
    visitor->trace(m_form);
    // BKTODO: visitor->trace(m_source);
    HTMLElement::trace(visitor);
}

void HTMLImageElement::notifyViewportChanged()
{
    // Re-selecting the source URL in order to pick a more fitting resource
    // And update the image's intrinsic dimensions when the viewport changes.
    // Picking of a better fitting resource is UA dependant, not spec required.
    selectSourceURL(ImageLoader::UpdateSizeChanged);
}

PassRefPtrWillBeRawPtr<HTMLImageElement> HTMLImageElement::createForJSConstructor(Document& document)
{
    RefPtrWillBeRawPtr<HTMLImageElement> image = adoptRefWillBeNoop(new HTMLImageElement(document));
    image->m_elementCreatedByParser = false;
    return image.release();
}

PassRefPtrWillBeRawPtr<HTMLImageElement> HTMLImageElement::createForJSConstructor(Document& document, int width)
{
    RefPtrWillBeRawPtr<HTMLImageElement> image = adoptRefWillBeNoop(new HTMLImageElement(document));
    image->setWidth(width);
    image->m_elementCreatedByParser = false;
    return image.release();
}

PassRefPtrWillBeRawPtr<HTMLImageElement> HTMLImageElement::createForJSConstructor(Document& document, int width, int height)
{
    RefPtrWillBeRawPtr<HTMLImageElement> image = adoptRefWillBeNoop(new HTMLImageElement(document));
    image->setWidth(width);
    image->setHeight(height);
    image->m_elementCreatedByParser = false;
    return image.release();
}

bool HTMLImageElement::isPresentationAttribute(const QualifiedName& name) const
{
    if (name == widthAttr || name == heightAttr || name == borderAttr || name == vspaceAttr || name == hspaceAttr || name == alignAttr || name == valignAttr)
        return true;
    return HTMLElement::isPresentationAttribute(name);
}

void HTMLImageElement::collectStyleForPresentationAttribute(const QualifiedName& name, const AtomicString& value, MutableStylePropertySet* style)
{
    if (name == widthAttr) {
        addHTMLLengthToStyle(style, CSSPropertyWidth, value);
    } else if (name == heightAttr) {
        addHTMLLengthToStyle(style, CSSPropertyHeight, value);
    } else if (name == borderAttr) {
        applyBorderAttributeToStyle(value, style);
    } else if (name == vspaceAttr) {
        addHTMLLengthToStyle(style, CSSPropertyMarginTop, value);
        addHTMLLengthToStyle(style, CSSPropertyMarginBottom, value);
    } else if (name == hspaceAttr) {
        addHTMLLengthToStyle(style, CSSPropertyMarginLeft, value);
        addHTMLLengthToStyle(style, CSSPropertyMarginRight, value);
    } else if (name == alignAttr) {
        applyAlignmentAttributeToStyle(value, style);
    } else if (name == valignAttr) {
        addPropertyToPresentationAttributeStyle(style, CSSPropertyVerticalAlign, value);
    } else {
        HTMLElement::collectStyleForPresentationAttribute(name, value, style);
    }
}

const AtomicString HTMLImageElement::imageSourceURL() const
{
    return m_bestFitImageURL.isNull() ? fastGetAttribute(srcAttr) : m_bestFitImageURL;
}

HTMLFormElement* HTMLImageElement::formOwner() const
{
    return m_form.get();
}

void HTMLImageElement::formRemovedFromTree(const Node& formRoot)
{
    ASSERT(m_form);
    if (NodeTraversal::highestAncestorOrSelf(*this) != formRoot)
        resetFormOwner();
}

void HTMLImageElement::resetFormOwner()
{
    m_formWasSetByParser = false;
    HTMLFormElement* nearestForm = findFormAncestor();
    if (m_form) {
        if (nearestForm == m_form.get())
            return;
        m_form->disassociate(*this);
    }
    if (nearestForm) {
#if ENABLE(OILPAN)
        m_form = nearestForm;
#else
        m_form = nearestForm->createWeakPtr();
#endif
        m_form->associate(*this);
    } else {
#if ENABLE(OILPAN)
        m_form = nullptr;
#else
        m_form = WeakPtr<HTMLFormElement>();
#endif
    }
}

void HTMLImageElement::setBestFitURLAndDPRFromImageCandidate(const ImageCandidate& candidate)
{
    m_bestFitImageURL = candidate.url();
    float candidateDensity = candidate.density();
    if (candidateDensity >= 0)
        m_imageDevicePixelRatio = 1.0 / candidateDensity;
    if (candidate.resourceWidth() > 0) {
        m_intrinsicSizingViewportDependant = true;
        UseCounter::count(document(), UseCounter::SrcsetWDescriptor);
    } else if (!candidate.srcOrigin()) {
        UseCounter::count(document(), UseCounter::SrcsetXDescriptor);
    }
    if (layoutObject() && layoutObject()->isImage())
        toLayoutImage(layoutObject())->setImageDevicePixelRatio(m_imageDevicePixelRatio);
}

void HTMLImageElement::parseAttribute(const QualifiedName& name, const AtomicString& oldValue, const AtomicString& value)
{
    ASSERT(false); // BKTODO:
#if 0
    if (name == altAttr || name == titleAttr) {
        if (userAgentShadowRoot()) {
            Element* text = userAgentShadowRoot()->getElementById("alttext");
            String value = altText();
            if (text && text->textContent() != value)
                text->setTextContent(altText());
        }
    } else if (name == srcAttr || name == srcsetAttr || name == sizesAttr) {
        selectSourceURL(ImageLoader::UpdateIgnorePreviousError);
    } else if (name == usemapAttr) {
        setIsLink(!value.isNull());
    } else if (RuntimeEnabledFeatures::referrerPolicyAttributeEnabled() && name == referrerpolicyAttr) {
        m_referrerPolicy = ReferrerPolicyDefault;
        if (!value.isNull())
            SecurityPolicy::referrerPolicyFromString(value, &m_referrerPolicy);
    } else {
        HTMLElement::parseAttribute(name, oldValue, value);
    }
#endif
}

String HTMLImageElement::altText() const
{
    // lets figure out the alt text.. magic stuff
    // http://www.w3.org/TR/1998/REC-html40-19980424/appendix/notes.html#altgen
    // also heavily discussed by Hixie on bugzilla
    const AtomicString& alt = fastGetAttribute(altAttr);
    if (!alt.isNull())
        return alt;
    // fall back to title attribute
    return fastGetAttribute(titleAttr);
}

static bool supportedImageType(const String& type)
{
    String trimmedType = ContentType(type).type();
    // An empty type attribute is implicitly supported.
    if (trimmedType.isEmpty())
        return true;
    return MIMETypeRegistry::isSupportedImagePrefixedMIMEType(trimmedType);
}

// http://picture.responsiveimages.org/#update-source-set
ImageCandidate HTMLImageElement::findBestFitImageFromPictureParent()
{
    ASSERT(isMainThread());
    Node* parent = parentNode();
    ASSERT(false); // BKTODO: m_source = nullptr;
    if (!parent || !isHTMLPictureElement(*parent))
        return ImageCandidate();
    for (Node* child = parent->firstChild(); child; child = child->nextSibling()) {
        if (child == this)
            return ImageCandidate();

        ASSERT(false); // BKTODO:
#if 0
        if (!isHTMLSourceElement(*child))
            continue;

        HTMLSourceElement* source = toHTMLSourceElement(child);
        if (!source->fastGetAttribute(srcAttr).isNull())
            UseCounter::countDeprecation(document(), UseCounter::PictureSourceSrc);
        String srcset = source->fastGetAttribute(srcsetAttr);
        if (srcset.isEmpty())
            continue;
        String type = source->fastGetAttribute(typeAttr);
        if (!type.isEmpty() && !supportedImageType(type))
            continue;

        if (!source->mediaQueryMatches())
            continue;

        ImageCandidate candidate = bestFitSourceForSrcsetAttribute(document().devicePixelRatio(), sourceSize(*source), source->fastGetAttribute(srcsetAttr), &document());
        if (candidate.isEmpty())
            continue;
        m_source = source;
        return candidate;
#endif
    }
    return ImageCandidate();
}

LayoutObject* HTMLImageElement::createLayoutObject(const ComputedStyle& style)
{
    const ContentData* contentData = style.contentData();
    if (contentData && contentData->isImage()) {
        const StyleImage* contentImage = toImageContentData(contentData)->image();
        bool errorOccurred = contentImage && contentImage->cachedImage() && contentImage->cachedImage()->errorOccurred();
        if (!errorOccurred)
            return LayoutObject::createObject(this, style);
    }

    if (m_useFallbackContent)
        return new LayoutBlockFlow(this);

    LayoutImage* image = new LayoutImage(this);
    image->setImageResource(LayoutImageResource::create());
    image->setImageDevicePixelRatio(m_imageDevicePixelRatio);
    return image;
}

void HTMLImageElement::attach(const AttachContext& context)
{
    HTMLElement::attach(context);

    if (layoutObject() && layoutObject()->isImage()) {
        LayoutImage* layoutImage = toLayoutImage(layoutObject());
        LayoutImageResource* layoutImageResource = layoutImage->imageResource();
        if (m_isFallbackImage) {
            float deviceScaleFactor = blink::deviceScaleFactor(layoutImage->frame());
            std::pair<Image*, float> brokenImageAndImageScaleFactor = ImageResource::brokenImage(deviceScaleFactor);
            ImageResource* newImageResource = new ImageResource(brokenImageAndImageScaleFactor.first);
            layoutImage->imageResource()->setImageResource(newImageResource);
        }
        if (layoutImageResource->hasImage())
            return;

        if (!imageLoader().image() && !layoutImageResource->cachedImage())
            return;
        layoutImageResource->setImageResource(imageLoader().image());
    }
}

Node::InsertionNotificationRequest HTMLImageElement::insertedInto(ContainerNode* insertionPoint)
{
    if (!m_formWasSetByParser || NodeTraversal::highestAncestorOrSelf(*insertionPoint) != NodeTraversal::highestAncestorOrSelf(*m_form.get()))
        resetFormOwner();
    if (m_listener)
        ASSERT(false); // BKTODO: document().mediaQueryMatcher().addViewportListener(m_listener);

    bool imageWasModified = false;
    if (document().isActive()) {
        ImageCandidate candidate = findBestFitImageFromPictureParent();
        if (!candidate.isEmpty()) {
            setBestFitURLAndDPRFromImageCandidate(candidate);
            imageWasModified = true;
        }
    }

    // If we have been inserted from a layoutObject-less document,
    // our loader may have not fetched the image, so do it now.
    if ((insertionPoint->inDocument() && !imageLoader().image()) || imageWasModified)
        ASSERT(false); // BKTODO: imageLoader().updateFromElement(ImageLoader::UpdateNormal, m_referrerPolicy);

    return HTMLElement::insertedInto(insertionPoint);
}

void HTMLImageElement::removedFrom(ContainerNode* insertionPoint)
{
    if (!m_form || NodeTraversal::highestAncestorOrSelf(*m_form.get()) != NodeTraversal::highestAncestorOrSelf(*this))
        resetFormOwner();
    if (m_listener)
        ASSERT(false); // BKTODO: document().mediaQueryMatcher().removeViewportListener(m_listener);
    HTMLElement::removedFrom(insertionPoint);
}

int HTMLImageElement::width()
{
    if (inActiveDocument())
        document().updateLayoutIgnorePendingStylesheets();

    if (!layoutObject()) {
        // check the attribute first for an explicit pixel value
        bool ok;
        int width = getAttribute(widthAttr).toInt(&ok);
        if (ok)
            return width;

        // if the image is available, use its width
        if (imageLoader().image())
            return imageLoader().image()->imageSize(LayoutObject::shouldRespectImageOrientation(nullptr), 1.0f).width();
    }

    LayoutBox* box = layoutBox();
    return box ? adjustForAbsoluteZoom(box->contentBoxRect().pixelSnappedWidth(), box) : 0;
}

int HTMLImageElement::height()
{
    if (inActiveDocument())
        document().updateLayoutIgnorePendingStylesheets();

    if (!layoutObject()) {
        // check the attribute first for an explicit pixel value
        bool ok;
        int height = getAttribute(heightAttr).toInt(&ok);
        if (ok)
            return height;

        // if the image is available, use its height
        if (imageLoader().image())
            return imageLoader().image()->imageSize(LayoutObject::shouldRespectImageOrientation(nullptr), 1.0f).height();
    }

    LayoutBox* box = layoutBox();
    return box ? adjustForAbsoluteZoom(box->contentBoxRect().pixelSnappedHeight(), box) : 0;
}

int HTMLImageElement::naturalWidth() const
{
    if (!imageLoader().image())
        return 0;

    return imageLoader().image()->imageSize(LayoutObject::shouldRespectImageOrientation(layoutObject()), m_imageDevicePixelRatio, ImageResource::IntrinsicCorrectedToDPR).width();
}

int HTMLImageElement::naturalHeight() const
{
    if (!imageLoader().image())
        return 0;

    return imageLoader().image()->imageSize(LayoutObject::shouldRespectImageOrientation(layoutObject()), m_imageDevicePixelRatio, ImageResource::IntrinsicCorrectedToDPR).height();
}

const String& HTMLImageElement::currentSrc() const
{
    // http://www.whatwg.org/specs/web-apps/current-work/multipage/edits.html#dom-img-currentsrc
    // The currentSrc IDL attribute must return the img element's current request's current URL.
    // Initially, the pending request turns into current request when it is either available or broken.
    // We use the image's dimensions as a proxy to it being in any of these states.
    if (!imageLoader().image() || !imageLoader().image()->image() || !imageLoader().image()->image()->width())
        return emptyAtom;

    ASSERT(false); // BKTODO: return imageLoader().image()->url().string();
    return emptyString();
}

bool HTMLImageElement::isURLAttribute(const Attribute& attribute) const
{
    ASSERT(false); // BKTODO:
    return false;
#if 0
    return attribute.name() == srcAttr
        || attribute.name() == lowsrcAttr
        || attribute.name() == longdescAttr
        || (attribute.name() == usemapAttr && attribute.value()[0] != '#')
        || HTMLElement::isURLAttribute(attribute);
#endif
}

bool HTMLImageElement::hasLegalLinkAttribute(const QualifiedName& name) const
{
    return name == srcAttr || HTMLElement::hasLegalLinkAttribute(name);
}

const QualifiedName& HTMLImageElement::subResourceAttributeName() const
{
    return srcAttr;
}

bool HTMLImageElement::draggable() const
{
    // Image elements are draggable by default.
    return !equalIgnoringCase(getAttribute(draggableAttr), "false");
}

void HTMLImageElement::setHeight(int value)
{
    setIntegralAttribute(heightAttr, value);
}

KURL HTMLImageElement::src() const
{
    return document().completeURL(getAttribute(srcAttr));
}

void HTMLImageElement::setSrc(const String& value)
{
    setAttribute(srcAttr, AtomicString(value));
}

void HTMLImageElement::setWidth(int value)
{
    setIntegralAttribute(widthAttr, value);
}

int HTMLImageElement::x() const
{
    document().updateLayoutIgnorePendingStylesheets();
    LayoutObject* r = layoutObject();
    if (!r)
        return 0;

    // FIXME: This doesn't work correctly with transforms.
    FloatPoint absPos = r->localToAbsolute();
    return absPos.x();
}

int HTMLImageElement::y() const
{
    document().updateLayoutIgnorePendingStylesheets();
    LayoutObject* r = layoutObject();
    if (!r)
        return 0;

    // FIXME: This doesn't work correctly with transforms.
    FloatPoint absPos = r->localToAbsolute();
    return absPos.y();
}

bool HTMLImageElement::complete() const
{
    return imageLoader().imageComplete();
}

void HTMLImageElement::didMoveToNewDocument(Document& oldDocument)
{
    selectSourceURL(ImageLoader::UpdateIgnorePreviousError);
    imageLoader().elementDidMoveToNewDocument();
    HTMLElement::didMoveToNewDocument(oldDocument);
}

bool HTMLImageElement::isServerMap() const
{
    ASSERT(false); // BKTODO:
    return false;
#if 0
    if (!fastHasAttribute(ismapAttr))
        return false;

    const AtomicString& usemap = fastGetAttribute(usemapAttr);

    // If the usemap attribute starts with '#', it refers to a map element in the document.
    if (usemap[0] == '#')
        return false;

    return document().completeURL(stripLeadingAndTrailingHTMLSpaces(usemap)).isEmpty();
#endif
}

Image* HTMLImageElement::imageContents()
{
    if (!imageLoader().imageComplete())
        return nullptr;

    return imageLoader().image()->image();
}

bool HTMLImageElement::isInteractiveContent() const
{
    ASSERT(false); // BKTODO: return fastHasAttribute(usemapAttr);
    return false;
}

#if 0 // BKTODO:
PassRefPtr<Image> HTMLImageElement::getSourceImageForCanvas(SourceImageStatus* status, AccelerationHint) const
{
    if (!complete() || !cachedImage()) {
        *status = IncompleteSourceImageStatus;
        return nullptr;
    }

    if (cachedImage()->errorOccurred()) {
        *status = UndecodableSourceImageStatus;
        return nullptr;
    }

    RefPtr<Image> sourceImage;
    if (cachedImage()->image()->isSVGImage()) {
        sourceImage = SVGImageForContainer::create(toSVGImage(cachedImage()->image()),
            cachedImage()->image()->size(), 1, document().completeURL(imageSourceURL()));
    } else {
        sourceImage = cachedImage()->image();
    }

    *status = NormalSourceImageStatus;
    return sourceImage->imageForDefaultFrame();
}

bool HTMLImageElement::isSVGSource() const
{
    return cachedImage() && cachedImage()->image()->isSVGImage();
}

bool HTMLImageElement::wouldTaintOrigin(SecurityOrigin* destinationSecurityOrigin) const
{
    ImageResource* image = cachedImage();
    if (!image)
        return false;
    return !image->isAccessAllowed(destinationSecurityOrigin);
}

FloatSize HTMLImageElement::elementSize() const
{
    ImageResource* image = cachedImage();
    if (!image)
        return FloatSize();

    return FloatSize(image->imageSize(LayoutObject::shouldRespectImageOrientation(layoutObject()), 1.0f));
}

FloatSize HTMLImageElement::defaultDestinationSize() const
{
    ImageResource* image = cachedImage();
    if (!image)
        return FloatSize();
    LayoutSize size;
    size = image->imageSize(LayoutObject::shouldRespectImageOrientation(layoutObject()), 1.0f);
    if (layoutObject() && layoutObject()->isLayoutImage() && image->image() && !image->image()->hasRelativeWidth())
        size.scale(toLayoutImage(layoutObject())->imageDevicePixelRatio());
    return FloatSize(size);
}
#endif

static bool sourceSizeValue(Element& element, Document& currentDocument, float& sourceSize)
{
    ASSERT(false); // BKTODO:
    return false;
#if 0
    String sizes = element.fastGetAttribute(sizesAttr);
    bool exists = !sizes.isNull();
    if (exists)
        UseCounter::count(currentDocument, UseCounter::Sizes);
    sourceSize = SizesAttributeParser(MediaValuesDynamic::create(currentDocument), sizes).length();
    return exists;
#endif
}

FetchRequest::ResourceWidth HTMLImageElement::resourceWidth()
{
    FetchRequest::ResourceWidth resourceWidth;
    ASSERT(false); // BKTODO:
#if 0
    Element* element = m_source.get();
    if (!element)
        element = this;
    resourceWidth.isSet = sourceSizeValue(*element, document(), resourceWidth.width);
#endif
    return resourceWidth;
}

float HTMLImageElement::sourceSize(Element& element)
{
    float value;
    // We don't care here if the sizes attribute exists, so we ignore the return value.
    // If it doesn't exist, we just return the default.
    sourceSizeValue(element, document(), value);
    return value;
}

void HTMLImageElement::forceReload() const
{
    ASSERT(false); // BKTODO: imageLoader().updateFromElement(ImageLoader::UpdateForcedReload, m_referrerPolicy);
}

#if 0 // BKTODO:
ScriptPromise HTMLImageElement::createImageBitmap(ScriptState* scriptState, EventTarget& eventTarget, int sx, int sy, int sw, int sh, ExceptionState& exceptionState)
{
    ASSERT(eventTarget.toDOMWindow());
    if (!cachedImage()) {
        exceptionState.throwDOMException(InvalidStateError, "No image can be retrieved from the provided element.");
        return ScriptPromise();
    }
    if (cachedImage()->image()->isSVGImage()) {
        exceptionState.throwDOMException(InvalidStateError, "The image element contains an SVG image, which is unsupported.");
        return ScriptPromise();
    }
    if (!sw || !sh) {
        exceptionState.throwDOMException(IndexSizeError, String::format("The source %s provided is 0.", sw ? "height" : "width"));
        return ScriptPromise();
    }
    return ImageBitmapSource::fulfillImageBitmap(scriptState, ImageBitmap::create(this, IntRect(sx, sy, sw, sh), eventTarget.toDOMWindow()->document()));
}
#endif

void HTMLImageElement::selectSourceURL(ImageLoader::UpdateFromElementBehavior behavior)
{
    if (!document().isActive())
        return;

    bool foundURL = false;
    ImageCandidate candidate = findBestFitImageFromPictureParent();
    if (!candidate.isEmpty()) {
        setBestFitURLAndDPRFromImageCandidate(candidate);
        foundURL = true;
    }

    ASSERT(false); // BKTODO:
#if 0
    if (!foundURL) {
        candidate = bestFitSourceForImageAttributes(document().devicePixelRatio(), sourceSize(*this), fastGetAttribute(srcAttr), fastGetAttribute(srcsetAttr), &document());
        setBestFitURLAndDPRFromImageCandidate(candidate);
    }
    if (m_intrinsicSizingViewportDependant && !m_listener) {
        m_listener = ViewportChangeListener::create(this);
        document().mediaQueryMatcher().addViewportListener(m_listener);
    }
    imageLoader().updateFromElement(behavior, m_referrerPolicy);

    // Images such as data: uri's can return immediately and may already have errored out.
    bool imageHasLoaded = imageLoader().image() && !imageLoader().image()->errorOccurred();
    bool imageStillLoading = !imageHasLoaded && imageLoader().hasPendingActivity() && !imageLoader().hasPendingError() && !imageSourceURL().isEmpty();
    if (imageHasLoaded || imageStillLoading)
        ensurePrimaryContent();
    else
        ensureFallbackContent();
#endif
}

#if 0 // BKTODO:
const KURL& HTMLImageElement::sourceURL() const
{
    return cachedImage()->response().url();
}
#endif

void HTMLImageElement::didAddUserAgentShadowRoot(ShadowRoot&)
{
    HTMLImageFallbackHelper::createAltTextShadowTree(*this);
}

void HTMLImageElement::ensureFallbackForGeneratedContent()
{
    setUseFallbackContent();
    reattachFallbackContent();
}

void HTMLImageElement::ensureFallbackContent()
{
    if (m_useFallbackContent || m_isFallbackImage)
        return;
    setUseFallbackContent();
    reattachFallbackContent();
}

void HTMLImageElement::ensurePrimaryContent()
{
    if (!m_useFallbackContent)
        return;
    m_useFallbackContent = false;
    reattachFallbackContent();
}

void HTMLImageElement::reattachFallbackContent()
{
    // This can happen inside of attach() in the middle of a recalcStyle so we need to
    // reattach synchronously here.
    if (document().inStyleRecalc())
        reattach();
    else
        lazyReattachIfAttached();
}

PassRefPtr<ComputedStyle> HTMLImageElement::customStyleForLayoutObject()
{
    RefPtr<ComputedStyle> newStyle = originalStyleForLayoutObject();

    if (!m_useFallbackContent)
        return newStyle;

    RefPtr<ComputedStyle> style = ComputedStyle::clone(*newStyle);
    return HTMLImageFallbackHelper::customStyleForAltText(*this, style);
}

void HTMLImageElement::setUseFallbackContent()
{
    m_useFallbackContent = true;
    if (document().inStyleRecalc())
        return;
    EventDispatchForbiddenScope::AllowUserAgentEvents allowEvents;
    ensureUserAgentShadowRoot();
}

#if 0 // BKTODO:
bool HTMLImageElement::isOpaque() const
{
    Image* image = const_cast<HTMLImageElement*>(this)->imageContents();
    return image && image->currentFrameKnownToBeOpaque();
}

IntSize HTMLImageElement::bitmapSourceSize() const
{
    ImageResource* image = cachedImage();
    if (!image)
        return IntSize();
    LayoutSize lSize = image->imageSize(LayoutObject::shouldRespectImageOrientation(layoutObject()), 1.0f);
    ASSERT(lSize.fraction().isZero());
    return IntSize(lSize.width(), lSize.height());
}
#endif

}
