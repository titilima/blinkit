// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: live_node_list_registry.cpp
// Description: LiveNodeListRegistry Class
//      Author: Ziming Li
//     Created: 2019-11-18
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "live_node_list_registry.h"

#include "third_party/blink/renderer/core/dom/document.h"

namespace blink {

static_assert(kNumNodeListInvalidationTypes <= sizeof(unsigned) * 8, "NodeListInvalidationType must fit in LiveNodeListRegistry bits");

void LiveNodeListRegistry::Add(const LiveNodeListBase *list, NodeListInvalidationType type)
{
    Entry entry = { list, MaskForInvalidationType(type) };
    ASSERT(std::find(m_data.begin(), m_data.end(), entry) == m_data.end());
    m_data.push_back(entry);
    m_mask |= entry.second;
}

void LiveNodeListRegistry::Remove(const LiveNodeListBase *list, NodeListInvalidationType type)
{
    ASSERT(false); // BKTODO:
}

}  // namespace blink
