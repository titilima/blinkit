// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: live_node_list_registry.h
// Description: LiveNodeListRegistry Class
//      Author: Ziming Li
//     Created: 2019-11-18
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BLINKIT_BLINK_LIVE_NODE_LIST_REGISTRY_H
#define BLINKIT_BLINK_LIVE_NODE_LIST_REGISTRY_H

#pragma once

#include <utility>
#include <vector>
#include "base/macros.h"
#include "third_party/blink/renderer/platform/wtf/allocator.h"

namespace blink {

class LiveNodeListBase;
enum NodeListInvalidationType : int;

class LiveNodeListRegistry
{
    DISALLOW_NEW();
    using Entry = std::pair<const LiveNodeListBase *, unsigned>;
public:
    LiveNodeListRegistry(void) = default;

    void Add(const LiveNodeListBase *list, NodeListInvalidationType type);
    void Remove(const LiveNodeListBase *list, NodeListInvalidationType type);

    bool IsEmpty(void) const { return 0 == m_mask; }

    bool ContainsInvalidationType(NodeListInvalidationType type) const
    {
        return m_mask & MaskForInvalidationType(type);
    }
private:
    static inline unsigned MaskForInvalidationType(NodeListInvalidationType type)
    {
        return 1u << type;
    }

    std::vector<Entry> m_data;
    unsigned m_mask = 0;

    DISALLOW_COPY_AND_ASSIGN(LiveNodeListRegistry);
};

}  // namespace blink

#endif // BLINKIT_BLINK_LIVE_NODE_LIST_REGISTRY_H
