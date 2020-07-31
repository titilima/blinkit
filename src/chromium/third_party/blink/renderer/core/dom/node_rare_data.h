// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: node_rare_data.h
// Description: Node Rare Data Classes
//      Author: Ziming Li
//     Created: 2020-03-04
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

#ifndef BLINKIT_BLINK_NODE_RARE_DATA_H
#define BLINKIT_BLINK_NODE_RARE_DATA_H

#include "base/macros.h"
#include "third_party/blink/renderer/platform/heap/handle.h"

namespace blink {

enum class DynamicRestyleFlags;
enum class ElementFlags;
#ifndef BLINKIT_CRAWLER_ONLY
class LayoutObject;
#endif
class NodeListsNodeData;

#ifndef BLINKIT_CRAWLER_ONLY
class NodeRenderingData
{
public:
    bool IsSharedEmptyData(void)
    {
        ASSERT(false); // BKTODO:
        return false;
    }
};
#endif

class NodeRareDataBase
{
public:
protected:
#ifndef BLINKIT_CRAWLER_ONLY
    explicit NodeRareDataBase(NodeRenderingData *nodeLayoutData) : m_nodeLayoutData(nodeLayoutData) {}
    ~NodeRareDataBase(void)
    {
        if (nullptr != m_nodeLayoutData && !m_nodeLayoutData->IsSharedEmptyData())
            delete m_nodeLayoutData;
    }

    NodeRenderingData *m_nodeLayoutData;
#endif
};

class NodeRareData : public GarbageCollectedFinalized<NodeRareData>, public NodeRareDataBase
{
public:
#ifdef BLINKIT_CRAWLER_ONLY
    static NodeRareData* Create(void) { return new NodeRareData; }
#else
    static NodeRareData* Create(NodeRenderingData *nodeLayoutData)
    {
        return new NodeRareData(nodeLayoutData);
    }
#endif
    ~NodeRareData(void);

    NodeListsNodeData* NodeLists(void) const { return m_nodeLists.get(); }
    // EnsureNodeLists() and a following NodeListsNodeData functions must be
    // wrapped with a ThreadState::GCForbiddenScope in order to avoid an
    // initialized node_lists_ is cleared by NodeRareData::TraceAfterDispatch().
    NodeListsNodeData& EnsureNodeLists(void)
    {
        if (!m_nodeLists)
            CreateNodeLists();
        return *m_nodeLists;
    }

    bool HasElementFlag(ElementFlags mask) const { return 0 != (m_elementFlags & static_cast<unsigned>(mask)); }
    void SetElementFlag(ElementFlags mask, bool value)
    {
        m_elementFlags = (m_elementFlags & ~static_cast<unsigned>(mask)) | (-(int32_t)value & static_cast<unsigned>(mask));
    }
    void ClearElementFlag(ElementFlags mask)
    {
        m_elementFlags &= ~static_cast<unsigned>(mask);
    }

    bool HasRestyleFlag(DynamicRestyleFlags mask) const { return 0 != (m_restyleFlags & static_cast<unsigned>(mask)); }
    void SetRestyleFlag(DynamicRestyleFlags mask)
    {
        m_restyleFlags |= static_cast<unsigned>(mask);
        ASSERT(HasRestyleFlags());
    }
    bool HasRestyleFlags(void) const { return 0 != m_restyleFlags; }
    void ClearRestyleFlags(void) { m_restyleFlags = 0; }

    enum {
        kNumberOfElementFlags = 6,
        kNumberOfDynamicRestyleFlags = 14
    };
protected:
#ifdef BLINKIT_CRAWLER_ONLY
    explicit NodeRareData(void);
#else
    explicit NodeRareData(NodeRenderingData *nodeLayoutData);
#endif
private:
    void CreateNodeLists(void);

    std::unique_ptr<NodeListsNodeData> m_nodeLists;

    unsigned m_elementFlags : kNumberOfElementFlags;
    unsigned m_restyleFlags : kNumberOfDynamicRestyleFlags;

    DISALLOW_COPY_AND_ASSIGN(NodeRareData);
};

} // namespace blink

#endif // BLINKIT_BLINK_NODE_RARE_DATA_H
