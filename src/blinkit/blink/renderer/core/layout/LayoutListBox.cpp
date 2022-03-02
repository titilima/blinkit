// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: LayoutListBox.cpp
// Description: LayoutListBox Class
//      Author: Ziming Li
//     Created: 2021-07-31
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006, 2007, 2008, 2011 Apple Inc. All rights reserved.
 *               2009 Torch Mobile Inc. All rights reserved. (http://www.torchmobile.com/)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "./LayoutListBox.h"

#include <cmath>
#include "blinkit/blink/renderer/core/HTMLNames.h"
#include "blinkit/blink/renderer/core/css/CSSFontSelector.h"
#include "blinkit/blink/renderer/core/css/resolver/StyleResolver.h"
// BKTODO: #include "core/dom/AXObjectCache.h"
#include "blinkit/blink/renderer/core/dom/element_traversal.h"
#include "blinkit/blink/renderer/core/dom/NodeComputedStyle.h"
#include "blinkit/blink/renderer/core/dom/StyleEngine.h"
#include "blinkit/blink/renderer/core/editing/FrameSelection.h"
#include "blinkit/blink/renderer/core/frame/FrameView.h"
#include "blinkit/blink/renderer/core/frame/LocalFrame.h"
#include "blinkit/blink/renderer/core/html/HTMLDivElement.h"
#include "blinkit/blink/renderer/core/html/HTMLOptGroupElement.h"
#include "blinkit/blink/renderer/core/html/HTMLOptionElement.h"
#include "blinkit/blink/renderer/core/html/HTMLSelectElement.h"
#include "blinkit/blink/renderer/core/input/EventHandler.h"
#include "blinkit/blink/renderer/core/layout/HitTestResult.h"
#include "blinkit/blink/renderer/core/layout/LayoutText.h"
#include "blinkit/blink/renderer/core/layout/LayoutTheme.h"
#include "blinkit/blink/renderer/core/layout/LayoutView.h"
#include "blinkit/blink/renderer/core/layout/TextRunConstructor.h"
#include "blinkit/blink/renderer/core/page/FocusController.h"
#include "blinkit/blink/renderer/core/page/Page.h"
#include "blinkit/blink/renderer/core/page/SpatialNavigation.h"
#include "blinkit/blink/renderer/core/paint/PaintLayer.h"
#include "blinkit/blink/renderer/platform/fonts/FontCache.h"
#include "blinkit/blink/renderer/platform/graphics/GraphicsContext.h"
#include "blinkit/blink/renderer/platform/text/BidiTextRun.h"

namespace blink {

using namespace HTMLNames;

// Default size when the multiple attribute is present but size attribute is absent.
const int defaultSize = 4;

const int defaultPaddingBottom = 1;

LayoutListBox::LayoutListBox(Element* element)
    : LayoutBlockFlow(element)
{
    ASSERT(element);
    ASSERT(element->isHTMLElement());
    ASSERT(isHTMLSelectElement(element));
}

LayoutListBox::~LayoutListBox()
{
}

inline HTMLSelectElement* LayoutListBox::selectElement() const
{
    return toHTMLSelectElement(node());
}

unsigned LayoutListBox::size() const
{
    unsigned specifiedSize = selectElement()->size();
    if (specifiedSize >= 1)
        return specifiedSize;

    return defaultSize;
}

LayoutUnit LayoutListBox::defaultItemHeight() const
{
    return style()->fontMetrics().height() + defaultPaddingBottom;
}

LayoutUnit LayoutListBox::itemHeight() const
{
    HTMLSelectElement* select = selectElement();
    if (!select)
        return 0;
    Element* baseItem = ElementTraversal::firstChild(*select);
    if (!baseItem)
        return defaultItemHeight();
    if (isHTMLOptGroupElement(baseItem))
        baseItem = &toHTMLOptGroupElement(baseItem)->optGroupLabelElement();
    else if (!isHTMLOptionElement(baseItem))
        return defaultItemHeight();
    LayoutObject* baseItemLayoutObject = baseItem->layoutObject();
    if (!baseItemLayoutObject || !baseItemLayoutObject->isBox())
        return defaultItemHeight();
    return toLayoutBox(baseItemLayoutObject)->size().height();
}

void LayoutListBox::computeLogicalHeight(LayoutUnit, LayoutUnit logicalTop, LogicalExtentComputedValues& computedValues) const
{
    LayoutUnit height = itemHeight() * size();
    // FIXME: The item height should have been added before updateLogicalHeight was called to avoid this hack.
    setIntrinsicContentLogicalHeight(height);

    height += borderAndPaddingHeight();

    LayoutBox::computeLogicalHeight(height, logicalTop, computedValues);
}

void LayoutListBox::stopAutoscroll()
{
    HTMLSelectElement* select = selectElement();
    if (select->isDisabledFormControl())
        return;
    select->handleMouseRelease();
}

void LayoutListBox::computeIntrinsicLogicalWidths(LayoutUnit& minLogicalWidth, LayoutUnit& maxLogicalWidth) const
{
    LayoutBlockFlow::computeIntrinsicLogicalWidths(minLogicalWidth, maxLogicalWidth);
    if (style()->width().hasPercent())
        minLogicalWidth = 0;
}

void LayoutListBox::scrollToRect(const LayoutRect& rect)
{
    if (hasOverflowClip()) {
        ASSERT(layer());
        ASSERT(layer()->scrollableArea());
        layer()->scrollableArea()->scrollIntoView(rect, ScrollAlignment::alignToEdgeIfNeeded, ScrollAlignment::alignToEdgeIfNeeded);
    }
}

} // namespace blink
