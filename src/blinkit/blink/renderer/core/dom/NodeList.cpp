// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: NodeList.cpp
// Description: NodeList Class
//      Author: Ziming Li
//     Created: 2021-07-21
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/dom/NodeList.h"

namespace blink {

#if 0 // BKTODO:
namespace {

    class NodeListIterationSource final : public ValueIterable<Node*>::IterationSource {
    public:
        explicit NodeListIterationSource(NodeList& nodeList)
            : m_nodeList(nodeList)
        {
        }

        bool next(ScriptState* scriptState, Node*& value, ExceptionState& exceptionState) override
        {
            value = m_nodeList->item(m_index);
            return !!value;
        }

        DEFINE_INLINE_VIRTUAL_TRACE()
        {
            visitor->trace(m_nodeList);
            ValueIterable<Node*>::IterationSource::trace(visitor);
        }

    private:
        const RefPtrWillBeMember<NodeList> m_nodeList;
    };

} // namespace

ValueIterable<Node*>::IterationSource* NodeList::startIteration(ScriptState*, ExceptionState&)
{
    return new NodeListIterationSource(*this);
}
#endif

} // namespace blink
