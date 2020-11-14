// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: element.cpp
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
 *           (C) 2007 David Smith (catfish.man@gmail.com)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2012, 2013 Apple Inc.
 * All rights reserved.
 *           (C) 2007 Eric Seidel (eric@webkit.org)
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

#include "element.h"

#include "third_party/blink/renderer/bindings/core/duk/duk_event_listener.h"
#include "third_party/blink/renderer/core/dom/attr.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/dom/document_fragment.h"
#include "third_party/blink/renderer/core/dom/element_data.h"
#include "third_party/blink/renderer/core/dom/element_data_cache.h"
#include "third_party/blink/renderer/core/dom/element_rare_data.h"
#include "third_party/blink/renderer/core/dom/mutation_observer_interest_group.h"
#include "third_party/blink/renderer/core/dom/text.h"
#include "third_party/blink/renderer/core/editing/serializers/serialization.h"
#include "third_party/blink/renderer/core/event_type_names.h"
#include "third_party/blink/renderer/core/html_names.h"
#include "third_party/blink/renderer/core/html/parser/html_parser_idioms.h"
#include "third_party/blink/renderer/platform/runtime_enabled_features.h"
#include "third_party/blink/renderer/platform/wtf/not_found.h"
#include "third_party/blink/renderer/platform/wtf/text/string_builder.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "third_party/blink/renderer/core/layout/layout_view.h"
#endif

using namespace BlinKit;

