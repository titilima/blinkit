// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: node.h
// Description: Node Class
//      Author: Ziming Li
//     Created: 2019-09-16
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2004-2011, 2014 Apple Inc. All rights reserved.
 * Copyright (C) 2008, 2009 Torch Mobile Inc. All rights reserved.
 * (http://www.torchmobile.com/)
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

#ifndef BLINKIT_BLINK_NODE_H
#define BLINKIT_BLINK_NODE_H

#pragma once

#include "third_party/blink/renderer/core/dom/events/event_target.h"
#include "third_party/blink/renderer/core/dom/node_rare_data.h"
#include "third_party/blink/renderer/core/dom/tree_scope.h"
#include "third_party/blink/renderer/core/html_names.h"
#include "third_party/blink/renderer/core/style/computed_style_constants.h"
#include "third_party/blink/renderer/platform/wtf/assertions.h"

namespace blink {

class Document;
class Element;
class ExceptionState;
class NodeList;
class NodeRareData;
class ShadowRoot;

const int kNodeStyleChangeShift = 18;
const int kNodeCustomElementShift = 20;

enum StyleChangeType {
    // This node does not need style recalculation.
    kNoStyleChange = 0,
    // This node needs style recalculation.
    kLocalStyleChange = 1 << kNodeStyleChangeShift,
    // This node and all of its flat-tree descendeants need style recalculation.
    kSubtreeStyleChange = 2 << kNodeStyleChangeShift,
    // This node and all of its descendants are detached and need style
    // recalculation.
    kNeedsReattachStyleChange = 3 << kNodeStyleChangeShift,
};

enum class CloneChildrenFlag { kClone, kSkip };

class Node : public EventTarget
{
    friend class TreeScope;
public:
    enum NodeType {
        kElementNode = 1,
        kAttributeNode = 2,
        kTextNode = 3,
        kCdataSectionNode = 4,
        kProcessingInstructionNode = 7,
        kCommentNode = 8,
        kDocumentNode = 9,
        kDocumentTypeNode = 10,
        kDocumentFragmentNode = 11,
    };

    ~Node(void) override;

    // Exports for JS
    Node* appendChild(Node *newChild, ExceptionState &exceptionState);
    Node* cloneNode(bool deep, ExceptionState &exceptionState) const;
    NodeList* childNodes(void);
    Node* firstChild(void) const;
    Node* insertBefore(Node *newChild, Node *refChild, ExceptionState &exceptionState);
    Node* lastChild(void) const;
    inline NodeType nodeType(void) const { return getNodeType(); }
    // Returns the DOM ownerDocument attribute. This method never returns null,
    // except in the case of a Document node.
    Document* ownerDocument(void) const;
    Node* removeChild(Node *child, ExceptionState &exceptionState);
    String textContent(bool convertBrsToNewlines = false) const;
    void setTextContent(const String &text);

    Document& GetDocument(void) const { return GetTreeScope().GetDocument(); }
    TreeScope& GetTreeScope(void) const
    {
        ASSERT(m_treeScope);
        return *m_treeScope;
    }
    TreeScope& ContainingTreeScope(void) const
    {
        ASSERT(IsInTreeScope());
        return *m_treeScope;
    }

    void RemoveAllEventListenersRecursively(void);

    // DOM methods & attributes for Node
    virtual String nodeName(void) const = 0;
    bool HasTagName(const HTMLQualifiedName &name) const;
    virtual String nodeValue(void) const;
    virtual void setNodeValue(const String &nodeValue);
    virtual NodeType getNodeType(void) const = 0;
    virtual Node* Clone(Document &document, CloneChildrenFlag flag) const = 0;
    ContainerNode* parentNode(void) const;
    ContainerNode* ParentOrShadowHostNode(void) const;
    Element* ParentOrShadowHostElement(void) const;
    Element* parentElement(void) const;
    ContainerNode* ParentElementOrShadowRoot(void) const;
    ContainerNode* ParentElementOrDocumentFragment(void) const;
    void SetParentOrShadowHostNode(ContainerNode *parent);
    Node* previousSibling(void) const { return m_previous; }
    void SetPreviousSibling(Node *previous) { m_previous = previous; }
    Node* nextSibling(void) const { return m_next; }
    void SetNextSibling(Node *next) { m_next = next; }
    bool hasChildren(void) const { return nullptr != firstChild(); }
    Node& TreeRoot(void) const;
    unsigned NodeIndex(void) const;
    unsigned CountChildren(void) const;
    bool contains(const Node *node) const;
    bool ContainsIncludingHostElements(const Node &node) const;

    // If this node is in a shadow tree, returns its shadow host. Otherwise,
    // returns nullptr.
    Element* OwnerShadowHost(void) const;
    // crbug.com/569532: containingShadowRoot() can return nullptr even if
    // isInShadowTree() returns true.
    // This can happen when handling queued events (e.g. during execCommand())
    ShadowRoot* ContainingShadowRoot(void) const;
    ShadowRoot* GetShadowRoot(void) const;

    Node* PseudoAwareNextSibling(void) const;
    Node* PseudoAwarePreviousSibling(void) const;
    Node* PseudoAwareFirstChild(void) const;
    Node* PseudoAwareLastChild(void) const;

    NodeListsNodeData* NodeLists(void);

    virtual void HandleLocalEvents(Event &event);
    virtual void WillCallDefaultEventHandler(const Event &event);
    virtual void DefaultEventHandler(Event &event);

    typedef ContainerNode* (*ParentGetter)(const Node &);
    Node* CommonAncestor(const Node &other, ParentGetter getParent) const;

    bool IsShadowIncludingInclusiveAncestorOf(const Node *node) const;
    bool IsDescendantOf(const Node *other) const;
    bool IsDocumentNode(void) const;
    bool IsDocumentTypeNode(void) const { return getNodeType() == kDocumentTypeNode; }
    bool IsTreeScope(void) const;
    bool HasRareData(void) const { return GetFlag(kHasRareDataFlag); }
    bool IsTextNode(void) const { return GetFlag(kIsTextFlag); }
    bool IsContainerNode(void) const { return GetFlag(kIsContainerFlag); }
    bool IsElementNode(void) const { return GetFlag(kIsElementFlag); }
    bool IsHTMLElement(void) const { return GetFlag(kIsHTMLFlag); }
#ifdef BLINKIT_CRAWLER_ONLY
    bool ForCrawler(void) const
    {
        ASSERT(GetFlag(kForCrawlerFlag));
        return true;
    }
#else
    bool ForCrawler(void) const { return GetFlag(kForCrawlerFlag); }
#endif
    bool IsDocumentFragment(void) const { return GetFlag(kIsDocumentFragmentFlag); }
    bool IsLink(void) const { return GetFlag(kIsLinkFlag); }
    bool IsUserActionElement(void) const { return GetFlag(kIsUserActionElementFlag); }
    bool isConnected(void) const { return GetFlag(kIsConnectedFlag); }
    bool IsInShadowTree(void) const { return GetFlag(kIsInShadowTreeFlag); }
    bool IsFinishedParsingChildren(void) const { return GetFlag(kIsFinishedParsingChildrenFlag); }
    bool IsInTreeScope(void) const { return GetFlag(static_cast<NodeFlags>(kIsConnectedFlag | kIsInShadowTreeFlag)); }
    bool ChildNeedsDistributionRecalc(void) const { return GetFlag(kChildNeedsDistributionRecalcFlag); }
    bool HasName(void) const
    {
        ASSERT(!IsTextNode());
        return GetFlag(kHasNameOrIsEditingTextFlag);
    }
    void SetHasName(bool f)
    {
        ASSERT(!IsTextNode());
        SetFlag(f, kHasNameOrIsEditingTextFlag);
    }
    bool HasEventTargetData(void) const { return GetFlag(kHasEventTargetDataFlag); }
    void SetHasEventTargetData(bool flag) { SetFlag(flag, kHasEventTargetDataFlag); }
    bool HasDuplicateAttribute(void) const { return GetFlag(kHasDuplicateAttributes); }
    void SetHasDuplicateAttributes(void) { SetFlag(kHasDuplicateAttributes); }

    bool IsActive(void) const { return IsUserActionElement() && IsUserActionElementActive(); }
    bool IsInDocumentTree(void) const { return isConnected() && !IsInShadowTree(); }
    bool IsShadowRoot(void) const { return IsDocumentFragment() && IsTreeScope(); }

    virtual PseudoId GetPseudoId(void) const { return kPseudoIdNone; }
    bool IsPseudoElement(void) const { return GetPseudoId() != kPseudoIdNone; }

    virtual bool IsCharacterDataNode(void) const { return false; }
    virtual bool IsAttributeNode(void) const { return false; }

    virtual bool ChildTypeAllowed(NodeType) const { return false; }

    enum InsertionNotificationRequest {
        kInsertionDone,
        kInsertionShouldCallDidNotifySubtreeInsertions
    };
    virtual InsertionNotificationRequest InsertedInto(ContainerNode &insertionPoint);
    virtual void DidNotifySubtreeInsertionsToDocument(void);
    virtual void RemovedFrom(ContainerNode &insertionPoint);

    void CheckSlotChangeAfterInserted(void)
    {
        ASSERT(false); // BKTODO:
    }

    bool MayContainLegacyNodeTreeWhereDistributionShouldBeSupported(void) const;
    // This is not what you might want to call in most cases.
    // You should call UpdateDistributionForFlatTreeTraversal, instead.
    // Only the implementation of v0 shadow trees uses this.
    void UpdateDistributionForLegacyDistributedNodes(void)
    {
        // The implementation is same to UpdateDistributionForFlatTreeTraversal.
        UpdateDistributionInternal();
    }

    void DispatchSubtreeModifiedEvent(void);

    void NotifyMutationObserversNodeWillDetach(void);

#ifndef BLINKIT_CRAWLER_ONLY
    struct AttachContext {
        STACK_ALLOCATED();

    public:
        // Keep track of previously attached in-flow box during attachment so that
        // we don't need to backtrack past display:none/contents and out of flow
        // objects when we need to do whitespace re-attachment.
        LayoutObject* previous_in_flow = nullptr;
        bool performing_reattach = false;
        bool clear_invalidation = false;
        // True if the previous_in_flow member is up-to-date, even if it is nullptr.
        bool use_previous_in_flow = false;

        AttachContext() {}
    };
    virtual void DetachLayoutTree(const AttachContext &context = AttachContext());
#endif
private:
    enum NodeFlags {
        kHasRareDataFlag = 1,

        // Node type flags. These never change once created.
        kIsTextFlag = 1 << 1,
        kIsContainerFlag = 1 << 2,
        kIsElementFlag = 1 << 3,
        kIsHTMLFlag = 1 << 4,
        kForCrawlerFlag = 1 << 5,
        kIsDocumentFragmentFlag = 1 << 6,
        kIsV0InsertionPointFlag = 1 << 7,

        // Changes based on if the element should be treated like a link,
        // ex. When setting the href attribute on an <a>.
        kIsLinkFlag = 1 << 8,

        // Changes based on :hover, :active and :focus state.
        kIsUserActionElementFlag = 1 << 9,

        // Tree state flags. These change when the element is added/removed
        // from a DOM tree.
        kIsConnectedFlag = 1 << 10,
        kIsInShadowTreeFlag = 1 << 11,

        // Set by the parser when the children are done parsing.
        kIsFinishedParsingChildrenFlag = 1 << 12,

        // Flags related to recalcStyle.
        kHasCustomStyleCallbacksFlag = 1 << 13,
        kChildNeedsStyleInvalidationFlag = 1 << 14,
        kNeedsStyleInvalidationFlag = 1 << 15,
        kChildNeedsDistributionRecalcFlag = 1 << 16,
        kChildNeedsStyleRecalcFlag = 1 << 17,
        kStyleChangeMask = 1 << kNodeStyleChangeShift | 1 << (kNodeStyleChangeShift + 1),

        kCustomElementStateMask = 0x3 << kNodeCustomElementShift,

        kHasNameOrIsEditingTextFlag = 1 << 22,
        kHasEventTargetDataFlag = 1 << 23,

        kV0CustomElementFlag = 1 << 24,
        kV0CustomElementUpgradedFlag = 1 << 25,

        kNeedsReattachLayoutTree = 1 << 26,
        kChildNeedsReattachLayoutTree = 1 << 27,

        kHasDuplicateAttributes = 1 << 28,

        // Temporary flag for some UseCounter items. crbug.com/859391.
        kInDOMNodeRemovedHandler = 1 << 29,

        kDefaultNodeFlags = kIsFinishedParsingChildrenFlag | kNeedsReattachStyleChange
    };

    // 3 bits remaining.

    bool GetFlag(NodeFlags mask) const { return 0 != (m_nodeFlags & mask); }
    void SetFlag(bool f, NodeFlags mask) {
        m_nodeFlags = (m_nodeFlags & ~mask) | (-(int32_t)f & mask);
    }
    void SetFlag(NodeFlags mask) { m_nodeFlags |= mask; }
    void ClearFlag(NodeFlags mask) { m_nodeFlags &= ~mask; }
protected:
    enum ConstructionType {
        kCreateOther = kIsFinishedParsingChildrenFlag,
        kCreateText = kDefaultNodeFlags | kIsTextFlag,
        kCreateContainer = kDefaultNodeFlags | kChildNeedsStyleRecalcFlag | kIsContainerFlag,
        kCreateElement = kCreateContainer | kIsElementFlag,
        kCreateShadowRoot = kCreateContainer | kIsDocumentFragmentFlag | kIsInShadowTreeFlag,
        kCreateDocumentFragment = kCreateContainer | kIsDocumentFragmentFlag,
        kCreateHTMLElement = kCreateElement | kIsHTMLFlag,
        kCreateDocument = kCreateContainer | kIsConnectedFlag,
        kCreateV0InsertionPoint = kCreateHTMLElement | kIsV0InsertionPointFlag,
        kCreateEditingText = kCreateText | kHasNameOrIsEditingTextFlag,
        kCreatePseudoElement = kDefaultNodeFlags | kIsContainerFlag | kIsElementFlag | kNeedsReattachLayoutTree,
        kCreateCrawlerDocument = kCreateDocument | kForCrawlerFlag,
        kCreateCrawlerElement = kCreateHTMLElement | kForCrawlerFlag,
    };

    Node(TreeScope *treeScope, ConstructionType type);

    void SetTreeScope(TreeScope *scope) { m_treeScope = scope; }
    void SetIsFinishedParsingChildren(bool value) { SetFlag(value, kIsFinishedParsingChildrenFlag); }

    NodeRareData* RareData(void) const
    {
        ASSERT(HasRareData());
        return static_cast<NodeRareData *>(m_data.m_rareData);
    }
    NodeRareData& EnsureRareData(void)
    {
        if (HasRareData())
            return *RareData();

        return CreateRareData();
    }

    // EventTarget overrides
    void AddedEventListener(const AtomicString& eventType, RegisteredEventListener &registeredListener) override;
    DispatchEventResult DispatchEventInternal(Event &event) override;
private:
    bool IsUserActionElementActive(void) const;

    void UpdateDistributionInternal(void);

    NodeRareData& CreateRareData(void);
    void ClearRareData(void);

    // EventTarget overrides
    ExecutionContext* GetExecutionContext(void) const final;
    Node* ToNode(void) final { return this; }
    EventTargetData* GetEventTargetData(void) override;
    EventTargetData& EnsureEventTargetData(void) override;

    uint32_t m_nodeFlags;
    Member<Node> m_parentOrShadowHostNode;
    Member<TreeScope> m_treeScope;
    Member<Node> m_previous;
    Member<Node> m_next;
    // When a node has rare data we move the layoutObject into the rare data.
    union DataUnion {
#ifdef BLINKIT_CRAWLER_ONLY
        NodeRareDataBase *m_rareData = nullptr;
#else
        DataUnion() : node_layout_data_(&NodeRenderingData::SharedEmptyData()) {}
        // LayoutObjects are fully owned by their DOM node. See LayoutObject's
        // LIFETIME documentation section.
        NodeRenderingData* node_layout_data_;
        NodeRareDataBase* rare_data_;
#endif
    } m_data;
 };

DEFINE_COMPARISON_OPERATORS_WITH_REFERENCES(Node)

#define DEFINE_NODE_TYPE_CASTS(thisType, predicate) \
    DEFINE_TYPE_CASTS(thisType, Node, node, node->predicate, node.predicate)

}  // namespace blink

#endif  // BLINKIT_BLINK_NODE_H
