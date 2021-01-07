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
#include "third_party/blink/renderer/core/dom/names_map.h"
#include "third_party/blink/renderer/core/dom/qualified_name.h"
#include "url/gurl.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "third_party/blink/renderer/core/css/css_primitive_value.h"
#   include "third_party/blink/renderer/core/dom/whitespace_attacher.h"
#   include "third_party/blink/renderer/core/resize_observer/resize_observer_data_map.h"
#endif

namespace blink {

class Attr;
class Attribute;
class ElementData;
class ElementRareData;
class NamedNodeMap;
#ifndef BLINKIT_CRAWLER_ONLY
class ElementAnimations;
class FocusOptions;
class InputDeviceCapabilities;
class MutableCSSPropertyValueSet;
class PseudoElement;
class ScrollStateCallback;
#endif

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

#ifndef BLINKIT_CRAWLER_ONLY
enum class SelectionBehaviorOnFocus {
    kReset,
    kRestore,
    kNone,
};
#endif

typedef std::vector<Attr *> AttrNodeList;

class Element : public ContainerNode
{
public:
    ~Element(void) override;
    void Trace(Visitor *visitor) override;

    // Exports for JS
    NamedNodeMap* attributes(void) const;
    bool hasAttribute(const AtomicString &name) const;
    String innerHTML(void) const;
    String outerHTML(void) const;
    void setAttribute(const AtomicString &localName, const AtomicString &value, ExceptionState &exceptionState);
    void setInnerHTML(const String &html, ExceptionState &exceptionState);
    void setOuterHTML(const String &html, ExceptionState &exceptionState);
    String tagName(void) const { return nodeName(); }

    const QualifiedName& TagQName(void) const { return m_tagName; }
    const AtomicString& prefix(void) const { return m_tagName.Prefix(); }
    const AtomicString& localName(void) const { return m_tagName.LocalName(); }
#ifndef BLINKIT_CRAWLER_ONLY
    AtomicString LocalNameForSelectorMatching(void) const;
#endif
    const AtomicString& namespaceURI(void) const { return m_tagName.NamespaceURI(); }
    bool HasTagName(const QualifiedName &tagName) const { return m_tagName.Matches(tagName); }
    // A fast function for checking the local name against another atomic string.
    bool HasLocalName(const AtomicString &other) const { return m_tagName.LocalName() == other; }
    bool ShouldSerializeEndTag(void) const;

    AttributeCollection Attributes(void) const;
    AttributeCollection AttributesWithoutUpdate(void) const;
    const AtomicString& getAttribute(const QualifiedName &name) const;
    const AtomicString& FastGetAttribute(const QualifiedName &name) const;
    int GetIntegralAttribute(const QualifiedName &name) const;
    GURL GetURLAttribute(const QualifiedName &name) const;
    bool hasAttribute(const QualifiedName &name) const;
    bool FastHasAttribute(const QualifiedName &name) const;
    void setAttribute(const QualifiedName &name, const AtomicString &value);
    Attr* getAttributeNode(const AtomicString &name);
    Attr* getAttributeNodeNS(const AtomicString &namespaceURI, const AtomicString &localName);
    void SynchronizeAttribute(const AtomicString &localName) const;
    Attr* AttrIfExists(const QualifiedName &name);
    Attr* EnsureAttr(const QualifiedName &name);
    AttrNodeList* GetAttrNodeList(void);
#ifndef BLINKIT_CRAWLER_ONLY
    // Returns attributes that should be checked against Trusted Types
    virtual const HashSet<AtomicString>& GetCheckedAttributeNames(void) const;
    GURL GetNonEmptyURLAttribute(const QualifiedName &name) const;
#endif

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

    bool IsInTopLayer(void) const { return HasElementFlag(ElementFlags::kIsInTopLayer); }
    bool StyleAffectedByEmpty(void) const { return HasElementFlag(ElementFlags::kStyleAffectedByEmpty); }
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
    virtual bool HasNonInBodyInsertionMode(void) const { return false; }
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
            : name(qname), old_value(oldValue), new_value(newValue), reason(reason)
        {
        }

        const QualifiedName &name;
        const AtomicString &old_value;
        const AtomicString &new_value;
        const AttributeModificationReason reason;
    };
    virtual void AttributeChanged(const AttributeModificationParams &params);
    virtual void ParseAttribute(const AttributeModificationParams &params);
    void ParserSetAttributes(const Vector<Attribute> &attributeVector);
    void StripScriptingAttributes(Vector<Attribute> &attributeVector) const;

