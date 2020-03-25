// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: node_lists_node_data.h
// Description: NodeListsNodeData Class
//      Author: Ziming Li
//     Created: 2020-03-03
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2008, 2010 Apple Inc. All rights reserved.
 * Copyright (C) 2008 David Smith <catfish.man@gmail.com>
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

#ifndef BLINKIT_BLINK_NODE_LISTS_NODE_DATA_H
#define BLINKIT_BLINK_NODE_LISTS_NODE_DATA_H

#pragma once

#include <unordered_map>
#include <utility>
#include "base/macros.h"
#include "base/memory/ptr_util.h"
#include "third_party/blink/renderer/core/css/css_selector.h"
#include "third_party/blink/renderer/core/dom/container_node.h"
#include "third_party/blink/renderer/core/dom/live_node_list_base.h"
#include "third_party/blink/renderer/core/dom/node_list.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/text/atomic_string.h"
#include "third_party/blink/renderer/platform/wtf/text/atomic_string_hash.h"
#include "third_party/blink/renderer/platform/wtf/text/string_hash.h"

namespace blink {

class ChildNodeList;

class NodeListsNodeData final : public GarbageCollected<NodeListsNodeData>
{
public:
    static std::unique_ptr<NodeListsNodeData> Create(void) { return base::WrapUnique(new NodeListsNodeData); }

    ChildNodeList* GetChildNodeList(ContainerNode &node)
    {
        ASSERT(!m_childNodeList || node == m_childNodeList->VirtualOwnerNode());
        ASSERT(!m_childNodeList); // BKTODO:
        return nullptr; // BKTODO: return ToChildNodeList(m_childNodeList.get());
    }

    void InvalidateCaches(const QualifiedName *attrName = nullptr);

    using NamedNodeListKey = std::pair<CollectionType, AtomicString>;
    struct NodeListAtomicCacheMapEntryHash {
        std::size_t operator()(const NamedNodeListKey &entry) const noexcept
        {
            return DefaultHash<AtomicString>::Hash::GetHash(
                entry.second == CSSSelector::UniversalSelectorAtom()
                ? g_star_atom
                : entry.second) +
                entry.first;
        }
    };

    typedef std::unordered_map<NamedNodeListKey, std::unique_ptr<LiveNodeListBase>, NodeListAtomicCacheMapEntryHash>
        NodeListAtomicNameCacheMap;

    template <typename T>
    T* AddCache(ContainerNode &node, CollectionType collectionType, const AtomicString &name)
    {
        NamedNodeListKey key = std::make_pair(collectionType, name);
        auto it = m_atomicNameCaches.find(key);
        if (std::end(m_atomicNameCaches) != it)
            return static_cast<T *>(it->second.get());

        T *list = T::Create(node, collectionType, name);
        m_atomicNameCaches[key] = base::WrapUnique(list);
        return list;
    }
private:
    NodeListsNodeData(void) = default;

    // Can be a ChildNodeList or an EmptyNodeList.
    std::unique_ptr<NodeList> m_childNodeList;
    NodeListAtomicNameCacheMap m_atomicNameCaches;
    DISALLOW_COPY_AND_ASSIGN(NodeListsNodeData);
};

template <typename Collection>
inline Collection* ContainerNode::EnsureCachedCollection(CollectionType type)
{
    return EnsureNodeLists().AddCache<Collection>(*this, type);
}

template <typename Collection>
inline Collection* ContainerNode::EnsureCachedCollection(CollectionType type, const AtomicString &name)
{
    return EnsureNodeLists().AddCache<Collection>(*this, type, name);
}

} // namespace blink

#endif // BLINKIT_BLINK_NODE_LISTS_NODE_DATA_H
