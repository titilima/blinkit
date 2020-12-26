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

namespace blink {

HTMLElement::HTMLElement(const QualifiedName &tagName, Document &document, ConstructionType type)
    : Element(tagName, &document, type)
{
    ASSERT(!tagName.LocalName().IsNull());
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

void HTMLElement::ChildrenChanged(const ChildrenChange &change)
{
    Element::ChildrenChanged(change);
    AdjustDirectionalityIfNeededAfterChildrenChanged(change);
}

void HTMLElement::CollectStyleForPresentationAttribute(
    const QualifiedName &name, const AtomicString &value,
    MutableCSSPropertyValueSet *style)
{
    ASSERT(false); // BKTODO:
}

TextDirection HTMLElement::Directionality(Node **strongDirectionalityTextNode) const
{
    ASSERT(false); // BKTODO:
#if 0
    if (auto* input_element = ToHTMLInputElementOrNull(*this))
    {
        bool has_strong_directionality;
        TextDirection text_direction = DetermineDirectionality(
            input_element->value(), &has_strong_directionality);
        if (strong_directionality_text_node) {
            *strong_directionality_text_node =
                has_strong_directionality
                ? const_cast<HTMLInputElement*>(input_element)
                : nullptr;
        }
        return text_direction;
    }

    Node* node = FlatTreeTraversal::FirstChild(*this);
    while (node) {
        // Skip bdi, script, style and text form controls.
        if (DeprecatedEqualIgnoringCase(node->nodeName(), "bdi") ||
            IsHTMLScriptElement(*node) || IsHTMLStyleElement(*node) ||
            (node->IsElementNode() && ToElement(node)->IsTextControl()) ||
            (node->IsElementNode() &&
                ToElement(node)->ShadowPseudoId() == "-webkit-input-placeholder")) {
            node = FlatTreeTraversal::NextSkippingChildren(*node, this);
            continue;
        }

        // Skip elements with valid dir attribute
        if (node->IsElementNode()) {
            AtomicString dir_attribute_value =
                ToElement(node)->FastGetAttribute(dirAttr);
            if (IsValidDirAttribute(dir_attribute_value)) {
                node = FlatTreeTraversal::NextSkippingChildren(*node, this);
                continue;
            }
        }

        if (node->IsTextNode()) {
            bool has_strong_directionality;
            TextDirection text_direction = DetermineDirectionality(
                node->textContent(true), &has_strong_directionality);
            if (has_strong_directionality) {
                if (strong_directionality_text_node)
                    *strong_directionality_text_node = node;
                return text_direction;
            }
        }
        node = FlatTreeTraversal::Next(*node, this);
    }
    if (strong_directionality_text_node)
        *strong_directionality_text_node = nullptr;
#endif
    return TextDirection::kLtr;
}

TextDirection HTMLElement::DirectionalityIfhasDirAutoAttribute(bool &isAuto) const
{
    isAuto = HasDirectionAuto();
    if (!isAuto)
        return TextDirection::kLtr;
    return Directionality();
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