    void BeginParsingChildren(void) { SetIsFinishedParsingChildren(false); }
    virtual void FinishParsingChildren(void);

#ifndef BLINKIT_CRAWLER_ONLY
    virtual bool IsFormControlElement(void) const { return false; }
    virtual bool IsInRange(void) const { return false; }
    virtual bool IsOutOfRange(void) const { return false; }
    virtual bool IsSpinButtonElement(void) const { return false; }
    // This returns true for <textarea> and some types of <input>.
    virtual bool IsTextControl(void) const { return false; }

    Element* AdjustedFocusedElementInTreeScope(void) const;

    bool HasAnimations(void) const;
    ElementAnimations* GetElementAnimations(void) const;

    bool HasDisplayContentsStyle(void) const;

    void RecalcStyle(StyleRecalcChange change);
    void RecalcStyleForTraversalRootAncestor(void);

    const CSSPropertyValueSet* PresentationAttributeStyle(void);
    virtual const CSSPropertyValueSet* AdditionalPresentationAttributeStyle(void) { return nullptr; }

    const CSSPropertyValueSet* InlineStyle(void) const
    {
        ASSERT(!ForCrawler());
        if (const ElementData *elementData = GetElementData())
            return elementData->inline_style_.Get();
        return nullptr;
    }
    ComputedStyle* MutableNonLayoutObjectComputedStyle(void) const
    {
        return const_cast<ComputedStyle *>(NonLayoutObjectComputedStyle());
    }
    // Returns the Element¡¯s ComputedStyle. If the ComputedStyle is not already
    // stored on the Element, computes the ComputedStyle and stores it on the
    // Element¡¯s ElementRareData.  Used for getComputedStyle when Element is
    // display none.
    const ComputedStyle* EnsureComputedStyle(PseudoId pseudoElementSpecifier = kPseudoIdNone);
    const ComputedStyle* NonLayoutObjectComputedStyle(void) const;
    // FIXME: public for LayoutTreeBuilder, we shouldn't expose this though.
    scoped_refptr<ComputedStyle> StyleForLayoutObject(void);

    void SetAnimationStyleChange(bool animationStyleChange);

    bool ShouldStoreNonLayoutObjectComputedStyle(const ComputedStyle &style) const;
    void StoreNonLayoutObjectComputedStyle(scoped_refptr<ComputedStyle> style);

    // Whether this element can receive focus at all. Most elements are not
    // focusable but some elements, such as form controls and links, are. Unlike
    // layoutObjectIsFocusable(), this method may be called when layout is not up
    // to date, so it must not use the layoutObject to determine focusability.
    virtual bool SupportsFocus(void) const;
    // IsFocusable(), IsKeyboardFocusable(), and IsMouseFocusable() check
    // whether the element can actually be focused. Callers should ensure
    // ComputedStyle is up to date;
    // e.g. by calling Document::UpdateStyleAndLayoutTree().
    bool IsFocusable(void) const;
    virtual bool IsKeyboardFocusable(void) const { return IsFocusable() && tabIndex() >= 0; }
    virtual bool willValidate(void) const { return false; }
    virtual bool MayTriggerVirtualKeyboard(void) const;
    virtual void AccessKeyAction(bool /*sendToAnyEvent*/) {}

    virtual String title(void) const { return String(); }
    virtual String DefaultToolTip(void) const { return String(); }
    int tabIndex(void) const override;

    virtual void blur(void);

    virtual void DispatchFocusEvent(Element *oldFocusedElement, WebFocusType type,
        InputDeviceCapabilities* sourceSapabilities = nullptr);
    virtual void DispatchBlurEvent(Element *newFocusedElement, WebFocusType type,
        InputDeviceCapabilities *sourceCapabilities = nullptr);
    virtual void DispatchFocusInEvent(const AtomicString &eventType, Element *oldFocusedElement, WebFocusType type,
        InputDeviceCapabilities *sourceCapabilities = nullptr);

    virtual void UpdateFocusAppearanceWithOptions(SelectionBehaviorOnFocus selectionBehavior,
        const FocusOptions &options);

    // Returns the shadow root attached to this element if it is a shadow host.
    ShadowRoot* GetShadowRoot(void) const;
    ShadowRoot* ShadowRootIfV1(void) const;

    virtual const AtomicString& ShadowPseudoId(void) const;
    PseudoElement* GetPseudoElement(PseudoId pseudoid) const;
    void PseudoStateChanged(CSSSelector::PseudoType pseudo);

    ResizeObserverDataMap* ResizeObserverData(void) const;

    virtual bool IsPresentationAttribute(const QualifiedName &) const { return false; }
    virtual void CollectStyleForPresentationAttribute(const QualifiedName &name, const AtomicString &value, MutableCSSPropertyValueSet *style) {}
    virtual bool HasLegalLinkAttribute(const QualifiedName &) const { return false; }
    virtual const QualifiedName& SubResourceAttributeName(void) const { return QualifiedName::Null(); }

