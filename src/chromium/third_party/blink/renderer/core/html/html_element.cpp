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

bool HTMLElement::SelfOrAncestorHasDirAutoAttribute(void) const
{
    // TODO(esprehn): Storing this state in the computed style is bad, we
    // should be able to answer questions about the shape of the DOM and the
    // text contained inside it without having style.
    if (const ComputedStyle *style = GetComputedStyle())
        return style->SelfOrAncestorHasDirAutoAttribute();
    return false;
}

} // namespace blink
