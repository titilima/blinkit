// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: static_node_list.h
// Description: StaticNodeTypeList Class
//      Author: Ziming Li
//     Created: 2020-03-30
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2007, 2008 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_DOM_STATIC_NODE_LIST_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_DOM_STATIC_NODE_LIST_H_

#include <vector>
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/dom/node_child_removal_tracker.h"
#include "third_party/blink/renderer/core/dom/node_list.h"

namespace blink {

class Node;

template <typename NodeType>
class StaticNodeTypeList final : public NodeList {
 public:
  static StaticNodeTypeList* Adopt(std::vector<NodeType *> &nodes, GCType gcType);

  static StaticNodeTypeList* CreateEmpty(GCType gcType) { return new StaticNodeTypeList(gcType); }

  ~StaticNodeTypeList() override;

  unsigned length() const override;
  NodeType* item(unsigned index) const override;

 private:
  StaticNodeTypeList(GCType gcType) : gcType_(gcType) {}

  GCType GetGCType(void) const final { return gcType_; }

  const GCType gcType_;
  std::vector<NodeType *> nodes_;
};

using StaticNodeList = StaticNodeTypeList<Node>;

template <typename NodeType>
StaticNodeTypeList<NodeType>* StaticNodeTypeList<NodeType>::Adopt(
    std::vector<NodeType *>& nodes, GCType gcType) {
  StaticNodeTypeList<NodeType>* node_list = new StaticNodeTypeList<NodeType>(gcType);
  swap(node_list->nodes_, nodes);
  return node_list;
}

template <typename NodeType>
StaticNodeTypeList<NodeType>::~StaticNodeTypeList() = default;

template <typename NodeType>
unsigned StaticNodeTypeList<NodeType>::length() const {
  return nodes_.size();
}

template <typename NodeType>
NodeType* StaticNodeTypeList<NodeType>::item(unsigned index) const {
  if (index < nodes_.size()) {
    auto* node = nodes_[index];
    if (node->GetDocument().InDOMNodeRemovedHandler() &&
        NodeChildRemovalTracker::IsBeingRemoved(node))
      node->GetDocument().CountDetachingNodeAccessInDOMNodeRemovedHandler();
    return node;
  }
  return nullptr;
}

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_DOM_STATIC_NODE_LIST_H_
