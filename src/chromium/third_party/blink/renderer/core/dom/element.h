// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: element.h
// Description: Element Class
//      Author: Ziming Li
//     Created: 2019-10-18
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Peter Kelly (pmk@post.com)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2003-2011, 2013, 2014 Apple Inc. All rights reserved.
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
 *
 */

#ifndef BLINKIT_BLINK_ELEMENT_H
#define BLINKIT_BLINK_ELEMENT_H

#pragma once

#include "base/memory/scoped_refptr.h"
#include "third_party/blink/renderer/core/css/css_selector.h"
#include "third_party/blink/renderer/core/dom/container_node.h"
#include "third_party/blink/renderer/core/dom/element_data.h"
#include "third_party/blink/renderer/core/dom/qualified_name.h"
#include "url/bk_url.h"

namespace blink {

class Attr;
class Attribute;
class ElementData;
class ElementRareData;
class NamedNodeMap;

enum class ElementFlags {
    kTabIndexWasSetExplicitly = 1 << 0,
    kStyleAffectedByEmpty = 1 << 1,
    kIsInCanvasSubtree = 1 << 2,
    kContainsFullScreenElement = 1 << 3,
    kIsInTopLayer = 1 << 4,
    kContainsPersistentVideo = 1 << 5,

    kNumberOfElementFlags = 6,  // Size of bitfield used to store the flags.
};

// https://html.spec.whatwg.org/multipage/dom.html#dom-document-nameditem-filter
enum class NamedItemType {
    kNone,
    kName,
    kNameOrId,
    kNameOrIdWithName,
};

typedef std::vector<Attr *> AttrNodeList;

class Element : public ContainerNode
{
public:
    ~Element(void) override;

    // Exports for JS
    NamedNodeMap* attributes(void) const;
    bool hasAttribute(const AtomicString &name) const;
    String innerHTML(void) const;
    void setInnerHTML(const String &html, ExceptionState &exceptionState);
    void setAttribute(const AtomicString &localName, const AtomicString &value, ExceptionState &exceptionState);
    String tagName(void) const { return nodeName(); }

    const QualifiedName& TagQName(void) const { return m_tagName; }
    const AtomicString& prefix(void) const { return m_tagName.Prefix(); }
    const AtomicString& localName(void) const { return m_tagName.LocalName(); }
    const AtomicString& namespaceURI(void) const { return m_tagName.NamespaceURI(); }
    bool HasTagName(const QualifiedName &tagName) const { return m_tagName.Matches(tagName); }
    bool ShouldSerializeEndTag(void) const;

    AttributeCollection Attributes(void) const;
    AttributeCollection AttributesWithoutUpdate(void) const;
    const AtomicString& getAttribute(const QualifiedName &name) const;
    const AtomicString& FastGetAttribute(const QualifiedName &name) const;
    BlinKit::BkURL GetURLAttribute(const QualifiedName &name) const;
    bool FastHasAttribute(const QualifiedName &name) const;
    void setAttribute(const QualifiedName &name, const AtomicString &value);
    Attr* getAttributeNode(const AtomicString &name);
    Attr* getAttributeNodeNS(const AtomicString &namespaceURI, const AtomicString &localName);
    void SynchronizeAttribute(const AtomicString &localName) const;
    Attr* AttrIfExists(const QualifiedName &name);
    Attr* EnsureAttr(const QualifiedName &name);
    AttrNodeList* GetAttrNodeList(void);

    bool HasID(void) const;
    bool HasClass(void) const;
    const SpaceSplitString& ClassNames(void) const;
    bool HasClassName(const AtomicString &className) const;
    const AtomicString& GetIdAttribute(void) const;
    // Call this to get the value of the id attribute for style resolution
    // purposes.  The value will already be lowercased if the document is in
    // compatibility mode, so this function is not suitable for non-style uses.
    const AtomicString& IdForStyleResolution(void) const;

