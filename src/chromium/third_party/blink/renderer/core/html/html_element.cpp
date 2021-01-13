// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_element.cpp
// Description: HTMLElement Class
//      Author: Ziming Li
//     Created: 2020-08-28
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "html_element.h"

#include "third_party/blink/renderer/core/dom/flat_tree_traversal.h"
#include "third_party/blink/renderer/core/html/forms/html_input_element.h"
#include "third_party/blink/renderer/core/html/html_dimension.h"

namespace blink {

HTMLElement::HTMLElement(const QualifiedName &tagName, Document &document, ConstructionType type)
    : Element(tagName, &document, type)
{
    ASSERT(!tagName.LocalName().IsNull());
}

void HTMLElement::AddHTMLLengthToStyle(
    MutableCSSPropertyValueSet *style,
    CSSPropertyID propertyId, const String &value,
    AllowPercentage allowPercentage)
{
    HTMLDimension dimension;
    if (!ParseDimensionValue(value, dimension))
        return;
    if (dimension.IsRelative())
        return;
    if (dimension.IsPercentage() && allowPercentage != kAllowPercentageValues)
        return;

    CSSPrimitiveValue::UnitType unit = dimension.IsPercentage()
        ? CSSPrimitiveValue::UnitType::kPercentage
        : CSSPrimitiveValue::UnitType::kPixels;
    AddPropertyToPresentationAttributeStyle(style, propertyId, dimension.Value(), unit);
}

void HTMLElement::AdjustDirectionalityIfNeededAfterChildrenChanged(const ChildrenChange &change)
{
    if (!SelfOrAncestorHasDirAutoAttribute())
        return;

    ASSERT(false); // BKTODO:
#if 0
    UpdateDistributionForFlatTreeTraversal();

    for (Element* element_to_adjust = this; element_to_adjust;
        element_to_adjust =
        FlatTreeTraversal::ParentElement(*element_to_adjust)) {
        if (ElementAffectsDirectionality(element_to_adjust)) {
            ToHTMLElement(element_to_adjust)->CalculateAndAdjustDirectionality();
            return;
        }
    }
#endif
}

void HTMLElement::ApplyAlignmentAttributeToStyle(const AtomicString &alignment, MutableCSSPropertyValueSet *style)
{
    // Vertical alignment with respect to the current baseline of the text
    // right or left means floating images.
    CSSValueID floatValue = CSSValueInvalid;
    CSSValueID verticalAlignValue = CSSValueInvalid;

    if (DeprecatedEqualIgnoringCase(alignment, "absmiddle"))
    {
        verticalAlignValue = CSSValueMiddle;
    }
    else if (DeprecatedEqualIgnoringCase(alignment, "absbottom"))
    {
        verticalAlignValue = CSSValueBottom;
    }
    else if (DeprecatedEqualIgnoringCase(alignment, "left"))
    {
        floatValue = CSSValueLeft;
        verticalAlignValue = CSSValueTop;
    }
    else if (DeprecatedEqualIgnoringCase(alignment, "right"))
    {
        floatValue = CSSValueRight;
        verticalAlignValue = CSSValueTop;
    }
    else if (DeprecatedEqualIgnoringCase(alignment, "top"))
    {
        verticalAlignValue = CSSValueTop;
    }
    else if (DeprecatedEqualIgnoringCase(alignment, "middle"))
    {
        verticalAlignValue = CSSValueWebkitBaselineMiddle;
    }
    else if (DeprecatedEqualIgnoringCase(alignment, "center"))
    {
        verticalAlignValue = CSSValueMiddle;
    }
    else if (DeprecatedEqualIgnoringCase(alignment, "bottom"))
    {
        verticalAlignValue = CSSValueBaseline;
    }
    else if (DeprecatedEqualIgnoringCase(alignment, "texttop"))
    {
        verticalAlignValue = CSSValueTextTop;
    }

    if (floatValue != CSSValueInvalid)
        AddPropertyToPresentationAttributeStyle(style, CSSPropertyFloat, floatValue);

    if (verticalAlignValue != CSSValueInvalid)
        AddPropertyToPresentationAttributeStyle(style, CSSPropertyVerticalAlign, verticalAlignValue);
}

const AtomicString& HTMLElement::autocapitalize(void) const
{
    ASSERT(false); // BKTODO:
#if 0
    DEFINE_STATIC_LOCAL(const AtomicString, kOff, ("off"));
    DEFINE_STATIC_LOCAL(const AtomicString, kNone, ("none"));
    DEFINE_STATIC_LOCAL(const AtomicString, kCharacters, ("characters"));
    DEFINE_STATIC_LOCAL(const AtomicString, kWords, ("words"));
    DEFINE_STATIC_LOCAL(const AtomicString, kSentences, ("sentences"));

    const AtomicString& value = FastGetAttribute(autocapitalizeAttr);
    if (value.IsEmpty())
        return g_empty_atom;

    if (EqualIgnoringASCIICase(value, kNone) ||
        EqualIgnoringASCIICase(value, kOff))
        return kNone;
    if (EqualIgnoringASCIICase(value, kCharacters))
        return kCharacters;
    if (EqualIgnoringASCIICase(value, kWords))
        return kWords;
    // "sentences", "on", or an invalid value
    return kSentences;
#endif
    return g_empty_atom;
}

void HTMLElement::ChildrenChanged(const ChildrenChange &change)
{
    Element::ChildrenChanged(change);
    AdjustDirectionalityIfNeededAfterChildrenChanged(change);
}

void HTMLElement::CollectStyleForPresentationAttribute(
    const QualifiedName &name, const AtomicString &value,
    MutableCSSPropertyValueSet *style)
{
    using namespace html_names;
    if (name == kAlignAttr)
    {
        if (DeprecatedEqualIgnoringCase(value, "middle"))
            AddPropertyToPresentationAttributeStyle(style, CSSPropertyTextAlign, CSSValueCenter);
        else
            AddPropertyToPresentationAttributeStyle(style, CSSPropertyTextAlign, value);
    }
    else if (name == kContenteditableAttr)
    {
        if (value.IsEmpty() || DeprecatedEqualIgnoringCase(value, "true"))
        {
            AddPropertyToPresentationAttributeStyle(style, CSSPropertyWebkitUserModify, CSSValueReadWrite);
            AddPropertyToPresentationAttributeStyle(style, CSSPropertyOverflowWrap, CSSValueBreakWord);
            AddPropertyToPresentationAttributeStyle(style, CSSPropertyWebkitLineBreak, CSSValueAfterWhiteSpace);
        }
        else if (DeprecatedEqualIgnoringCase(value, "plaintext-only"))
        {
            AddPropertyToPresentationAttributeStyle(style, CSSPropertyWebkitUserModify, CSSValueReadWritePlaintextOnly);
            AddPropertyToPresentationAttributeStyle(style, CSSPropertyOverflowWrap, CSSValueBreakWord);
            AddPropertyToPresentationAttributeStyle(style, CSSPropertyWebkitLineBreak, CSSValueAfterWhiteSpace);
        }
        else if (DeprecatedEqualIgnoringCase(value, "false"))
        {
            AddPropertyToPresentationAttributeStyle(style, CSSPropertyWebkitUserModify, CSSValueReadOnly);
        }
    }
    else if (name == kHiddenAttr)
    {
        AddPropertyToPresentationAttributeStyle(style, CSSPropertyDisplay, CSSValueNone);
    }
    else if (name == kDraggableAttr)
    {
        if (DeprecatedEqualIgnoringCase(value, "true"))
        {
            AddPropertyToPresentationAttributeStyle(style, CSSPropertyWebkitUserDrag, CSSValueElement);
            AddPropertyToPresentationAttributeStyle(style, CSSPropertyUserSelect, CSSValueNone);
        }
        else if (DeprecatedEqualIgnoringCase(value, "false"))
        {
            AddPropertyToPresentationAttributeStyle(style, CSSPropertyWebkitUserDrag, CSSValueNone);
        }
    }
    else if (name == kDirAttr)
    {
        ASSERT(false); // BKTODO:
#if 0
        if (DeprecatedEqualIgnoringCase(value, "auto"))
        {
            AddPropertyToPresentationAttributeStyle(style, CSSPropertyUnicodeBidi, UnicodeBidiAttributeForDirAuto(this));
        }
        else
        {
            if (IsValidDirAttribute(value))
                AddPropertyToPresentationAttributeStyle(style, CSSPropertyDirection, value);
            else if (IsHTMLBodyElement(*this))
                AddPropertyToPresentationAttributeStyle(style, CSSPropertyDirection, "ltr");
            if (!HasTagName(bdiTag) && !HasTagName(bdoTag) && !HasTagName(outputTag))
                AddPropertyToPresentationAttributeStyle(style, CSSPropertyUnicodeBidi, CSSValueIsolate);
        }
#endif
    }
    else
    {
        Element::CollectStyleForPresentationAttribute(name, value, style);
    }
}

TextDirection HTMLElement::Directionality(Node **strongDirectionalityTextNode) const
{
    if (const HTMLInputElement *inputElement = ToHTMLInputElementOrNull(*this))
    {
        ASSERT(false); // BKTODO:
#if 0
        bool hasStrongDirectionality;
        TextDirection textDirection = DetermineDirectionality(inputElement->value(), &hasStrongDirectionality);
        if (nullptr != strongDirectionalityTextNode)
        {
            *strongDirectionalityTextNode = hasStrongDirectionality
                ? const_cast<HTMLInputElement *>(inputElement)
                : nullptr;
        }
        return textDirection;
#endif
    }

    Node *node = FlatTreeTraversal::FirstChild(*this);
    while (nullptr != node)
    {
        // Skip bdi, script, style and text form controls.
        if (DeprecatedEqualIgnoringCase(node->nodeName(), "bdi")
            || IsHTMLScriptElement(*node)
            || IsHTMLStyleElement(*node)
            || (node->IsElementNode() && ToElement(node)->IsTextControl())
            || (node->IsElementNode() && ToElement(node)->ShadowPseudoId() == "-webkit-input-placeholder"))
        {
            node = FlatTreeTraversal::NextSkippingChildren(*node, this);
            continue;
        }

        // Skip elements with valid dir attribute
        if (node->IsElementNode())
        {
            AtomicString dirAttributeValue = ToElement(node)->FastGetAttribute(html_names::kDirAttr);
            ASSERT(false); // BKTODO:
#if 0
            if (IsValidDirAttribute(dirAttributeValue))
            {
                node = FlatTreeTraversal::NextSkippingChildren(*node, this);
                continue;
            }
#endif
        }

        if (node->IsTextNode())
        {
            ASSERT(false); // BKTODO:
#if 0
            bool hasStrongDirectionality;
            TextDirection textDirection = DetermineDirectionality(node->textContent(true), &hasStrongDirectionality);
            if (hasStrongDirectionality)
            {
                if (nullptr != strongDirectionalityTextNode)
                    *strongDirectionalityTextNode = node;
                return textDirection;
            }
#endif
        }
        node = FlatTreeTraversal::Next(*node, this);
    }

    if (strongDirectionalityTextNode)
        *strongDirectionalityTextNode = nullptr;
    return TextDirection::kLtr;
}

TextDirection HTMLElement::DirectionalityIfhasDirAutoAttribute(bool &isAuto) const
{
    isAuto = HasDirectionAuto();
    if (!isAuto)
        return TextDirection::kLtr;
    return Directionality();
}

bool HTMLElement::draggable(void) const
{
    ASSERT(false); // BKTODO:
    return false;
#if 0
    return DeprecatedEqualIgnoringCase(getAttribute(html_names::kDraggableAttr), "true");
#endif
}

bool HTMLElement::HasDirectionAuto(void) const
{
    // <bdi> defaults to dir="auto"
    // https://html.spec.whatwg.org/multipage/semantics.html#the-bdi-element
    const AtomicString &direction = FastGetAttribute(html_names::kDirAttr);
    return DeprecatedEqualIgnoringCase(direction, "auto");
#if 0 // BKTODO: Process <bdi>
    return (IsHTMLBDIElement(*this) && direction == g_null_atom) ||
        DeprecatedEqualIgnoringCase(direction, "auto");
#endif
}

Node::InsertionNotificationRequest HTMLElement::InsertedInto(ContainerNode &insertionPoint)
{
    // Process the superclass first to ensure that `InActiveDocument()` is
    // updated.
    Element::InsertedInto(insertionPoint);

#if 0 // BKTODO: Check if necessary
    if (GetDocument().GetContentSecurityPolicy()->HasHeaderDeliveredPolicy() &&
        InActiveDocument() && FastHasAttribute(nonceAttr)) {
        setAttribute(nonceAttr, g_empty_atom);
    }
#endif

    return kInsertionDone;
}

bool HTMLElement::IsPresentationAttribute(const QualifiedName &name) const
{
    using namespace html_names;
    if (name == kAlignAttr || name == kContenteditableAttr || name == kHiddenAttr || name == kLangAttr
        || name == kDraggableAttr || name == kDirAttr)
    {
        return true;
    }
    return Element::IsPresentationAttribute(name);
}

bool HTMLElement::ParseColorWithLegacyRules(const String &attributevalue, Color &parsedColor)
{
    ASSERT(false); // BKTODO:
    return false;
}

bool HTMLElement::SelfOrAncestorHasDirAutoAttribute(void) const
{
    // TODO(esprehn): Storing this state in the computed style is bad, we
    // should be able to answer questions about the shape of the DOM and the
    // text contained inside it without having style.
    if (const ComputedStyle *style = GetComputedStyle())
        return style->SelfOrAncestorHasDirAutoAttribute();
    return false;
}

int HTMLElement::tabIndex(void) const
{
    if (SupportsFocus())
        return Element::tabIndex();
    return -1;
}

String HTMLElement::title(void) const
{
    return FastGetAttribute(html_names::kTitleAttr);
}

} // namespace blink
