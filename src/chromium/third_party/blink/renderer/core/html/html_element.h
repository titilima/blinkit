// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_element.h
// Description: HTMLElement Class
//      Author: Ziming Li
//     Created: 2020-08-28
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_HTML_ELEMENT_H
#define BLINKIT_BLINK_HTML_ELEMENT_H

#pragma once

#include "third_party/blink/renderer/core/dom/element.h"

namespace blink {

class HTMLElement : public Element
{
public:
    typedef Element* (*Creator)(Document &);
protected:
    HTMLElement(const QualifiedName &tagName, Document &document, ConstructionType type = kCreateHTMLElement);

    void ChildrenChanged(const ChildrenChange &change) override;
    InsertionNotificationRequest InsertedInto(ContainerNode &insertionPoint) override;
private:
    bool SelfOrAncestorHasDirAutoAttribute(void) const;
    void AdjustDirectionalityIfNeededAfterChildrenChanged(const ChildrenChange &change);
};

DEFINE_ELEMENT_TYPE_CASTS(HTMLElement, IsHTMLElement());

} // namespace blink

#define DECLARE_NODE_FACTORY(T) static Element* Create(Document &)

#define DEFINE_NODE_FACTORY(T)              \
    Element* T::Create(Document &document)  \
    {                                       \
        return new T(document);             \
    }

#include "third_party/blink/renderer/core/html_element_type_helpers.h"

#endif // BLINKIT_BLINK_HTML_ELEMENT_H