    const AtomicString& GetNameAttribute(void) const;

    AtomicString LowercaseIfNecessary(const AtomicString &name) const;

    const AtomicString& IsValue(void) const;
    void SetIsValue(const AtomicString &isValue);

    String TextFromChildren(void) const;

    void SetStyleAffectedByEmpty(void) { SetElementFlag(ElementFlags::kStyleAffectedByEmpty); }

    Element* CloneWithChildren(Document *nullableFactory = nullptr) const;
    Element* CloneWithoutChildren(Document *nullableFactory = nullptr) const;
    void CloneAttributesFrom(const Element &other);
    // Step 5 of https://dom.spec.whatwg.org/#concept-node-clone
    virtual void CloneNonAttributePropertiesFrom(const Element &source, CloneChildrenFlag flag) {} // BKTODO: Check overrides for child classes

    bool IsInDescendantTreeOf(const Element *shadowHost) const;

    AtomicString ComputeInheritedLanguage(void) const;

    // Used for disabled form elements; if true, prevents mouse events from being
    // dispatched to event listeners, and prevents DOMActivate events from being
    // sent at all.
    virtual bool IsDisabledFormControl(void) const { return false; }
    virtual bool IsOptionalFormControl(void) const { return false; }
    virtual bool IsRequiredFormControl(void) const { return false; }
    virtual bool IsScriptElement(void) const { return false; }
    virtual bool IsValidElement(void) { return false; }
    // Elements that may have an insertion mode other than "in body" should
    // override this and return true.
    // https://html.spec.whatwg.org/multipage/parsing.html#reset-the-insertion-mode-appropriately
    virtual bool HasNonInBodyInsertionMode(void) const { return false; } // BKTODO: Check overrides
    virtual bool ShouldAppearIndeterminate(void) const { return false; }

    virtual bool MatchesDefaultPseudoClass(void) const { return false; }
    virtual bool MatchesEnabledPseudoClass(void) const { return false; }
    virtual bool MatchesReadOnlyPseudoClass(void) const { return false; }
    virtual bool MatchesReadWritePseudoClass(void) const { return false; }
    virtual bool MatchesValidityPseudoClasses(void) const { return false; }

    virtual bool IsURLAttribute(const Attribute &attribute) const { return false; }

    enum class AttributeModificationReason { kDirectly, kByParser, kByCloning };
    struct AttributeModificationParams {
        STACK_ALLOCATED();
    public:
        AttributeModificationParams(const QualifiedName &qname, const AtomicString &oldValue, const AtomicString &newValue, AttributeModificationReason reason)
            : name(qname), oldValue(oldValue), newValue(newValue), reason(reason)
        {
        }

        const QualifiedName &name;
        const AtomicString &oldValue;
        const AtomicString &newValue;
        const AttributeModificationReason reason;
    };
    virtual void AttributeChanged(const AttributeModificationParams &params);
    virtual void ParseAttribute(const AttributeModificationParams &params);
    void ParserSetAttributes(const Vector<Attribute> &attributeVector);
    void StripScriptingAttributes(Vector<Attribute> &attributeVector) const;

    void BeginParsingChildren(void) { SetIsFinishedParsingChildren(false); }
    virtual void FinishParsingChildren(void);

    // Node overrides
    String nodeName(void) const override;
    NodeType getNodeType(void) const final { return kElementNode; }
    Node* Clone(Document &document, CloneChildrenFlag flag) const override;
#ifndef BLINKIT_CRAWLER_ONLY
    void DefaultEventHandler(Event &event) override;
#endif
    InsertionNotificationRequest InsertedInto(ContainerNode &insertionPoint) override;
protected:
    Element(const QualifiedName &tagName, Document *document, ConstructionType type);

    const ElementData* GetElementData(void) const { return m_elementData.get(); }

    virtual void ParserDidSetAttributes(void) {}

    virtual NamedItemType GetNamedItemType(void) const { return NamedItemType::kNone; }