    virtual bool ShouldForceLegacyLayout(void) const { return false; }
    virtual scoped_refptr<ComputedStyle> CustomStyleForLayoutObject(void);
    virtual LayoutObject* CreateLayoutObject(const ComputedStyle &style);
    virtual bool LayoutObjectIsNeeded(const ComputedStyle &style) const;

    void RebuildLayoutTreeForTraversalRootAncestor(void) { RebuildFirstLetterLayoutTree(); }
    bool NeedsRebuildLayoutTree(const WhitespaceAttacher &whitespaceAttacher) const
    {
        // TODO(futhark@chromium.org): !CanParticipateInFlatTree() can be removed
        // when Shadow DOM V0 support is removed.
        return NeedsReattachLayoutTree() || ChildNeedsReattachLayoutTree() || !CanParticipateInFlatTree()
            || (whitespaceAttacher.TraverseIntoDisplayContents() && HasDisplayContentsStyle());
    }
    void RebuildLayoutTree(WhitespaceAttacher &whitespaceAttacher);

    void SetApplyScroll(ScrollStateCallback *scrollStateCallback);

    void SetNeedsCompositingUpdate(void);
    void SetNeedsResizeObserverUpdate(void);
#endif

    // Node overrides
    String nodeName(void) const override;
    NodeType getNodeType(void) const final { return kElementNode; }
    Node* Clone(Document &document, CloneChildrenFlag flag) const override;
#ifndef BLINKIT_CRAWLER_ONLY
    void DefaultEventHandler(Event &event) override;
#endif
protected:
    Element(const QualifiedName &tagName, Document *document, ConstructionType type);

    const ElementData* GetElementData(void) const { return m_elementData.Get(); }

    InsertionNotificationRequest InsertedInto(ContainerNode &insertionPoint) override;
    void RemovedFrom(ContainerNode &insertionPoint) override;

    virtual void ParserDidSetAttributes(void) {}

    virtual NamedItemType GetNamedItemType(void) const { return NamedItemType::kNone; }

    using AttributeChangedFunction = void (Element::*)(const Element::AttributeModificationParams &params);
    struct AttributeTriggers {
        const QualifiedName &attribute;
        const AtomicString &event;
        AttributeChangedFunction function;
    };
    virtual AttributeTriggers* TriggersForAttributeName(const QualifiedName &attrName);

#ifndef BLINKIT_CRAWLER_ONLY
    bool CanGeneratePseudoElement(PseudoId) const;

    scoped_refptr<ComputedStyle> OriginalStyleForLayoutObject(void);

    virtual bool ChildrenCanHaveStyle(void) const { return true; }
    virtual void WillRecalcStyle(StyleRecalcChange change);
    virtual void DidRecalcStyle(StyleRecalcChange change);

    void AddPropertyToPresentationAttributeStyle(MutableCSSPropertyValueSet *style, CSSPropertyID propertyId,
        CSSValueID identifier);
    void AddPropertyToPresentationAttributeStyle(MutableCSSPropertyValueSet *style, CSSPropertyID propertyId,
        double value, CSSPrimitiveValue::UnitType unit);
    void AddPropertyToPresentationAttributeStyle(MutableCSSPropertyValueSet *style, CSSPropertyID propertyId,
        const String &value);
    void AddPropertyToPresentationAttributeStyle(MutableCSSPropertyValueSet *style, CSSPropertyID propertyId,
        const CSSValue &value);

    void ChildrenChanged(const ChildrenChange &change) override;
    void AttachLayoutTree(AttachContext &context) override;
    void DetachLayoutTree(const AttachContext &context = AttachContext()) override;
#endif
    // classAttributeChanged() exists to share code between
    // parseAttribute (called via setAttribute()) and
    // svgAttributeChanged (called when element.className.baseValue is set)
    void ClassAttributeChanged(const AtomicString &newClassString);
private:
    ElementRareData* GetElementRareData(void) const;
    ElementRareData& EnsureElementRareData(void);
    UniqueElementData& EnsureUniqueElementData(void);

    bool HasElementFlag(ElementFlags mask) const
    {
        return HasRareData() && HasElementFlagInternal(mask);
    }
    void SetElementFlag(ElementFlags mask, bool value = true);
    bool HasElementFlagInternal(ElementFlags mask) const;

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

#ifndef BLINKIT_CRAWLER_ONLY
    StyleRecalcChange RecalcOwnStyle(StyleRecalcChange change);
    // If the only inherited changes in the parent element are independent,
    // these changes can be directly propagated to this element (the child).
    // If these conditions are met, propagates the changes to the current style
    // and returns the new style. Otherwise, returns null.
    scoped_refptr<ComputedStyle> PropagateInheritedProperties(StyleRecalcChange change);
    // Returns true if we should traverse shadow including children and pseudo
    // elements for RecalcStyle.
    const ComputedStyle* VirtualEnsureComputedStyle(PseudoId pseudoElementSpecifier = kPseudoIdNone) override
    {
        return EnsureComputedStyle(pseudoElementSpecifier);
    }

