// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: element_rare_data.cpp
// Description: ElementRareData Class
//      Author: Ziming Li
//     Created: 2019-11-16
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
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

#include "element_rare_data.h"

#include "third_party/blink/renderer/core/dom/attr.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "third_party/blink/renderer/core/style/computed_style.h"
#endif

namespace blink {

struct SameSizeAsElementRareData : NodeRareData
{
#ifdef BLINKIT_CRAWLER_ONLY
    void* pointersOrStrings[4];
#else
    void* pointersOrStrings[8];
#endif
};

#ifdef BLINKIT_CRAWLER_ONLY
ElementRareData::ElementRareData(void)
#else
ElementRareData::ElementRareData(NodeRenderingData *nodeLayoutData) : NodeRareData(nullptr)
#endif
{
    m_isElementRareData = true;
    ASSERT(false); // BKTODO:
}

ElementRareData::~ElementRareData(void)
{
#ifndef BLINKIT_CRAWLER_ONLY
    ASSERT(!m_pseudoElementData);
#endif
}

AttrNodeList& ElementRareData::EnsureAttrNodeList(void)
{
    if (!m_attrNodeList)
        m_attrNodeList = std::make_unique<AttrNodeList>();
    return *m_attrNodeList;
}

void ElementRareData::TraceAfterDispatch(Visitor *visitor)
{
#ifndef BLINKIT_CRAWLER_ONLY
    m_shadowRoot->Trace(visitor);
    visitor->Trace(m_pseudoElementData);
#endif
    visitor->Trace(m_attributeMap);
    if (m_attrNodeList)
        visitor->Trace(*m_attrNodeList);
    NodeRareData::TraceAfterDispatch(visitor);
}

#ifndef BLINKIT_CRAWLER_ONLY
void ElementRareData::ClearComputedStyle(void)
{
    m_computedStyle = nullptr;
}

void ElementRareData::ClearPseudoElements(void)
{
    if (m_pseudoElementData)
    {
        m_pseudoElementData->ClearPseudoElements();
        m_pseudoElementData.Clear();
    }
}

PseudoElement* ElementRareData::GetPseudoElement(PseudoId pseudoId) const
{
    if (!m_pseudoElementData)
        return nullptr;
    return m_pseudoElementData->GetPseudoElement(pseudoId);
}

void ElementRareData::SetComputedStyle(scoped_refptr<ComputedStyle> computedStyle)
{
    m_computedStyle = std::move(computedStyle);
}

void ElementRareData::SetPseudoElement(PseudoId pseudoId, PseudoElement *element)
{
    if (!m_pseudoElementData)
    {
        if (nullptr == element)
            return;
        m_pseudoElementData = PseudoElementData::Create();
    }
    m_pseudoElementData->SetPseudoElement(pseudoId, element);
}
#endif // BLINKIT_CRAWLER_ONLY

static_assert(sizeof(ElementRareData) == sizeof(SameSizeAsElementRareData), "ElementRareData should stay small");

}  // namespace blink