namespace blink {

enum class ClassStringContent { kEmpty, kWhiteSpaceOnly, kHasClasses };

Element::Element(const QualifiedName &tagName, Document *document, ConstructionType type)
    : ContainerNode(document, type), m_tagName(tagName)
{
    // Nothing
}

Element::~Element(void)
{
#ifndef BLINKIT_CRAWLER_ONLY
    ASSERT(NeedsAttach());
#endif
}

void Element::AppendAttributeInternal(
    const QualifiedName &name, const AtomicString &value,
    SynchronizationOfLazyAttribute inSynchronizationOfLazyAttribute)
{
    if (!inSynchronizationOfLazyAttribute)
        WillModifyAttribute(name, g_null_atom, value);
    EnsureUniqueElementData().Attributes().Append(name, value);
    if (!inSynchronizationOfLazyAttribute)
        DidAddAttribute(name, value);
}

#ifndef BLINKIT_CRAWLER_ONLY
void Element::AttachLayoutTree(AttachContext &context)
{
    ASSERT(false); // BKTODO:
}
#endif

Attr* Element::AttrIfExists(const QualifiedName &name)
{
    if (AttrNodeList *attrNodeList = GetAttrNodeList())
    {
        for (const auto &attr : *attrNodeList)
        {
            if (attr->GetQualifiedName().Matches(name))
                return attr;
        }
    }
    return nullptr;
}

static inline AtomicString MakeIdForStyleResolution(const AtomicString &value, bool inQuirksMode)
{
    if (inQuirksMode)
        return value.LowerASCII();
    return value;
}

void Element::AttributeChanged(const AttributeModificationParams &params)
{
    const QualifiedName &name = params.name;
#ifndef BLINKIT_CRAWLER_ONLY
    const bool forUI = !ForCrawler();
    if (forUI)
    {
        if (ShadowRoot *parentShadowRoot = ShadowRootWhereNodeCanBeDistributedForV0(*this))
        {
            ASSERT(false); // BKTODO:
#if 0
            if (ShouldInvalidateDistributionWhenAttributeChanged(
                *parent_shadow_root, name, params.new_value))
                parent_shadow_root->SetNeedsDistributionRecalc();
#endif
        }
        ASSERT(name.LocalNameUpper() != "SLOT"); // BKTODO:
#if 0
        if (name == HTMLNames::slotAttr && params.old_value != params.new_value) {
            if (ShadowRoot* root = V1ShadowRootOfParent())
                root->DidChangeHostChildSlotName(params.old_value, params.new_value);
        }
#endif
    }
#endif

    ParseAttribute(params);

    GetDocument().IncDOMTreeVersion();

    if (name == html_names::kIdAttr)
    {
        AtomicString oldId = GetElementData()->IdForStyleResolution();
        AtomicString newId = MakeIdForStyleResolution(params.new_value, GetDocument().InQuirksMode());
        if (newId != oldId)
        {
            GetElementData()->SetIdForStyleResolution(newId);
#ifndef BLINKIT_CRAWLER_ONLY
            if (forUI)
                GetDocument().GetStyleEngine().IdChangedForElement(oldId, newId, *this);
#endif
        }
    }
    else if (name == html_names::kClassAttr)
    {
        ClassAttributeChanged(params.new_value);
        if (HasRareData())
        {
            ASSERT(false); // BKTODO:
#if 0
            if (GetElementRareData()->GetClassList())
            {
                GetElementRareData()->GetClassList()->DidUpdateAttributeValue(
                    params.old_value, params.new_value);
            }
#endif
        }
    }
    else if (name == html_names::kNameAttr)
    {
        SetHasName(!params.new_value.IsNull());
    }
#ifndef BLINKIT_CRAWLER_ONLY
    else if (forUI)
    {
        BKLOG("// BKTODO: Process attribute `%s`", name.LocalName().StdUtf8().c_str());
#if 0
        if (name == HTMLNames::partAttr)
        {
            if (RuntimeEnabledFeatures::CSSPartPseudoElementEnabled()) {
                EnsureElementRareData().SetPart(params.new_value);
                GetDocument().GetStyleEngine().PartChangedForElement(*this);
            }
        }
        else if (name == HTMLNames::partmapAttr)
        {
            if (RuntimeEnabledFeatures::CSSPartPseudoElementEnabled()) {
                EnsureElementRareData().SetPartNamesMap(params.new_value);
                GetDocument().GetStyleEngine().PartmapChangedForElement(*this);
            }
        }
        else if (IsStyledElement())
        {
            if (name == styleAttr) {
                StyleAttributeChanged(params.new_value, params.reason);
            }
            else if (IsPresentationAttribute(name)) {
                GetElementData()->presentation_attribute_style_is_dirty_ = true;
                SetNeedsStyleRecalc(kLocalStyleChange,
                    StyleChangeReasonForTracing::FromAttribute(name));
            }
            else if (RuntimeEnabledFeatures::InvisibleDOMEnabled() &&
                name == HTMLNames::invisibleAttr &&
                params.old_value != params.new_value) {
                InvisibleAttributeChanged(params.old_value, params.new_value);
            }
        }
#endif
    }
#endif

    InvalidateNodeListCachesInAncestors(&name, this, nullptr);

#ifndef BLINKIT_CRAWLER_ONLY
    if (forUI)
    {
        ASSERT(name.LocalNameUpper() != "TABINDEX"); // BKTODO:
#if 0
        if (params.reason == AttributeModificationReason::kDirectly &&
            name == tabindexAttr && AdjustedFocusedElementInTreeScope() == this)
        {
            // The attribute change may cause supportsFocus() to return false
            // for the element which had focus.
            //
            // TODO(tkent): We should avoid updating style.  We'd like to check only
            // DOM-level focusability here.
            GetDocument().UpdateStyleAndLayoutTreeForNode(this);
            if (!SupportsFocus())
                blur();
        }
#endif
    }
#endif
}

AttributeCollection Element::Attributes(void) const
{
    if (const ElementData *elementData = GetElementData())
    {
        SynchronizeAllAttributes();
        return elementData->Attributes();
    }
    return AttributeCollection();
}

NamedNodeMap* Element::attributes(void) const
{
    ElementRareData &rareData = const_cast<Element *>(this)->EnsureElementRareData();
    if (NamedNodeMap *attributeMap = rareData.AttributeMap())
        return attributeMap;

    std::unique_ptr<NamedNodeMap> attributeMap = NamedNodeMap::Create(const_cast<Element *>(this));
    rareData.SetAttributeMap(attributeMap);
    return rareData.AttributeMap();
}

AttributeCollection Element::AttributesWithoutUpdate(void) const
{
    if (!GetElementData())
        return AttributeCollection();
    return GetElementData()->Attributes();
}

#ifndef BLINKIT_CRAWLER_ONLY

void Element::CancelFocusAppearanceUpdate(void)
{
    ASSERT(!ForCrawler());
    if (GetDocument().FocusedElement() == this)
        ASSERT(false); // BKTODO: GetDocument().CancelFocusAppearanceUpdate();
}

namespace {

bool HasSiblingsForNonEmpty(const Node *sibling, Node* (*nextFunc)(const Node &))
{
    while (nullptr != sibling)
    {
        if (sibling->IsElementNode())
            return true;
        if (sibling->IsTextNode() && !ToText(sibling)->data().IsEmpty())
            return true;
        sibling = nextFunc(*sibling);
    }
    return false;
}

}  // namespace

void Element::CheckForEmptyStyleChange(const Node *nodeBeforeChange, const Node *nodeAfterChange)
{
    if (!InActiveDocument())
        return;
    if (!StyleAffectedByEmpty())
        return;
    if (HasSiblingsForNonEmpty(nodeBeforeChange, NodeTraversal::PreviousSibling)
        || HasSiblingsForNonEmpty(nodeAfterChange, NodeTraversal::NextSibling))
    {
        return;
    }
    PseudoStateChanged(CSSSelector::kPseudoEmpty);
}

void Element::ChildrenChanged(const ChildrenChange &change)
{
    ContainerNode::ChildrenChanged(change);

    CheckForEmptyStyleChange(change.siblingBeforeChange, change.siblingAfterChange);

    if (!change.byParser && change.IsChildElementChange())
    {
        auto changeType = kElementRemoved == change.type
            ? kSiblingElementRemoved
            : kSiblingElementInserted;
        CheckForSiblingStyleChanges(changeType, ToElement(change.siblingChanged),
            change.siblingBeforeChange, change.siblingAfterChange);
    }

    if (ShadowRoot *shadowRoot = GetShadowRoot())
        shadowRoot->SetNeedsDistributionRecalcWillBeSetNeedsAssignmentRecalc();
}
#endif

bool Element::ChildTypeAllowed(NodeType type) const
{
    switch (type)
    {
        case kElementNode:
        case kTextNode:
        case kCommentNode:
        case kProcessingInstructionNode:
        case kCdataSectionNode:
            return true;
        default:
            break;
    }
    return false;
}

template <typename CharacterType>
static inline ClassStringContent ClassStringHasClassName(const CharacterType *characters, unsigned length)
{
    ASSERT(length > 0);

    unsigned i = 0;
    do {
        if (IsNotHTMLSpace<CharacterType>(characters[i]))
            break;
        ++i;
    } while (i < length);

    if (i == length && length >= 1)
        return ClassStringContent::kWhiteSpaceOnly;

    return ClassStringContent::kHasClasses;
}

static inline ClassStringContent ClassStringHasClassName(const AtomicString &newClassString)
{
    unsigned length = newClassString.length();

    if (!length)
        return ClassStringContent::kEmpty;

    if (newClassString.Is8Bit())
        return ClassStringHasClassName(newClassString.Characters8(), length);
    return ClassStringHasClassName(newClassString.Characters16(), length);
}

void Element::ClassAttributeChanged(const AtomicString &newClassString)
{
    const ElementData *elementData = GetElementData();
    ASSERT(nullptr != elementData);

    ClassStringContent classStringContentType = ClassStringHasClassName(newClassString);
    const bool shouldFoldCase = GetDocument().InQuirksMode();
    if (classStringContentType == ClassStringContent::kHasClasses)
    {
#ifdef BLINKIT_CRAWLER_ONLY
        elementData->SetClass(newClassString, shouldFoldCase);
#else
        if (ForCrawler())
        {
            elementData->SetClass(newClassString, shouldFoldCase);
        }
        else
        {
            const SpaceSplitString oldClasses = elementData->ClassNames();
            elementData->SetClass(newClassString, shouldFoldCase);
            const SpaceSplitString &newClasses = elementData->ClassNames();
            GetDocument().GetStyleEngine().ClassChangedForElement(oldClasses, newClasses, *this);
        }
#endif
    }
    else
    {
#ifndef BLINKIT_CRAWLER_ONLY
        if (!ForCrawler())
        {
            const SpaceSplitString &oldClasses = elementData->ClassNames();
            GetDocument().GetStyleEngine().ClassChangedForElement(oldClasses, *this);
        }
#endif
        if (classStringContentType == ClassStringContent::kWhiteSpaceOnly)
            elementData->SetClass(newClassString, shouldFoldCase);
        else
            elementData->ClearClass();
    }
}

const SpaceSplitString& Element::ClassNames(void) const
{
    ASSERT(HasClass());
    ASSERT(GetElementData());
    return GetElementData()->ClassNames();
}

void Element::CloneAttributesFrom(const Element &other)
{
    if (HasRareData())
        DetachAllAttrNodesFromElement();

    other.SynchronizeAllAttributes();
    if (!other.m_elementData)
    {
        m_elementData.reset();
        return;
    }

    const AtomicString &oldId = GetIdAttribute();
    const AtomicString &newId = other.GetIdAttribute();

    if (!oldId.IsNull() || !newId.IsNull())
        UpdateId(oldId, newId);

    const AtomicString &oldName = GetNameAttribute();
    const AtomicString &newName = other.GetNameAttribute();

    if (!oldName.IsNull() || !newName.IsNull())
        UpdateName(oldName, newName);

    // Quirks mode makes class and id not case sensitive. We can't share the
    // ElementData if the idForStyleResolution and the className need different
    // casing.
    bool ownerDocumentsHaveDifferentCaseSensitivity = false;
    if (other.HasClass() || other.HasID())
        ownerDocumentsHaveDifferentCaseSensitivity = other.GetDocument().InQuirksMode() != GetDocument().InQuirksMode();

    // If 'other' has a mutable ElementData, convert it to an immutable one so we
    // can share it between both elements.
    // We can only do this if there are no presentation attributes and sharing the
    // data won't result in different case sensitivity of class or id.
    if (other.m_elementData->IsUnique() && !ownerDocumentsHaveDifferentCaseSensitivity
        && nullptr == other.m_elementData->PresentationAttributeStyle())
    {
        const_cast<Element &>(other).m_elementData = ToUniqueElementData(other.m_elementData.get())->MakeShareableCopy();
    }

    ASSERT(GetDocument() == other.GetDocument()); // Old logic from NeedsURLResolutionForInlineStyle
    if (!other.m_elementData->IsUnique() && !ownerDocumentsHaveDifferentCaseSensitivity)
        m_elementData = other.m_elementData;
    else
        m_elementData = other.m_elementData->MakeUniqueCopy();

    for (const Attribute &attr : m_elementData->Attributes())
    {
        AttributeModificationParams params(attr.GetName(), g_null_atom, attr.Value(), AttributeModificationReason::kByCloning);
        AttributeChanged(params);
    }
}

Node* Element::Clone(Document &document, CloneChildrenFlag flag) const
{
    return flag == CloneChildrenFlag::kClone
        ? CloneWithChildren(&document)
        : CloneWithoutChildren(&document);
}

Element* Element::CloneWithChildren(Document *factory) const
{
    if (nullptr == factory)
        factory = &GetDocument();

    Element *clone = CloneWithoutAttributesAndChildren(*factory);

    clone->CloneAttributesFrom(*this);
    clone->CloneNonAttributePropertiesFrom(*this, CloneChildrenFlag::kClone);
    clone->CloneChildNodesFrom(*this);
    return clone;
}

Element* Element::CloneWithoutAttributesAndChildren(Document &factory) const
{
    ASSERT(!IsScriptElement()); // BKTODO:
    return factory.CreateElement(localName(), CreateElementFlags::ByCloneNode());
}

Element* Element::CloneWithoutChildren(Document *nullableFactory) const
{
    ASSERT(false); // BKTODO:
    return nullptr;
}

AtomicString Element::ComputeInheritedLanguage(void) const
{
    const Node *n = this;
    AtomicString value;
    // The language property is inherited, so we iterate over the parents to find
    // the first language.
    do {
        if (n->IsElementNode())
        {
            if (const ElementData *elementData = ToElement(n)->GetElementData())
            {
                AttributeCollection attributes = elementData->Attributes();
                if (const Attribute *attribute = attributes.Find(html_names::kLangAttr))
                    value = attribute->Value();
            }
        }
        else if (auto *document = DynamicTo<Document>(n))
        {
            // checking the MIME content-language
            value = document->ContentLanguage();
        }

        n = n->ParentOrShadowHostNode();
    } while (nullptr != n && value.IsNull());

    return value;
}

#ifndef BLINKIT_CRAWLER_ONLY
scoped_refptr<ComputedStyle> Element::CustomStyleForLayoutObject(void)
{
    ASSERT(false); // BKTODO: Check child classes.
    return nullptr;
}

void Element::DefaultEventHandler(Event &event)
{
    ASSERT(false); // BKTODO:
#if 0
    if (RuntimeEnabledFeatures::InvisibleDOMEnabled() &&
        event.type() == EventTypeNames::activateinvisible &&
        event.target() == this) {
        removeAttribute(invisibleAttr);
        event.SetDefaultHandled();
        return;
    }
#endif
    ContainerNode::DefaultEventHandler(event);
}
#endif

void Element::DetachAllAttrNodesFromElement(void)
{
    ASSERT(false); // BKTODO:
}

#ifndef BLINKIT_CRAWLER_ONLY
void Element::DetachLayoutTree(const AttachContext &context)
{
    CancelFocusAppearanceUpdate();
    RemoveCallbackSelectors();
    if (HasRareData())
    {
        ASSERT(false); // BKTODO:
#if 0
        ElementRareData* data = GetElementRareData();
        if (!context.performing_reattach)
            data->ClearPseudoElements();

        // attachLayoutTree() will clear the computed style for us when inside
        // recalcStyle.
        if (!GetDocument().InStyleRecalc())
            data->ClearComputedStyle();

        if (ElementAnimations* element_animations = data->GetElementAnimations()) {
            if (context.performing_reattach) {
                // FIXME: We call detach from within style recalc, so compositingState
                // is not up to date.
                // https://code.google.com/p/chromium/issues/detail?id=339847
                DisableCompositingQueryAsserts disabler;

                // FIXME: restart compositor animations rather than pull back to the
                // main thread
                element_animations->RestartAnimationOnCompositor();
            }
            else {
                element_animations->CssAnimations().Cancel();
                element_animations->SetAnimationStyleChange(false);
            }
            element_animations->ClearBaseComputedStyle();
        }

        DetachPseudoElement(kPseudoIdBefore, context);

        if (ShadowRoot* shadow_root = data->GetShadowRoot())
            shadow_root->DetachLayoutTree(context);
#endif
    }

    ContainerNode::DetachLayoutTree(context);

    DetachPseudoElement(kPseudoIdAfter, context);
    DetachPseudoElement(kPseudoIdBackdrop, context);
    DetachPseudoElement(kPseudoIdFirstLetter, context);

    if (!context.performing_reattach && IsUserActionElement())
    {
        ASSERT(false); // BKTODO:
#if 0
        if (IsHovered())
            GetDocument().HoveredElementDetached(*this);
        if (InActiveChain())
            GetDocument().ActiveChainNodeDetached(*this);
        GetDocument().UserActionElements().DidDetach(*this);
#endif
    }

    if (context.clear_invalidation)
    {
        GetDocument().GetStyleEngine().GetPendingNodeInvalidations().ClearInvalidation(*this);
    }

    SetNeedsResizeObserverUpdate();

    DCHECK(NeedsAttach());
}

void Element::DetachPseudoElement(PseudoId pseudoId, const AttachContext &context)
{
    if (PseudoElement *pseudoElement = GetPseudoElement(pseudoId))
        ASSERT(false); // BKTODO: pseudo_element->DetachLayoutTree(context);
}
#endif

void Element::DidAddAttribute(const QualifiedName &name, const AtomicString &value)
{
    if (name == html_names::kIdAttr)
        UpdateId(g_null_atom, value);
    AttributeChanged(AttributeModificationParams(name, g_null_atom, value, AttributeModificationReason::kDirectly));
    DispatchSubtreeModifiedEvent();
}

void Element::DidModifyAttribute(const QualifiedName &name, const AtomicString &oldValue, const AtomicString &newValue)
{
    ASSERT(false); // BKTODO:
}

#ifndef BLINKIT_CRAWLER_ONLY
void Element::DidRecalcStyle(StyleRecalcChange change)
{
    ASSERT(false); // BKTODO: Check child classes.
}
#endif

Attr* Element::EnsureAttr(const QualifiedName &name)
{
    Attr *attrNode = AttrIfExists(name);
    if (nullptr == attrNode)
    {
        attrNode = Attr::Create(*this, name);
        GetTreeScope().AdoptIfNeeded(*attrNode);
        EnsureElementRareData().AddAttr(attrNode);
    }
    return attrNode;
}

ElementRareData& Element::EnsureElementRareData(void)
{
    return static_cast<ElementRareData &>(EnsureRareData());
}

UniqueElementData& Element::EnsureUniqueElementData(void)
{
    if (!m_elementData)
        m_elementData = UniqueElementData::Create();
    else if (!m_elementData->IsUnique())
        m_elementData = ToShareableElementData(m_elementData.get())->MakeUniqueCopy();
    return ToUniqueElementData(*m_elementData);
}

const AtomicString& Element::FastGetAttribute(const QualifiedName &name) const
{
    if (const ElementData *elementData = GetElementData())
    {
        if (const Attribute *attribute = elementData->Attributes().Find(name))
            return attribute->Value();
    }
    return g_null_atom;
}

bool Element::FastHasAttribute(const QualifiedName &name) const
{
    if (const ElementData *elementData = GetElementData())
        return elementData->Attributes().FindIndex(name) != kNotFound;
    return false;
}

void Element::FinishParsingChildren(void)
{
    SetIsFinishedParsingChildren(true);
#ifndef BLINKIT_CRAWLER_ONLY
    if (!ForCrawler())
    {
        CheckForEmptyStyleChange(this, this);
        CheckForSiblingStyleChanges(kFinishedParsingChildren, nullptr, lastChild(), nullptr);
    }
#endif
}

ElementAnimations* Element::GetElementAnimations(void) const
{
    if (HasRareData())
        return GetElementRareData()->GetElementAnimations();
    return nullptr;
}

AttrNodeList* Element::GetAttrNodeList(void)
{
    return HasRareData() ? GetElementRareData()->GetAttrNodeList() : nullptr;
}

const AtomicString& Element::getAttribute(const QualifiedName &name) const
{
    if (const ElementData *elementData = GetElementData())
    {
        SynchronizeAttribute(name);
        if (const Attribute *attribute = elementData->Attributes().Find(name))
            return attribute->Value();
    }
    return g_null_atom;
}

Attr* Element::getAttributeNode(const AtomicString &name)
{
    const ElementData *elementData = GetElementData();
    if (nullptr == elementData)
        return nullptr;

    SynchronizeAttribute(name);
    const Attribute* attribute = elementData->Attributes().Find(LowercaseIfNecessary(name));
    if (nullptr == attribute)
        return nullptr;

    return EnsureAttr(attribute->GetName());
}

Attr* Element::getAttributeNodeNS(const AtomicString &namespaceURI, const AtomicString &localName)
{
    ASSERT(false); // BKTODO:
    return nullptr;
}

ElementRareData* Element::GetElementRareData(void) const
{
    ASSERT(HasRareData());
    return static_cast<ElementRareData *>(RareData());
}

const AtomicString& Element::GetIdAttribute(void) const
{
    return HasID() ? FastGetAttribute(html_names::kIdAttr) : g_null_atom;
}

const AtomicString& Element::GetNameAttribute(void) const
{
    return HasName() ? FastGetAttribute(html_names::kNameAttr) : g_null_atom;
}

#ifndef BLINKIT_CRAWLER_ONLY
PseudoElement* Element::GetPseudoElement(PseudoId pseudoid) const
{
    return HasRareData() ? GetElementRareData()->GetPseudoElement(pseudoid) : nullptr;
}

ShadowRoot* Element::GetShadowRoot(void) const
{
    return HasRareData() ? GetElementRareData()->GetShadowRoot() : nullptr;
}
#endif

GURL Element::GetURLAttribute(const QualifiedName &name) const
{
#if DCHECK_IS_ON()
    if (const ElementData *elementData = GetElementData())
    {
        if (const Attribute *attribute = elementData->Attributes().Find(name))
            ASSERT(IsURLAttribute(*attribute));
    }
#endif
    return GetDocument().CompleteURL(StripLeadingAndTrailingHTMLSpaces(getAttribute(name)));
}

#ifndef BLINKIT_CRAWLER_ONLY
bool Element::HasAnimations(void) const
{
    ASSERT(!ForCrawler());
    if (!HasRareData())
        return false;

    if (ElementAnimations *elementAnimations = GetElementRareData()->GetElementAnimations())
        return !elementAnimations->IsEmpty();
    return false;
}
#endif

bool Element::hasAttribute(const AtomicString &name) const
{
    if (const ElementData *elementData = GetElementData())
    {
        SynchronizeAttribute(name);
        return GetElementData()->Attributes().FindIndex(LowercaseIfNecessary(name)) != kNotFound;
    }
    return false;
}

bool Element::hasAttribute(const QualifiedName &name) const
{
    ASSERT(false); // BKTODO:
    return false;
}

bool Element::HasClass(void) const
{
    if (const ElementData *elementData = GetElementData())
        return elementData->HasClass();
    return false;
}

bool Element::HasClassName(const AtomicString &className) const
{
    return HasClass() && ClassNames().Contains(className);
}

bool Element::HasElementFlagInternal(ElementFlags mask) const
{
    return GetElementRareData()->HasElementFlag(mask);
}

bool Element::HasID(void) const
{
    if (const ElementData *elementData = GetElementData())
        return elementData->HasID();
    return false;
}

const AtomicString& Element::IdForStyleResolution(void) const
{
    ASSERT(HasID());
    return GetElementData()->IdForStyleResolution();
}

String Element::innerHTML(void) const
{
    return CreateMarkup(this, kChildrenOnly);
}

Node::InsertionNotificationRequest Element::InsertedInto(ContainerNode &insertionPoint)
{
    // need to do superclass processing first so isConnected() is true
    // by the time we reach updateId
    ContainerNode::InsertedInto(insertionPoint);

    ASSERT(!HasRareData() || !GetElementRareData()->HasPseudoElements());

    if (!insertionPoint.IsInTreeScope())
        return kInsertionDone;

    if (HasRareData())
    {
        ElementRareData *rareData = GetElementRareData();
        if (rareData->IntersectionObserverData() && rareData->IntersectionObserverData()->HasObservations())
        {
            ASSERT(false); // BKTODO:
#if 0
            GetDocument().EnsureIntersectionObserverController().AddTrackedTarget(
                *this);
            if (LocalFrameView* frame_view = GetDocument().View())
                frame_view->SetIntersectionObservationState(LocalFrameView::kRequired);
#endif
        }
    }

#ifndef BLINKIT_CRAWLER_ONLY
    if (isConnected())
    {
#if 0 // BKTODO: Process custom element logic.
        if (GetCustomElementState() == CustomElementState::kCustom)
            CustomElement::EnqueueConnectedCallback(this);
        else if (IsUpgradedV0CustomElement())
            V0CustomElement::DidAttach(this, GetDocument());
        else if (GetCustomElementState() == CustomElementState::kUndefined)
            CustomElement::TryToUpgrade(this);
#endif
    }
#endif

    TreeScope &scope = insertionPoint.GetTreeScope();
#ifdef BLINKIT_CRAWLER_ONLY
    ASSERT(GetTreeScope() == scope);
#else
    if (GetTreeScope() != scope)
        return kInsertionDone;
#endif

    const AtomicString &idValue = GetIdAttribute();
    if (!idValue.IsNull())
        UpdateId(scope, g_null_atom, idValue);

    const AtomicString &nameValue = GetNameAttribute();
    if (!nameValue.IsNull())
        UpdateName(g_null_atom, nameValue);

    return kInsertionDone;
}

#ifndef BLINKIT_CRAWLER_ONLY
bool Element::IsFocusable(void) const
{
    ASSERT(false); // BKTODO:
    return false;
}
#endif

bool Element::IsInDescendantTreeOf(const Element *shadowHost) const
{
#ifdef BLINKIT_CRAWLER_ONLY
    NOTREACHED();
#else
    DCHECK(nullptr != shadowHost);
    DCHECK(IsShadowHost(shadowHost));

    for (const Element *ancestorShadowHost = OwnerShadowHost();
        nullptr != ancestorShadowHost;
        ancestorShadowHost = ancestorShadowHost->OwnerShadowHost())
    {
        if (ancestorShadowHost == shadowHost)
            return true;
    }
#endif
    return false;
}

const AtomicString& Element::IsValue(void) const
{
    if (HasRareData())
        return GetElementRareData()->IsValue();
    return g_null_atom;
}

#ifndef BLINKIT_CRAWLER_ONLY
bool Element::LayoutObjectIsNeeded(const ComputedStyle &style) const
{
    ASSERT(false); // BKTODO: Check child classes.
    return false;
}
#endif

AtomicString Element::LowercaseIfNecessary(const AtomicString &name) const
{
    return name.LowerASCII();
}

String Element::nodeName(void) const
{
    return m_tagName.ToString();
}

#ifndef BLINKIT_CRAWLER_ONLY
const ComputedStyle* Element::NonLayoutObjectComputedStyle(void) const
{
    if (NeedsReattachLayoutTree())
        return GetNonAttachedStyle();

    if (!HasRareData())
        return nullptr;

    return GetElementRareData()->GetComputedStyle();
}

scoped_refptr<ComputedStyle> Element::OriginalStyleForLayoutObject(void)
{
    ASSERT(!ForCrawler());
    ASSERT(GetDocument().InStyleRecalc());
    return GetDocument().EnsureStyleResolver().StyleForElement(this);
}
#endif

String Element::outerHTML(void) const
{
    return CreateMarkup(this, kIncludeNode);
}

void Element::ParseAttribute(const AttributeModificationParams &params)
{
    AttributeTriggers *triggers = TriggersForAttributeName(params.name);
    if (nullptr == triggers)
        return;

    if (triggers->event != g_null_atom)
    {
        std::shared_ptr<EventListener> eventListener = DukEventListener::CreateAttributeEventListener(this, params.name, params.new_value);
        SetAttributeEventListener(triggers->event, eventListener.get());
    }

    if (nullptr != triggers->function)
        (this->*(triggers->function))(params);
}

void Element::ParserSetAttributes(const Vector<Attribute> &attributeVector)
{
    ASSERT(!isConnected());
    ASSERT(nullptr == parentNode());
    ASSERT(!m_elementData);

    if (!attributeVector.IsEmpty())
    {
        if (ElementDataCache *elementDataCache = GetDocument().GetElementDataCache())
            m_elementData = elementDataCache->CachedShareableElementDataWithAttributes(attributeVector);
        else
            ASSERT(false); // BKTODO: m_elementData = ShareableElementData::CreateWithAttributes(attributeVector);
    }

    ParserDidSetAttributes();

    for (const auto &attribute : attributeVector)
    {
        AttributeModificationParams params(attribute.GetName(), g_null_atom, attribute.Value(), AttributeModificationReason::kByParser);
        AttributeChanged(params);
    }
}

#ifndef BLINKIT_CRAWLER_ONLY
scoped_refptr<ComputedStyle> Element::PropagateInheritedProperties(StyleRecalcChange change)
{
    if (change != kIndependentInherit)
        return nullptr;
    if (IsPseudoElement())
        return nullptr;
    if (NeedsStyleRecalc())
        return nullptr;
    if (HasAnimations())
        return nullptr;

    const ComputedStyle *parentStyle = ParentComputedStyle();
    ASSERT(nullptr != parentStyle);

    const ComputedStyle *style = GetComputedStyle();
    if (nullptr == style || style->Animations() || style->Transitions())
        return nullptr;

    scoped_refptr<ComputedStyle> newStyle = ComputedStyle::Clone(*style);
    newStyle->PropagateIndependentInheritedProperties(*parentStyle);
    return newStyle;
}

void Element::PseudoStateChanged(CSSSelector::PseudoType pseudo)
{
    // We can't schedule invaliation sets from inside style recalc otherwise
    // we'd never process them.
    // TODO(esprehn): Make this an ASSERT and fix places that call into this
    // like HTMLSelectElement.
    if (GetDocument().InStyleRecalc())
        return;
    GetDocument().GetStyleEngine().PseudoStateChangedForElement(pseudo, *this);
}

StyleRecalcChange Element::RecalcOwnStyle(StyleRecalcChange change)
{
    ASSERT(GetDocument().InStyleRecalc());
    ASSERT(change >= kIndependentInherit || NeedsStyleRecalc());
    ASSERT(nullptr != ParentComputedStyle());
    ASSERT(nullptr == GetNonAttachedStyle());

    scoped_refptr<const ComputedStyle> oldStyle = GetComputedStyle();

    // When propagating inherited changes, we don't need to do a full style recalc
    // if the only changed properties are independent. In this case, we can simply
    // set these directly on the ComputedStyle object.
    scoped_refptr<ComputedStyle> newStyle = PropagateInheritedProperties(change);
    if (!newStyle)
        newStyle = StyleForLayoutObject();
    if (!newStyle)
    {
        DCHECK(IsPseudoElement());
        SetNeedsReattachLayoutTree();
        return kReattach;
    }

    StyleRecalcChange localChange = ComputedStyle::StylePropagationDiff(oldStyle.get(), newStyle.get());
    if (localChange != kNoChange)
    {
        if (this == GetDocument().documentElement())
        {
            if (GetDocument().GetStyleEngine().UpdateRemUnits(oldStyle.get(), newStyle.get()))
            {
                // Trigger a full document recalc on rem unit changes. We could keep
                // track of which elements depend on rem units like we do for viewport
                // styles, but we assume root font size changes are rare and just
                // recalculate everything.
                if (localChange < kForce)
                    localChange = kForce;
            }
        }
    }

    if (kReattach == change || kReattach == localChange)
    {
        SetNonAttachedStyle(newStyle);
        SetNeedsReattachLayoutTree();
        return kReattach;
    }

    ASSERT(oldStyle);

    if (localChange != kNoChange)
        UpdateCallbackSelectors(oldStyle.get(), newStyle.get());

    if (LayoutObject *layoutObject = GetLayoutObject())
    {
        // kNoChange may mean that the computed style didn't change, but there are
        // additional flags in ComputedStyle which may have changed. For instance,
        // the AffectedBy* flags. We don't need to go through the visual
        // invalidation diffing in that case, but we replace the old ComputedStyle
        // object with the new one to ensure the mentioned flags are up to date.
        if (localChange == kNoChange)
            layoutObject->SetStyleInternal(newStyle.get());
        else
            layoutObject->SetStyle(newStyle.get());
    }
    else
    {
        if (ShouldStoreNonLayoutObjectComputedStyle(*newStyle))
            StoreNonLayoutObjectComputedStyle(newStyle);
        else if (HasRareData())
            GetElementRareData()->ClearComputedStyle();
    }

    if (GetStyleChangeType() >= kSubtreeStyleChange)
        return kForce;

    if (change > kInherit || localChange > kInherit)
        return std::max(localChange, change);

    if (localChange < kIndependentInherit)
    {
        if (oldStyle->HasChildDependentFlags())
        {
            if (ChildNeedsStyleRecalc())
                return kInherit;
            newStyle->CopyChildDependentFlagsFrom(*oldStyle);
        }

        if (oldStyle->HasPseudoElementStyle() || newStyle->HasPseudoElementStyle())
            return kUpdatePseudoElements;
    }

    return localChange;
}

void Element::RecalcStyle(StyleRecalcChange change)
{
    ASSERT(!ForCrawler());
    ASSERT(GetDocument().InStyleRecalc());
    ASSERT(!GetDocument().Lifecycle().InDetach());
    // If we are re-attaching in a Shadow DOM v0 tree, we recalc down to the
    // distributed nodes to propagate kReattach down the flat tree (See
    // V0InsertionPoint::DidRecalcStyle). That means we may have a shadow-
    // including parent (V0InsertionPoint) with dirty recalc bit in the case where
    // fallback content has been redistributed to a different insertion point.
    // This will not happen for Shadow DOM v1 because we walk assigned nodes and
    // slots themselves are assigned and part of the flat tree.
    ASSERT(!ParentOrShadowHostNode()->NeedsStyleRecalc() || (ParentOrShadowHostNode()->IsV0InsertionPoint() && change == kReattach));
    ASSERT(InActiveDocument());

    if (HasCustomStyleCallbacks())
        WillRecalcStyle(change);

    if (change >= kIndependentInherit || NeedsStyleRecalc())
    {
        if (HasRareData())
        {
            ElementRareData *data = GetElementRareData();
            if (change != kIndependentInherit)
            {
                // We keep the old computed style around for display: contents, option
                // and optgroup. This way we can call stylePropagationDiff accurately.
                //
                // We could clear it always, but we'd have more expensive restyles for
                // children.
                //
                // Note that we can't just keep stored other kind of non-layout object
                // computed style (like the one that gets set when getComputedStyle is
                // called on a display: none element), because that is a sizable memory
                // hit.
                //
                // Also, we don't want to leave a stale computed style, which may happen
                // if we don't end up calling recalcOwnStyle because there's no parent
                // style.
                const ComputedStyle *nonLayoutStyle = NonLayoutObjectComputedStyle();
                if (nullptr == nonLayoutStyle || !ShouldStoreNonLayoutObjectComputedStyle(*nonLayoutStyle)
                    || nullptr == ParentComputedStyle())
                {
                    data->ClearComputedStyle();
                }
            }
            if (change >= kIndependentInherit)
            {
                if (ElementAnimations *elementAnimations = data->GetElementAnimations())
                    elementAnimations->SetAnimationStyleChange(false);
            }
        }

        if (nullptr != ParentComputedStyle())
        {
            change = RecalcOwnStyle(change);
        }
        else if (!CanParticipateInFlatTree())
        {
            // Recalculate style for Shadow DOM v0 <content> insertion point.
            // It does not take style since it's not part of the flat tree, but we
            // need to traverse into fallback children for reattach.
            if (NeedsAttach())
                change = kReattach;
            if (change == kReattach)
                SetNeedsReattachLayoutTree();
            else if (GetStyleChangeType() == kSubtreeStyleChange)
                change = kForce;
        }

        // Needed because the RebuildLayoutTree code needs to see what the
        // StyleChangeType() was on reattach roots. See Node::ReattachLayoutTree()
        // for an example.
        if (change != kReattach)
            ClearNeedsStyleRecalc();
    }

    if (change >= kUpdatePseudoElements || ChildNeedsStyleRecalc())
    {
        ASSERT(false); // BKTODO:
#if 0
        // ChildrenCanHaveStyle(), hence ShouldCallRecalcStyleForChildren(),
        // returns false for <object> elements below. Yet, they may have ::backdrop
        // elements.
        UpdatePseudoElement(kPseudoIdBackdrop, change);
#endif
    }

    if (ShouldCallRecalcStyleForChildren(change))
    {
        ASSERT(false); // BKTODO:
#if 0
        UpdatePseudoElement(kPseudoIdBefore, change);

        if (change > kUpdatePseudoElements || ChildNeedsStyleRecalc())
        {
            SelectorFilterParentScope filter_scope(*this);
            if (ShadowRoot* root = GetShadowRoot()) {
                if (root->ShouldCallRecalcStyle(change))
                    root->RecalcStyle(change);
            }
            RecalcDescendantStyles(change);
        }

        UpdatePseudoElement(kPseudoIdAfter, change);

        // If we are re-attaching us or any of our descendants, we need to attach
        // the descendants before we know if this element generates a ::first-letter
        // and which element the ::first-letter inherits style from.
        if (change < kReattach && !ChildNeedsReattachLayoutTree())
            UpdateFirstLetterPseudoElement(StyleUpdatePhase::kRecalc);
#endif

        ClearChildNeedsStyleRecalc();
    }

    if (HasCustomStyleCallbacks())
        DidRecalcStyle(change);
}

void Element::RecalcStyleForTraversalRootAncestor(void)
{
    ASSERT(false); // BKTODO:
}

void Element::RemoveCallbackSelectors(void)
{
    UpdateCallbackSelectors(GetComputedStyle(), nullptr);
}
#endif

void Element::setAttribute(const QualifiedName &name, const AtomicString &value)
{
    SynchronizeAttribute(name);
    wtf_size_t index = GetElementData()
        ? GetElementData()->Attributes().FindIndex(name)
        : kNotFound;
    SetAttributeInternal(index, name, value, kNotInSynchronizationOfLazyAttribute);
}

void Element::setAttribute(const AtomicString &localName, const AtomicString &value, ExceptionState &exceptionState)
{
    if (!Document::IsValidName(localName))
    {
        exceptionState.ThrowDOMException(DOMExceptionCode::kInvalidCharacterError,
            "'" + localName + "' is not a valid attribute name.");
        return;
    }

    SynchronizeAttribute(localName);
    AtomicString caseAdjustedLocalName = LowercaseIfNecessary(localName);

    if (nullptr == GetElementData())
    {
        QualifiedName qName(g_null_atom, caseAdjustedLocalName, g_null_atom);
        SetAttributeInternal(kNotFound, qName, value, kNotInSynchronizationOfLazyAttribute);
        return;
    }

    AttributeCollection attributes = GetElementData()->Attributes();
    wtf_size_t index = attributes.FindIndex(caseAdjustedLocalName);
    const QualifiedName qName = index != kNotFound
        ? attributes[index].GetName()
        : QualifiedName(g_null_atom, caseAdjustedLocalName, g_null_atom);
    SetAttributeInternal(index, qName, value, kNotInSynchronizationOfLazyAttribute);
}

void Element::SetAttributeInternal(
    wtf_size_t index,
    const QualifiedName &name, const AtomicString &newValue,
    SynchronizationOfLazyAttribute inSynchronizationOfLazyAttribute)
{
    if (newValue.IsNull())
    {
        ASSERT(false); // BKTODO:
#if 0
        if (index != kNotFound)
            RemoveAttributeInternal(index, inSynchronizationOfLazyAttribute);
#endif
        return;
    }

    if (index == kNotFound)
    {
        AppendAttributeInternal(name, newValue, inSynchronizationOfLazyAttribute);
        return;
    }

    const Attribute &existingAttribute = GetElementData()->Attributes().at(index);
    AtomicString existingAttributeValue = existingAttribute.Value();
    QualifiedName existingAttributeName = existingAttribute.GetName();

    if (!inSynchronizationOfLazyAttribute)
        WillModifyAttribute(existingAttributeName, existingAttributeValue, newValue);
    if (newValue != existingAttributeValue)
        EnsureUniqueElementData().Attributes().at(index).SetValue(newValue);
    if (!inSynchronizationOfLazyAttribute)
        DidModifyAttribute(existingAttributeName, existingAttributeValue, newValue);
}

void Element::SetElementFlag(ElementFlags mask, bool value)
{
    if (!HasRareData() && !value)
        return;
    EnsureElementRareData().SetElementFlag(mask, value);
}

void Element::setInnerHTML(const String &html, NodeVector &detachedChildren, ExceptionState &exceptionState)
{
    if (html.IsEmpty() && !HasNonInBodyInsertionMode())
    {
        setTextContent(html, detachedChildren);
    }
    else
    {
        DocumentFragment *fragment = CreateFragmentForInnerOuterHTML(html, this, kAllowScriptingContent, "innerHTML", exceptionState);
        if (nullptr != fragment)
        {
            ContainerNode *container = this;
            ASSERT(!HasTagName(html_names::kTemplateTag)); // BKTODO:
#if 0
            if (auto* template_element = ToHTMLTemplateElementOrNull(*this))
                container = template_element->content();
#endif
            ReplaceChildrenWithFragment(container, fragment, detachedChildren, exceptionState);
        }
    }
}

void Element::setOuterHTML(const String &html, NodeVector &detachedNodes, ExceptionState &exceptionState)
{
    Node *p = parentNode();
    if (nullptr == p)
    {
        exceptionState.ThrowDOMException(DOMExceptionCode::kNoModificationAllowedError,
            "This element has no parent node.");
        return;
    }
    if (!p->IsElementNode())
    {
        exceptionState.ThrowDOMException(DOMExceptionCode::kNoModificationAllowedError,
            "This element's parent is of type '" + p->nodeName() + "', which is not an element node.");
        return;
    }

    Element *parent = ToElement(p);
    Node *prev = previousSibling();
    Node *next = nextSibling();

    DocumentFragment *fragment = CreateFragmentForInnerOuterHTML(html, parent, kAllowScriptingContent, "outerHTML", exceptionState);
    if (exceptionState.HadException())
        return;

    parent->ReplaceChild(fragment, this, detachedNodes, exceptionState);
    Node *node = nullptr != next ? next->previousSibling() : nullptr;
    if (!exceptionState.HadException() && nullptr != node && node->IsTextNode())
        MergeWithNextTextNode(ToText(node), detachedNodes, exceptionState);

    if (!exceptionState.HadException() && nullptr != prev && prev->IsTextNode())
        MergeWithNextTextNode(ToText(prev), detachedNodes, exceptionState);
}

void Element::SetIsValue(const AtomicString &isValue)
{
    ASSERT(false); // BKTODO:
}

#ifndef BLINKIT_CRAWLER_ONLY
void Element::SetNeedsResizeObserverUpdate(void)
{
    ASSERT(!HasRareData()); // BKTODO:
}

const AtomicString& Element::ShadowPseudoId(void) const
{
    ASSERT(false); // BKTODO: Check child classes.
    return g_null_atom;
}

bool Element::ShouldCallRecalcStyleForChildren(StyleRecalcChange change)
{
    if (change != kReattach)
        return change >= kUpdatePseudoElements || ChildNeedsStyleRecalc();
    ASSERT(false); // BKTODO:
#if 0
    if (!ChildrenCanHaveStyle())
        return false;
    if (const ComputedStyle *newStyle = GetNonAttachedStyle())
        return LayoutObjectIsNeeded(*newStyle) || ShouldStoreNonLayoutObjectComputedStyle(*newStyle);
#endif
    return !CanParticipateInFlatTree();
}
#endif

bool Element::ShouldSerializeEndTag(void) const
{
    using namespace html_names;
    // See https://www.w3.org/TR/DOM-Parsing/
    if (HasTagName(kAreaTag) || HasTagName(kBaseTag) || HasTagName(kBasefontTag)
        || HasTagName(kBgsoundTag) || HasTagName(kBrTag) || HasTagName(kColTag)
        || HasTagName(kEmbedTag) || HasTagName(kFrameTag) || HasTagName(kHrTag)
        || HasTagName(kImgTag) || HasTagName(kInputTag) || HasTagName(kKeygenTag)
        || HasTagName(kLinkTag) || HasTagName(kMetaTag) || HasTagName(kParamTag)
        || HasTagName(kSourceTag) || HasTagName(kTrackTag) || HasTagName(kWbrTag))
    {
        return false;
    }
    return true;
}

#ifndef BLINKIT_CRAWLER_ONLY
bool Element::ShouldStoreNonLayoutObjectComputedStyle(const ComputedStyle &style) const
{
#if DCHECK_IS_ON()
    if (style.Display() == EDisplay::kContents && !NeedsReattachLayoutTree())
        DCHECK(!GetLayoutObject() || IsPseudoElement());
#endif
    if (style.Display() == EDisplay::kNone)
        return false;
#if 0 // BKTODO: Remove this later.
    if (IsSVGElement()) {
        Element* parent_element = LayoutTreeBuilderTraversal::ParentElement(*this);
        if (parent_element && !parent_element->IsSVGElement())
            return false;
        if (IsSVGStopElement(*this))
            return true;
    }
#endif
    if (style.Display() == EDisplay::kContents)
        return true;
    ASSERT(false); // BKTODO:
    return false;
#if 0
    return IsHTMLOptGroupElement(*this) || IsHTMLOptionElement(*this);
#endif
}

void Element::StoreNonLayoutObjectComputedStyle(scoped_refptr<ComputedStyle> style)
{
    ASSERT(style);
    ASSERT(ShouldStoreNonLayoutObjectComputedStyle(*style));
    EnsureElementRareData().SetComputedStyle(std::move(style));
}
#endif

void Element::StripScriptingAttributes(Vector<Attribute> &attributeVector) const
{
    ASSERT(false); // BKTODO:
}

#ifndef BLINKIT_CRAWLER_ONLY
scoped_refptr<ComputedStyle> Element::StyleForLayoutObject(void)
{
    ASSERT(GetDocument().InStyleRecalc());

    // FIXME: Instead of clearing updates that may have been added from calls to
    // StyleForElement outside RecalcStyle, we should just never set them if we're
    // not inside RecalcStyle.
    if (ElementAnimations *elementAnimations = GetElementAnimations())
        elementAnimations->CssAnimations().ClearPendingUpdate();

#if 0 // BKTODO: Enable later.
    if (RuntimeEnabledFeatures::InvisibleDOMEnabled() &&
        hasAttribute(HTMLNames::invisibleAttr)) {
        auto style = ComputedStyle::Create();
        style->SetDisplay(EDisplay::kNone);
        return style;
    }
#endif

    scoped_refptr<ComputedStyle> style = HasCustomStyleCallbacks()
        ? CustomStyleForLayoutObject()
        : OriginalStyleForLayoutObject();
    if (!style)
    {
        ASSERT(IsPseudoElement());
        return nullptr;
    }

    // StyleForElement() might add active animations so we need to get it again.
    if (ElementAnimations *elementAnimations = GetElementAnimations())
    {
        elementAnimations->CssAnimations().MaybeApplyPendingUpdate(this);
        elementAnimations->UpdateAnimationFlags(*style);
    }

    if (style->HasTransform())
    {
        if (const CSSPropertyValueSet *inlineStyle = InlineStyle())
        {
            style->SetHasInlineTransform(
                inlineStyle->HasProperty(CSSPropertyTransform)
                || inlineStyle->HasProperty(CSSPropertyTranslate)
                || inlineStyle->HasProperty(CSSPropertyRotate)
                || inlineStyle->HasProperty(CSSPropertyScale));
        }
    }

    style->UpdateIsStackingContext(this == GetDocument().documentElement(),
        IsInTopLayer(),
        false); // BKTODO: IsSVGForeignObjectElement(*this));

    return style;
}

bool Element::SupportsFocus(void) const
{
    ASSERT(false); // BKTODO:
    return false;
#if 0
    // FIXME: supportsFocus() can be called when layout is not up to date.
    // Logic that deals with the layoutObject should be moved to
    // layoutObjectIsFocusable().
    // But supportsFocus must return true when the element is editable, or else
    // it won't be focusable. Furthermore, supportsFocus cannot just return true
    // always or else tabIndex() will change for all HTML elements.
    return HasElementFlag(ElementFlags::kTabIndexWasSetExplicitly) ||
        IsRootEditableElement(*this) ||
        (IsShadowHost(this) && AuthorShadowRoot() &&
            AuthorShadowRoot()->delegatesFocus()) ||
        SupportsSpatialNavigationFocus();
#endif
}
#endif

void Element::SynchronizeAllAttributes(void) const
{
#ifndef BLINKIT_CRAWLER_ONLY
    if (!GetElementData())
        return;
    // NOTE: AnyAttributeMatches in selector_checker.cc currently assumes that all
    // lazy attributes have a null namespace.  If that ever changes we'll need to
    // fix that code.
    if (GetElementData()->style_attribute_is_dirty_)
    {
        ASSERT(false); // BKTODO:
#if 0
        DCHECK(IsStyledElement());
        SynchronizeStyleAttributeInternal();
#endif
    }
#endif
}

void Element::SynchronizeAttribute(const AtomicString &localName) const
{
#ifndef BLINKIT_CRAWLER_ONLY
    // This version of synchronizeAttribute() is streamlined for the case where
    // you don't have a full QualifiedName, e.g when called from DOM API.
    if (!GetElementData())
        return;
    ASSERT(false); // BKTODO:
#if 0
    if (GetElementData()->style_attribute_is_dirty_ &&
        LowercaseIfNecessary(localName) == kStyleAttr.LocalName()) {
        DCHECK(IsStyledElement());
        SynchronizeStyleAttributeInternal();
        return;
    }
#endif
#endif
}

void Element::SynchronizeAttribute(const QualifiedName &name) const
{
#ifndef BLINKIT_CRAWLER_ONLY
    if (!GetElementData())
        return;
    ASSERT(false); // BKTODO:
#if 0
    if (UNLIKELY(name == styleAttr &&
        GetElementData()->style_attribute_is_dirty_)) {
        DCHECK(IsStyledElement());
        SynchronizeStyleAttributeInternal();
        return;
    }
#endif
#endif
}

String Element::TextFromChildren(void) const
{
    Text *firstTextNode = nullptr;
    bool foundMultipleTextNodes = false;
    unsigned totalLength = 0;

    for (Node *child = firstChild(); nullptr != child; child = child->nextSibling())
    {
        if (!child->IsTextNode())
            continue;
        Text *text = ToText(child);
        if (nullptr == firstTextNode)
            firstTextNode = text;
        else
            foundMultipleTextNodes = true;
        unsigned length = text->data().length();
        if (length > std::numeric_limits<unsigned>::max() - totalLength)
            return g_empty_string;
        totalLength += length;
    }

    if (nullptr == firstTextNode)
        return g_empty_string;

    if (nullptr != firstTextNode && !foundMultipleTextNodes)
    {
        firstTextNode->Atomize();
        return firstTextNode->data();
    }

    StringBuilder content;
    content.ReserveCapacity(totalLength);
    for (Node *child = firstTextNode; nullptr != child; child = child->nextSibling())
    {
        if (!child->IsTextNode())
            continue;
        content.Append(ToText(child)->data());
    }

    ASSERT(content.length() == totalLength);
    return content.ToString();
}

Element::AttributeTriggers* Element::TriggersForAttributeName(const QualifiedName &attrName)
{
    const AtomicString &kNoEvent = g_null_atom;
    static AttributeTriggers attributeTriggers[] = {
        { html_names::kOnloadAttr, event_type_names::kLoad, nullptr },
    };

    static std::unordered_map<QualifiedName, uint32_t> s_attributeToTriggerIndexMap;
    if (s_attributeToTriggerIndexMap.empty())
    {
        for (uint32_t i = 0; i < std::size(attributeTriggers); ++i)
            s_attributeToTriggerIndexMap[attributeTriggers[i].attribute] = i;
    }

    auto it = s_attributeToTriggerIndexMap.find(attrName);
    if (std::end(s_attributeToTriggerIndexMap) != it)
        return &attributeTriggers[it->second];
    return nullptr;
}

#ifndef BLINKIT_CRAWLER_ONLY
void Element::UpdateCallbackSelectors(const ComputedStyle *oldStyle, const ComputedStyle *newStyle)
{
    Vector<String> emptyVector;
    const Vector<String> &oldCallbackSelectors = nullptr != oldStyle ? oldStyle->CallbackSelectors() : emptyVector;
    const Vector<String> &newCallbackSelectors = nullptr != newStyle ? newStyle->CallbackSelectors() : emptyVector;
    if (oldCallbackSelectors.IsEmpty() && newCallbackSelectors.IsEmpty())
        return;
    ASSERT(false); // BKTODO:
#if 0
    if (oldCallbackSelectors != newCallbackSelectors)
        CSSSelectorWatch::From(GetDocument())
        .UpdateSelectorMatches(old_callback_selectors, new_callback_selectors);
#endif
}
#endif

void Element::UpdateId(const AtomicString &oldId, const AtomicString &newId)
{
    if (!IsInTreeScope())
        return;

    if (oldId == newId)
        return;

    UpdateId(ContainingTreeScope(), oldId, newId);
}

void Element::UpdateId(TreeScope &scope, const AtomicString &oldId, const AtomicString &newId)
{
    ASSERT(IsInTreeScope());
    ASSERT(oldId != newId);

    if (!oldId.IsEmpty())
        scope.RemoveElementById(oldId, *this);
    if (!newId.IsEmpty())
        scope.AddElementById(newId, *this);

    NamedItemType type = GetNamedItemType();
    if (NamedItemType::kNameOrId == type || NamedItemType::kNameOrIdWithName == type)
        UpdateIdNamedItemRegistration(type, oldId, newId);
}

void Element::UpdateIdNamedItemRegistration(NamedItemType type, const AtomicString &oldName, const AtomicString &newName)
{
    ASSERT(false); // BKTODO:
}

void Element::UpdateName(const AtomicString &oldName, const AtomicString &newName)
{
    if (!IsInDocumentTree())
        return;

    if (oldName == newName)
        return;

    NamedItemType type = GetNamedItemType();
    if (type != NamedItemType::kNone)
        UpdateNamedItemRegistration(type, oldName, newName);
}

void Element::UpdateNamedItemRegistration(NamedItemType type, const AtomicString &oldName, const AtomicString &newName)
{
    ASSERT(false); // BKTODO:
}

void Element::WillModifyAttribute(const QualifiedName &name, const AtomicString &oldValue, const AtomicString &newValue)
{
    if (name == html_names::kNameAttr)
        UpdateName(oldValue, newValue);

#ifndef BLINKIT_CRAWLER_ONLY
    ASSERT(false); // BKTODO:
#if 0
    if (GetCustomElementState() == CustomElementState::kCustom) {
        CustomElement::EnqueueAttributeChangedCallback(this, name, old_value,
            new_value);
    }

    if (old_value != new_value) {
        GetDocument().GetStyleEngine().AttributeChangedForElement(name, *this);
        if (IsUpgradedV0CustomElement()) {
            V0CustomElement::AttributeDidChange(this, name.LocalName(), old_value,
                new_value);
        }
    }
#endif
#endif

    if (MutationObserverInterestGroup *recipients = MutationObserverInterestGroup::CreateForAttributesMutation(*this, name))
        ASSERT(false); // BKTODO: recipients->EnqueueMutationRecord(MutationRecord::CreateAttributes(this, name, oldValue));
}

#ifndef BLINKIT_CRAWLER_ONLY
void Element::WillRecalcStyle(StyleRecalcChange change)
{
    ASSERT(false); // BKTODO:
}
#endif

} // namespace blink
