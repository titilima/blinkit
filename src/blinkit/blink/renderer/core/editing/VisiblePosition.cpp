/*
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
 * Portions Copyright (c) 2011 Motorola Mobility, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "core/editing/VisiblePosition.h"

#include "bindings/core/v8/ExceptionState.h"
#include "core/HTMLNames.h"
#include "core/dom/Document.h"
#include "core/dom/Range.h"
#include "core/dom/Text.h"
#include "core/editing/EditingUtilities.h"
#include "core/editing/TextAffinity.h"
#include "core/editing/VisibleUnits.h"
#include "core/html/HTMLElement.h"
#include "core/layout/LayoutBlock.h"
#include "core/layout/line/RootInlineBox.h"
#include "platform/geometry/FloatQuad.h"
#include "wtf/text/CString.h"

#ifndef NDEBUG
#include <stdio.h>
#endif

namespace blink {

using namespace HTMLNames;

template <typename Strategy>
VisiblePositionTemplate<Strategy>::VisiblePositionTemplate()
{
}

template <typename Strategy>
VisiblePositionTemplate<Strategy>::VisiblePositionTemplate(const PositionWithAffinityTemplate<Strategy>& positionWithAffinity)
    : m_positionWithAffinity(positionWithAffinity)
{
}

template<typename Strategy>
VisiblePositionTemplate<Strategy> VisiblePositionTemplate<Strategy>::create(const PositionWithAffinityTemplate<Strategy>& positionWithAffinity)
{
    const PositionTemplate<Strategy> deepPosition = canonicalPositionOf(positionWithAffinity.position());
    if (deepPosition.isNull())
        return VisiblePositionTemplate<Strategy>();
    const PositionWithAffinityTemplate<Strategy> downstreamPosition(deepPosition);
    if (positionWithAffinity.affinity() == TextAffinity::Downstream)
        return VisiblePositionTemplate<Strategy>(downstreamPosition);

    // When not at a line wrap, make sure to end up with
    // |TextAffinity::Downstream| affinity.
    const PositionWithAffinityTemplate<Strategy> upstreamPosition(deepPosition, TextAffinity::Upstream);
    if (inSameLine(downstreamPosition, upstreamPosition))
        return VisiblePositionTemplate<Strategy>(downstreamPosition);
    return VisiblePositionTemplate<Strategy>(upstreamPosition);
}

VisiblePosition createVisiblePosition(const Position& position, TextAffinity affinity)
{
    return createVisiblePosition(PositionWithAffinity(position, affinity));
}

VisiblePosition createVisiblePosition(const PositionWithAffinity& positionWithAffinity)
{
    return VisiblePosition::create(positionWithAffinity);
}

VisiblePositionInComposedTree createVisiblePosition(const PositionInComposedTree& position, TextAffinity affinity)
{
    return VisiblePositionInComposedTree::create(PositionInComposedTreeWithAffinity(position, affinity));
}

VisiblePositionInComposedTree createVisiblePosition(const PositionInComposedTreeWithAffinity& positionWithAffinity)
{
    return VisiblePositionInComposedTree::create(positionWithAffinity);
}

VisiblePosition createVisiblePositionInDOMTree(const Position& position, TextAffinity affinity)
{
    return createVisiblePosition(position, affinity);
}

VisiblePosition createVisiblePositionInDOMTree(const PositionInComposedTree& position, TextAffinity affinity)
{
    const VisiblePositionInComposedTree visiblePosition = createVisiblePosition(position);
    return createVisiblePosition(toPositionInDOMTree(visiblePosition.deepEquivalent()), affinity);
}

#ifndef NDEBUG

template<typename Strategy>
void VisiblePositionTemplate<Strategy>::debugPosition(const char* msg) const
{
    if (isNull()) {
        fprintf(stderr, "Position [%s]: null\n", msg);
        return;
    }
    deepEquivalent().debugPosition(msg);
}

template<typename Strategy>
void VisiblePositionTemplate<Strategy>::formatForDebugger(char* buffer, unsigned length) const
{
    deepEquivalent().formatForDebugger(buffer, length);
}

template<typename Strategy>
void VisiblePositionTemplate<Strategy>::showTreeForThis() const
{
    deepEquivalent().showTreeForThis();
}

#endif

template class CORE_TEMPLATE_EXPORT VisiblePositionTemplate<EditingStrategy>;
template class CORE_TEMPLATE_EXPORT VisiblePositionTemplate<EditingInComposedTreeStrategy>;

} // namespace blink

#ifndef NDEBUG

void showTree(const blink::VisiblePosition* vpos)
{
    if (vpos)
        vpos->showTreeForThis();
    else
        fprintf(stderr, "Cannot showTree for (nil) VisiblePosition.\n");
}

void showTree(const blink::VisiblePosition& vpos)
{
    vpos.showTreeForThis();
}

#endif
