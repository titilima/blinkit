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
#include "third_party/blink/renderer/core/dom/container_node.h"
#include "third_party/blink/renderer/core/dom/element_data.h"
#include "third_party/blink/renderer/core/dom/qualified_name.h"

namespace blink {

class Attr;
class Attribute;
class ElementData;
class ElementRareData;

// https://html.spec.whatwg.org/multipage/dom.html#dom-document-nameditem-filter
enum class NamedItemType {
    kNone,
    kName,
    kNameOrId,
    kNameOrIdWithName,
};

class Element : public ContainerNode
{
public:
    ~Element(void) override;

    const QualifiedName& TagQName(void) const { return m_tagName; }
    const AtomicString& localName(void) const { return m_tagName.LocalName(); }
    const AtomicString& namespaceURI(void) const { return m_tagName.NamespaceURI(); }
    bool HasTagName(const QualifiedName &tagName) const { return m_tagName.Matches(tagName); }

    AttributeCollection Attributes(void) const;
    AttributeCollection AttributesWithoutUpdate(void) const;
    const AtomicString& getAttribute(const QualifiedName &name) const;
    const AtomicString& FastGetAttribute(const QualifiedName &name) const;
    void setAttribute(const QualifiedName &name, const AtomicString &value);
    Attr* getAttributeNode(const AtomicString &name);
    Attr* getAttributeNodeNS(const AtomicString &namespaceURI, const AtomicString &localName);

    bool HasID(void) const;
    const AtomicString& GetIdAttribute(void) const;

    const AtomicString& GetNameAttribute(void) const;

    AtomicString LowercaseIfNecessary(const AtomicString &name) const;

    virtual bool IsScriptElement(void) const { return false; }

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
private:
    ElementRareData* GetElementRareData(void) const;
    void UpdateId(TreeScope &scope, const AtomicString &oldId, const AtomicString &newId);
    void UpdateName(const AtomicString &oldName, const AtomicString &newName);
    void UpdateNamedItemRegistration(NamedItemType type, const AtomicString &oldName, const AtomicString &newName);

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

}  // namespace blink

#endif // BLINKIT_BLINK_ELEMENT_H
