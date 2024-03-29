// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CharacterData.cpp
// Description: CharacterData Class
//      Author: Ziming Li
//     Created: 2021-07-04
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 * Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2013 Apple Inc. All rights reserved.
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
 */

#include "./CharacterData.h"

#include "blinkit/blink/renderer/bindings/core/exception_state.h"
#include "blinkit/blink/renderer/core/dom/document.h"
#include "blinkit/blink/renderer/core/dom/ExceptionCode.h"
#include "blinkit/blink/renderer/core/dom/MutationObserverInterestGroup.h"
#include "blinkit/blink/renderer/core/dom/MutationRecord.h"
#include "blinkit/blink/renderer/core/dom/ProcessingInstruction.h"
#include "blinkit/blink/renderer/core/dom/Text.h"
#include "blinkit/blink/renderer/core/editing/FrameSelection.h"
#if 0 // BKTODO: MutationEvent stuff is not recommended, remove it later. See also: https://developer.mozilla.org/en-US/docs/Web/API/MutationEvent
#include "core/events/MutationEvent.h"
#endif
#include "blinkit/blink/renderer/core/inspector/InspectorInstrumentation.h"
#include "blinkit/blink/renderer/wtf/CheckedArithmetic.h"

namespace blink {

void CharacterData::atomize()
{
    m_data = AtomicString(m_data);
}

void CharacterData::setData(const String& data)
{
    const String& nonNullData = !data.isNull() ? data : emptyString();
    if (m_data == nonNullData)
        return;

    unsigned oldLength = length();

    setDataAndUpdate(nonNullData, 0, oldLength, nonNullData.length(), UpdateFromNonParser);
#ifdef BLINKIT_UI_ENABLED
    if (isUINode())
        document().didRemoveText(this, 0, oldLength);
#endif
}

String CharacterData::substringData(unsigned offset, unsigned count, ExceptionState& exceptionState)
{
    if (offset > length()) {
        exceptionState.throwDOMException(IndexSizeError, "The offset " + String::number(offset) + " is greater than the node's length (" + String::number(length()) + ").");
        return String();
    }

    return m_data.substring(offset, count);
}

void CharacterData::parserAppendData(const String& data)
{
    String newStr = m_data + data;

    setDataAndUpdate(newStr, m_data.length(), 0, data.length(), UpdateFromParser);
}

void CharacterData::appendData(const String& data)
{
    String newStr = m_data + data;

    setDataAndUpdate(newStr, m_data.length(), 0, data.length(), UpdateFromNonParser);

    // FIXME: Should we call textInserted here?
}

void CharacterData::insertData(unsigned offset, const String& data, ExceptionState& exceptionState, RecalcStyleBehavior recalcStyleBehavior)
{
    if (offset > length()) {
        exceptionState.throwDOMException(IndexSizeError, "The offset " + String::number(offset) + " is greater than the node's length (" + String::number(length()) + ").");
        return;
    }

    String newStr = m_data;
    newStr.insert(data, offset);

    setDataAndUpdate(newStr, offset, 0, data.length(), UpdateFromNonParser, recalcStyleBehavior);

    document().didInsertText(this, offset, data.length());
}

static bool validateOffsetCount(unsigned offset, unsigned count, unsigned length, unsigned& realCount, ExceptionState& exceptionState)
{
    if (offset > length) {
        exceptionState.throwDOMException(IndexSizeError, "The offset " + String::number(offset) + " is greater than the node's length (" + String::number(length) + ").");
        return false;
    }

    Checked<unsigned, RecordOverflow> offsetCount = offset;
    offsetCount += count;

    if (offsetCount.hasOverflowed() || offset + count > length)
        realCount = length - offset;
    else
        realCount = count;

    return true;
}

void CharacterData::deleteData(unsigned offset, unsigned count, ExceptionState& exceptionState, RecalcStyleBehavior recalcStyleBehavior)
{
    unsigned realCount = 0;
    if (!validateOffsetCount(offset, count, length(), realCount, exceptionState))
        return;

    String newStr = m_data;
    newStr.remove(offset, realCount);

    setDataAndUpdate(newStr, offset, realCount, 0, UpdateFromNonParser, recalcStyleBehavior);

    document().didRemoveText(this, offset, realCount);
}

void CharacterData::replaceData(unsigned offset, unsigned count, const String& data, ExceptionState& exceptionState)
{
    unsigned realCount = 0;
    if (!validateOffsetCount(offset, count, length(), realCount, exceptionState))
        return;

    String newStr = m_data;
    newStr.remove(offset, realCount);
    newStr.insert(data, offset);

    setDataAndUpdate(newStr, offset, realCount, data.length(), UpdateFromNonParser);

    if (isUINode())
    {
        // update the markers for spell checking and grammar checking
        document().didRemoveText(this, offset, realCount);
        document().didInsertText(this, offset, data.length());
    }
}

String CharacterData::nodeValue() const
{
    return m_data;
}

bool CharacterData::containsOnlyWhitespace() const
{
    return m_data.containsOnlyWhitespace();
}

void CharacterData::setNodeValue(const String& nodeValue)
{
    setData(nodeValue);
}

void CharacterData::setDataAndUpdate(const String& newData, unsigned offsetOfReplacedData, unsigned oldLength, unsigned newLength, UpdateSource source, RecalcStyleBehavior recalcStyleBehavior)
{
    String oldData = m_data;
    m_data = newData;

    ASSERT(!layoutObject() || isTextNode());
    if (isTextNode())
        toText(this)->updateTextLayoutObject(offsetOfReplacedData, oldLength, recalcStyleBehavior);

    if (source != UpdateFromParser) {
        if (nodeType() == PROCESSING_INSTRUCTION_NODE)
            toProcessingInstruction(this)->didAttributeChanged();

        if (document().frame())
            document().frame()->selection().didUpdateCharacterData(this, offsetOfReplacedData, oldLength, newLength);
    }

    document().incDOMTreeVersion();
    didModifyData(oldData, source);
}

void CharacterData::didModifyData(const String& oldData, UpdateSource source)
{
    if (OwnPtrWillBeRawPtr<MutationObserverInterestGroup> mutationRecipients = MutationObserverInterestGroup::createForCharacterDataMutation(*this))
        mutationRecipients->enqueueMutationRecord(MutationRecord::createCharacterData(this, oldData));

    if (parentNode()) {
        ContainerNode::ChildrenChange change = {ContainerNode::TextChanged, previousSibling(), nextSibling(), ContainerNode::ChildrenChangeSourceAPI};
        parentNode()->childrenChanged(change);
    }

    // Skip DOM mutation events if the modification is from parser.
    // Note that mutation observer events will still fire.
    // Spec: https://html.spec.whatwg.org/multipage/syntax.html#insert-a-character
    if (source != UpdateFromParser && !isInShadowTree()) {
        if (document().hasListenerType(Document::DOMCHARACTERDATAMODIFIED_LISTENER))
            ASSERT(false); // BKTODO: dispatchScopedEvent(MutationEvent::create(EventTypeNames::DOMCharacterDataModified, true, nullptr, oldData, m_data));
        dispatchSubtreeModifiedEvent();
    }
    InspectorInstrumentation::characterDataModified(this);
}

int CharacterData::maxCharacterOffset() const
{
    return static_cast<int>(length());
}

} // namespace blink
