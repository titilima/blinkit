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

class FormAssociated;
class HTMLFormElement;

class HTMLElement : public Element
{
public:
    typedef Element* (*Creator)(Document &, const CreateElementFlags);

    virtual bool IsHTMLUnknownElement(void) const { return false; }
    virtual bool IsLabelable(void) const { return false; }
    // http://www.whatwg.org/specs/web-apps/current-work/multipage/elements.html#interactive-content
    virtual bool IsInteractiveContent(void) const { return false; }

    bool HasDirectionAuto(void) const;
    TextDirection DirectionalityIfhasDirAutoAttribute(bool &isAuto) const;

    virtual HTMLFormElement* formOwner(void) const { return nullptr; }
    virtual const AtomicString& autocapitalize(void) const;
    virtual FormAssociated* ToFormAssociatedOrNull(void) { return nullptr; };

    virtual String AltText(void) const { return String(); }

    String title(void) const final;
    int tabIndex(void) const override;

    virtual bool HasCustomFocusLogic(void) const { return false; }
protected:
    HTMLElement(const QualifiedName &tagName, Document &document, ConstructionType type = kCreateHTMLElement);

    static bool ParseColorWithLegacyRules(const String &attributevalue, Color &parsedColor);

    void ChildrenChanged(const ChildrenChange &change) override;
    InsertionNotificationRequest InsertedInto(ContainerNode &insertionPoint) override;

    bool IsPresentationAttribute(const QualifiedName &name) const override;
    void CollectStyleForPresentationAttribute(const QualifiedName &name, const AtomicString &value,
        MutableCSSPropertyValueSet *style) override;
private:
    TextDirection Directionality(Node **strongDirectionalityTextNode = nullptr) const;

    bool SelfOrAncestorHasDirAutoAttribute(void) const;
    void AdjustDirectionalityIfNeededAfterChildrenChanged(const ChildrenChange &change);
};

DEFINE_ELEMENT_TYPE_CASTS(HTMLElement, IsHTMLElement());

template <typename T>
bool IsElementOfType(const HTMLElement &);

template <>
inline bool IsElementOfType<const HTMLElement>(const HTMLElement &)
{
    return true;
}

} // namespace blink

#define DECLARE_NODE_FACTORY(T) static Element* Create(Document &, const CreateElementFlags)

#define DEFINE_NODE_FACTORY(T)                                          \
    Element* T::Create(Document &document, const CreateElementFlags)    \
    {                                                                   \
        return new T(document);                                         \
    }

// This requires IsHTML*Element(const Element&) and IsHTML*Element(constHTMLElement&).
// When the input element is an HTMLElement, we don't need to check the namespace URI, just the local name.
#define DEFINE_HTMLELEMENT_TYPE_CASTS_WITH_FUNCTION(ThisType)                       \
    inline bool Is##ThisType(const ThisType *element);                              \
    inline bool Is##ThisType(const ThisType &element);                              \
    inline bool Is##ThisType(const HTMLElement *element) {                          \
        return nullptr != element && Is##ThisType(*element);                        \
    }                                                                               \
    inline bool Is##ThisType(const Node &node) {                                    \
        return node.IsHTMLElement() ? Is##ThisType(ToHTMLElement(node)) : false;    \
    }                                                                               \
    inline bool Is##ThisType(const Node *node) {                                    \
        return nullptr != node && Is##ThisType(*node);                              \
    }                                                                               \
    inline bool Is##ThisType(const Element *element) {                              \
        return nullptr != element && Is##ThisType(*element);                        \
    }                                                                               \
    template <>                                                                     \
    inline bool IsElementOfType<const ThisType>(const HTMLElement &element) {       \
        return Is##ThisType(element);                                               \
    }                                                                               \
    DEFINE_ELEMENT_TYPE_CASTS_WITH_FUNCTION(ThisType)

#include "third_party/blink/renderer/core/html_element_type_helpers.h"

#endif // BLINKIT_BLINK_HTML_ELEMENT_H
