// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ScrollState.cpp
// Description: ScrollState Class
//      Author: Ziming Li
//     Created: 2021-12-04
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "./ScrollState.h"

#include "blinkit/blink/renderer/core/dom/dom_node_ids.h"
#include "blinkit/blink/renderer/core/dom/Element.h"
#include "blinkit/blink/renderer/core/dom/ExceptionCode.h"

namespace blink {

namespace {
Element* elementForId(int elementId)
{
    Node* node = DOMNodeIds::nodeForId(elementId);
    ASSERT(node);
    if (!node)
        return nullptr;
    ASSERT(node->isElementNode());
    if (!node->isElementNode())
        return nullptr;
    return static_cast<Element*>(node);
}
} // namespace

PassRefPtrWillBeRawPtr<ScrollState> ScrollState::create(
    double deltaX, double deltaY, double deltaGranularity, double velocityX,
    double velocityY, bool inInertialPhase,
    bool isBeginning, bool isEnding,
    bool fromUserInput, bool shouldPropagate,
    bool deltaConsumedForScrollSequence)
{
    OwnPtr<WebScrollStateData> data(adoptPtr(new WebScrollStateData(deltaX, deltaY, deltaGranularity, velocityX, velocityY,
        inInertialPhase, isBeginning, isEnding, fromUserInput, shouldPropagate,
        deltaConsumedForScrollSequence)));
    return adoptRefWillBeNoop(new ScrollState(data.release()));
}

PassRefPtrWillBeRawPtr<ScrollState> ScrollState::create(PassOwnPtr<WebScrollStateData> data)
{
    ScrollState* scrollState = new ScrollState(data);
    return adoptRefWillBeNoop(scrollState);
}

ScrollState::ScrollState(PassOwnPtr<WebScrollStateData> data)
    : m_data(data)
{
}

void ScrollState::consumeDelta(double x, double y, ExceptionState& exceptionState)
{
    if ((m_data->deltaX > 0 && 0 > x) || (m_data->deltaX < 0 && 0 < x) || (m_data->deltaY > 0 && 0 > y) || (m_data->deltaY < 0 && 0 < y)) {
        exceptionState.throwDOMException(InvalidModificationError, "Can't increase delta using consumeDelta");
        return;
    }
    if (fabs(x) > fabs(m_data->deltaX) || fabs(y) > fabs(m_data->deltaY)) {
        exceptionState.throwDOMException(InvalidModificationError, "Can't change direction of delta using consumeDelta");
        return;
    }
    consumeDeltaNative(x, y);
}

void ScrollState::distributeToScrollChainDescendant()
{
    if (!m_scrollChain.empty()) {
        int descendantId = m_scrollChain.front();
        m_scrollChain.pop_front();
        elementForId(descendantId)->callDistributeScroll(*this);
    }
}

void ScrollState::consumeDeltaNative(double x, double y)
{
    m_data->deltaX -= x;
    m_data->deltaY -= y;

    if (x || y)
        m_data->deltaConsumedForScrollSequence = true;
}

Element* ScrollState::currentNativeScrollingElement() const
{
    if (m_data->currentNativeScrollingElement == 0)
        return nullptr;
    return elementForId(m_data->currentNativeScrollingElement);
}

void ScrollState::setCurrentNativeScrollingElement(Element* element)
{
    m_data->currentNativeScrollingElement = DOMNodeIds::idForNode(element);
}

int ScrollState::currentNativeScrollingElementId() const
{
    return m_data->currentNativeScrollingElement;
}

void ScrollState::setCurrentNativeScrollingElementById(int elementId)
{
    m_data->currentNativeScrollingElement = elementId;
}

} // namespace blink
