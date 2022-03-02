#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: StyledMarkupSerializer.h
// Description: StyledMarkupSerializer Class
//      Author: Ziming Li
//     Created: 2022-03-02
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
 * Copyright (C) 2008, 2009, 2010, 2011 Google Inc. All rights reserved.
 * Copyright (C) 2011 Igalia S.L.
 * Copyright (C) 2011 Motorola Mobility. All rights reserved.
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

#ifndef StyledMarkupSerializer_h
#define StyledMarkupSerializer_h

#include "blinkit/blink/renderer/core/dom/node_traversal.h"
#include "blinkit/blink/renderer/core/editing/EditingStrategy.h"
#include "blinkit/blink/renderer/core/editing/EditingStyle.h"
#include "blinkit/blink/renderer/core/editing/Position.h"
#include "blinkit/blink/renderer/core/editing/serializers/StyledMarkupAccumulator.h"
#include "blinkit/blink/renderer/wtf/Forward.h"

namespace blink {

template<typename Strategy>
class StyledMarkupSerializer final
{
    STACK_ALLOCATED();
public:
    StyledMarkupSerializer(EAbsoluteURLs, EAnnotateForInterchange, const PositionTemplate<Strategy>& start, const PositionTemplate<Strategy>& end, Node* highestNodeToBeSerialized, ConvertBlocksToInlines);

    String createMarkup();

private:
    bool shouldAnnotate() const { return m_shouldAnnotate == AnnotateForInterchange; }

    const PositionTemplate<Strategy> m_start;
    const PositionTemplate<Strategy> m_end;
    const EAbsoluteURLs m_shouldResolveURLs;
    const EAnnotateForInterchange m_shouldAnnotate;
    const RefPtrWillBeMember<Node> m_highestNodeToBeSerialized;
    const ConvertBlocksToInlines m_convertBlocksToInlines;
    RawPtrWillBeMember<Node> m_lastClosed;
    RefPtrWillBeMember<EditingStyle> m_wrappingStyle;
};

extern template class StyledMarkupSerializer<EditingStrategy>;
extern template class StyledMarkupSerializer<EditingInComposedTreeStrategy>;

} // namespace blink

#endif // StyledMarkupSerializer_h
