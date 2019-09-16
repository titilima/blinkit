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

#include "third_party/blink/renderer/core/dom/events/event_target.h"

namespace blink {

class TreeScope;

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

class Node : public EventTarget
{
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

    ~Node(void);
private:
    enum NodeFlags {
        kHasRareDataFlag = 1,

        // Node type flags. These never change once created.
        kIsTextFlag = 1 << 1,
        kIsContainerFlag = 1 << 2,
        kIsElementFlag = 1 << 3,
        kIsHTMLFlag = 1 << 4,
        kIsSVGFlag = 1 << 5,
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

        kDefaultNodeFlags =
        kIsFinishedParsingChildrenFlag | kNeedsReattachStyleChange
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
        kCreateSVGElement = kCreateElement | kIsSVGFlag,
        kCreateDocument = kCreateContainer | kIsConnectedFlag,
        kCreateV0InsertionPoint = kCreateHTMLElement | kIsV0InsertionPointFlag,
        kCreateEditingText = kCreateText | kHasNameOrIsEditingTextFlag,
        kCreatePseudoElement = kDefaultNodeFlags | kIsContainerFlag | kIsElementFlag | kNeedsReattachLayoutTree,
    };

    Node(TreeScope*, ConstructionType);
private:
    uint32_t m_nodeFlags;
};

}  // namespace blink

#endif  // BLINKIT_BLINK_NODE_H