    // ContainerNode overrides
#ifndef BLINKIT_CRAWLER_ONLY
    void ChildrenChanged(const ChildrenChange &change) override;
#endif
    // classAttributeChanged() exists to share code between
    // parseAttribute (called via setAttribute()) and
    // svgAttributeChanged (called when element.className.baseValue is set)
    void ClassAttributeChanged(const AtomicString &newClassString);
private:
    ElementRareData* GetElementRareData(void) const;
    ElementRareData& EnsureElementRareData(void);
    UniqueElementData& EnsureUniqueElementData(void);

    void SetElementFlag(ElementFlags mask, bool value = true);

    virtual Element* CloneWithoutAttributesAndChildren(Document &factory) const;

    void DetachAllAttrNodesFromElement(void);
    void SynchronizeAllAttributes(void) const;
    void SynchronizeAttribute(const QualifiedName &name) const;
    void UpdateId(const AtomicString &oldId, const AtomicString &newId);
    void UpdateId(TreeScope &scope, const AtomicString &oldId, const AtomicString &newId);
    void UpdateName(const AtomicString &oldName, const AtomicString &newName);
    void UpdateNamedItemRegistration(NamedItemType type, const AtomicString &oldName, const AtomicString &newName);
    void UpdateIdNamedItemRegistration(NamedItemType type, const AtomicString &oldName, const AtomicString &newName);

    enum SynchronizationOfLazyAttribute {
        kNotInSynchronizationOfLazyAttribute = 0,
        kInSynchronizationOfLazyAttribute
    };
    void WillModifyAttribute(const QualifiedName &name, const AtomicString &oldValue, const AtomicString &newValue);
    void AppendAttributeInternal(const QualifiedName &name, const AtomicString &value,
        SynchronizationOfLazyAttribute inSynchronizationOfLazyAttribute);
    void SetAttributeInternal(wtf_size_t index, const QualifiedName &name, const AtomicString &newValue,
        SynchronizationOfLazyAttribute inSynchronizationOfLazyAttribute);
    void DidAddAttribute(const QualifiedName &name, const AtomicString &value);
    void DidModifyAttribute(const QualifiedName &name, const AtomicString &oldValue, const AtomicString &newValue);

    // Node overrides
    bool ChildTypeAllowed(NodeType type) const final;

    QualifiedName m_tagName;

    std::shared_ptr<ElementData> m_elementData;
};

DEFINE_NODE_TYPE_CASTS(Element, IsElementNode());

template <typename T>
bool IsElementOfType(const Node &);

template <>
inline bool IsElementOfType<const Element>(const Node &node)
{
    return node.IsElementNode();
}
template <typename T>
inline bool IsElementOfType(const Element &element)
{
    return IsElementOfType<T>(static_cast<const Node &>(element));
}
template <>
inline bool IsElementOfType<const Element>(const Element &)
{
    return true;
}

// Type casting.
template <typename T>
inline T& ToElement(Node &node)
{
    ASSERT(IsElementOfType<const T>(node));
    return static_cast<T &>(node);
}
template <typename T>
inline T* ToElement(Node *node)
{
    ASSERT(!node || IsElementOfType<const T>(*node));
    return static_cast<T *>(node);
}
template <typename T>
inline const T& ToElement(const Node &node)
{
    ASSERT(IsElementOfType<const T>(node));
    return static_cast<const T &>(node);
}
template <typename T>
inline const T* ToElement(const Node *node)
{
    ASSERT(!node || IsElementOfType<const T>(*node));
    return static_cast<const T *>(node);
}

inline bool IsShadowHost(const Node *node)
{
#ifdef BLINKIT_CRAWLER_ONLY
    return false;
#else
    return nullptr != node && nullptr != node->GetShadowRoot();
#endif
}

}  // namespace blink

#endif // BLINKIT_BLINK_ELEMENT_H
