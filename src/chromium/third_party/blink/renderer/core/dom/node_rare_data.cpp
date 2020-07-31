// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: node_rare_data.cpp
// Description: Node Rare Data Classes
//      Author: Ziming Li
//     Created: 2020-03-04
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2012 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "node_rare_data.h"

#include "third_party/blink/renderer/core/dom/node_lists_node_data.h"

namespace blink {

#ifdef BLINKIT_CRAWLER_ONLY
NodeRareData::NodeRareData(void) : m_elementFlags(0), m_restyleFlags(0) {}
#else
NodeRareData::NodeRareData(NodeRenderingData *nodeLayoutData)
    : NodeRareDataBase(nodeLayoutData)
    , m_elementFlags(0)
    , m_restyleFlags(0)
{
    ASSERT(nullptr != nodeLayoutData);
}
#endif

NodeRareData::~NodeRareData(void) = default;

void NodeRareData::CreateNodeLists(void)
{
    m_nodeLists = NodeListsNodeData::Create();
}

} // namespace blink
