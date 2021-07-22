// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: SlotAssignment.cpp
// Description: SlotAssignment Class
//      Author: Ziming Li
//     Created: 2021-07-22
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/dom/shadow/SlotAssignment.h"

#include "core/HTMLNames.h"
#include "core/dom/ElementTraversal.h"
#include "core/dom/NodeTraversal.h"
#include "core/dom/shadow/InsertionPoint.h"
#include "core/dom/shadow/ShadowRoot.h"
#include "core/html/HTMLSlotElement.h"

namespace blink {

HTMLSlotElement* SlotAssignment::assignedSlotFor(const Node& node) const
{
    auto it = m_assignment.find(const_cast<Node *>(&node));
    return m_assignment.end() != it ? it->second : nullptr;
}

static void detachNotAssignedNode(Node& node)
{
    if (node.layoutObject())
        node.lazyReattachIfAttached();
}

void SlotAssignment::resolveAssignment(const ShadowRoot& shadowRoot)
{
    m_assignment.clear();

    using Name2Slot = std::unordered_map<AtomicString, Member<HTMLSlotElement>>;
    Name2Slot name2slot;
    HTMLSlotElement* defaultSlot = nullptr;
    WillBeHeapVector<RefPtrWillBeMember<HTMLSlotElement>> slots;

    // TODO(hayato): Cache slots elements so that we do not have to travese the shadow tree. See ShadowRoot::descendantInsertionPoints()
    for (HTMLSlotElement& slot : Traversal<HTMLSlotElement>::descendantsOf(shadowRoot)) {
        slot.clearDistribution();

        slots.append(&slot);

        AtomicString name = slot.fastGetAttribute(HTMLNames::nameAttr);
        if (name.isNull() || name.isEmpty()) {
            if (!defaultSlot)
                defaultSlot = &slot;
        } else {
            name2slot.emplace(name, &slot);
        }
    }

    for (Node& child : NodeTraversal::childrenOf(*shadowRoot.host())) {
        if (child.isElementNode()) {
            if (isActiveInsertionPoint(child)) {
                // TODO(hayato): Support re-distribution across v0 and v1 shadow trees
                detachNotAssignedNode(child);
                continue;
            }
            AtomicString slotName = toElement(child).fastGetAttribute(HTMLNames::slotAttr);
            if (slotName.isNull() || slotName.isEmpty()) {
                if (defaultSlot)
                    assign(child, *defaultSlot);
                else
                    detachNotAssignedNode(child);
            } else {
                auto it = name2slot.find(slotName);
                if (name2slot.end() != it)
                    assign(child, *(it->second));
                else
                    detachNotAssignedNode(child);
            }
        } else if (defaultSlot) {
            assign(child, *defaultSlot);
        } else {
            detachNotAssignedNode(child);
        }
    }

    // Update each slot's distribution in reverse tree order so that a child slot is visited before its parent slot.
    for (auto slot = slots.rbegin(); slot != slots.rend(); ++slot)
        (*slot)->updateDistributedNodesWithFallback();
}

void SlotAssignment::assign(Node& hostChild, HTMLSlotElement& slot)
{
    m_assignment.emplace(&hostChild, &slot);
    slot.appendAssignedNode(hostChild);
    if (isHTMLSlotElement(hostChild))
        slot.appendDistributedNodesFrom(toHTMLSlotElement(hostChild));
    else
        slot.appendDistributedNode(hostChild);
}

DEFINE_TRACE(SlotAssignment)
{
#if ENABLE(OILPAN)
    visitor->trace(m_assignment);
#endif
}

} // namespace blink