    bool ShouldCallRecalcStyleForChildren(StyleRecalcChange change);
    bool ShouldInvalidateDistributionWhenAttributeChanged(ShadowRoot &shadowRoot,
        const QualifiedName &name, const AtomicString &newValue);

    void UpdatePresentationAttributeStyle(void);

    void CancelFocusAppearanceUpdate(void);

    PseudoElement* CreatePseudoElementIfNeeded(PseudoId pseudoId);
    void AttachPseudoElement(PseudoId pseudoId, AttachContext &context);
    void UpdatePseudoElement(PseudoId pseudoId, StyleRecalcChange change);
    void DetachPseudoElement(PseudoId pseudoId, const AttachContext &context);

    enum class StyleUpdatePhase {
        kRecalc,
        kRebuildLayoutTree,
        kAttachLayoutTree,
    };
    void UpdateFirstLetterPseudoElement(StyleUpdatePhase phase);

    void RebuildPseudoElementLayoutTree(PseudoId pseudoId, WhitespaceAttacher &whitespaceAttacher);
    void RebuildFirstLetterLayoutTree(void);
    void RebuildShadowRootLayoutTree(WhitespaceAttacher &whitespaceAttacher);

    void AddCallbackSelectors(void);
    void UpdateCallbackSelectors(const ComputedStyle *oldStyle, const ComputedStyle *newStyle);
    void RemoveCallbackSelectors(void);

    void CheckForEmptyStyleChange(const Node *nodeBeforeChange, const Node *nodeAfterChange);

    // FIXME: Everyone should allow author shadows.
    virtual bool AreAuthorShadowsAllowed(void) const { return true; }
    virtual bool AlwaysCreateUserAgentShadowRoot(void) const { return false; }
#endif

    QualifiedName m_tagName;

    Member<ElementData> m_elementData;
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

template <typename T>
inline T* ToElementOrNull(Node &node)
{
    return IsElementOfType<const T>(node) ? static_cast<T *>(&node) : nullptr;
}

template <typename T>
inline T* ToElementOrNull(Node *node)
{
    return (nullptr != node && IsElementOfType<const T>(*node)) ? static_cast<T *>(node) : nullptr;
}

template <typename T>
inline const T* ToElementOrNull(const Node &node)
{
    return IsElementOfType<const T>(node) ? static_cast<const T *>(&node) : nullptr;
}

template <typename T>
inline const T* ToElementOrNull(const Node *node)
{
    return (nullptr != node && IsElementOfType<const T>(*node)) ? static_cast<const T *>(node) : nullptr;
}

inline bool IsShadowHost(const Node *node)
{
#ifdef BLINKIT_CRAWLER_ONLY
    return false;
#else
    return nullptr != node && nullptr != node->GetShadowRoot();
#endif
}

inline bool IsShadowHost(const Node &node)
{
#ifdef BLINKIT_CRAWLER_ONLY
    return false;
#else
    return nullptr != node.GetShadowRoot();
#endif
}

#ifndef BLINKIT_CRAWLER_ONLY
inline bool IsAtShadowBoundary(const Element *element)
{
    if (nullptr != element)
    {
        if (ContainerNode *parentNode = element->parentNode())
            return parentNode->IsShadowRoot();
    }
    return false;
}
#endif

}  // namespace blink

// These macros do the same as their NODE equivalents but additionally provide a
// template specialization for isElementOfType<>() so that the Traversal<> API
// works for these Element types.
#define DEFINE_ELEMENT_TYPE_CASTS(thisType, predicate)              \
    template <>                                                     \
    inline bool IsElementOfType<const thisType>(const Node& node) { \
        return node.predicate;                                      \
    }                                                               \
    DEFINE_NODE_TYPE_CASTS(thisType, predicate)

#define DEFINE_ELEMENT_TYPE_CASTS_WITH_FUNCTION(ThisType)           \
    template <>                                                     \
    inline bool IsElementOfType<const ThisType>(const Node &node) { \
        return Is##ThisType(node);                                  \
    }                                                               \
    DEFINE_NODE_TYPE_CASTS_WITH_FUNCTION(ThisType)

#endif // BLINKIT_BLINK_ELEMENT_H
