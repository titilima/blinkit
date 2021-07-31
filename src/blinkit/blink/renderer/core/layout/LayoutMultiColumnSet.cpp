/*
 * Copyright (C) 2012 Apple Inc.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
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

#include "core/layout/LayoutMultiColumnSet.h"

#include "core/editing/PositionWithAffinity.h"
#include "core/layout/LayoutMultiColumnFlowThread.h"
#include "core/layout/MultiColumnFragmentainerGroup.h"
#include "core/paint/MultiColumnSetPainter.h"
#include "platform/RuntimeEnabledFeatures.h"

namespace blink {

LayoutMultiColumnSet::LayoutMultiColumnSet(LayoutFlowThread* flowThread)
    : LayoutBlockFlow(nullptr)
    , m_fragmentainerGroups(*this)
    , m_flowThread(flowThread)
    , m_initialHeightCalculated(false)
{
}

LayoutMultiColumnSet* LayoutMultiColumnSet::createAnonymous(LayoutFlowThread& flowThread, const ComputedStyle& parentStyle)
{
    Document& document = flowThread.document();
    LayoutMultiColumnSet* layoutObject = new LayoutMultiColumnSet(&flowThread);
    layoutObject->setDocumentForAnonymous(&document);
    layoutObject->setStyle(ComputedStyle::createAnonymousStyleWithDisplay(parentStyle, BLOCK));
    return layoutObject;
}

unsigned LayoutMultiColumnSet::fragmentainerGroupIndexAtFlowThreadOffset(LayoutUnit flowThreadOffset) const
{
    ASSERT(m_fragmentainerGroups.size() > 0);
    if (flowThreadOffset <= 0)
        return 0;
    // TODO(mstensho): Introduce an interval tree or similar to speed up this.
    for (unsigned index = 0; index < m_fragmentainerGroups.size(); index++) {
        const auto& row = m_fragmentainerGroups[index];
        if (row.logicalTopInFlowThread() <= flowThreadOffset && row.logicalBottomInFlowThread() > flowThreadOffset)
            return index;
    }
    return m_fragmentainerGroups.size() - 1;
}

const MultiColumnFragmentainerGroup& LayoutMultiColumnSet::fragmentainerGroupAtVisualPoint(const LayoutPoint&) const
{
    // FIXME: implement this, once we have support for multiple rows.
    return m_fragmentainerGroups.first();
}

LayoutUnit LayoutMultiColumnSet::pageLogicalHeightForOffset(LayoutUnit offsetInFlowThread) const
{
    return fragmentainerGroupAtFlowThreadOffset(offsetInFlowThread).logicalHeight();
}

LayoutUnit LayoutMultiColumnSet::pageRemainingLogicalHeightForOffset(LayoutUnit offsetInFlowThread, PageBoundaryRule pageBoundaryRule) const
{
    const MultiColumnFragmentainerGroup& row = fragmentainerGroupAtFlowThreadOffset(offsetInFlowThread);
    LayoutUnit pageLogicalHeight = row.logicalHeight();
    ASSERT(pageLogicalHeight); // It's not allowed to call this method if the height is unknown.
    LayoutUnit pageLogicalBottom = row.columnLogicalTopForOffset(offsetInFlowThread) + pageLogicalHeight;
    LayoutUnit remainingLogicalHeight = pageLogicalBottom - offsetInFlowThread;

    if (pageBoundaryRule == AssociateWithFormerPage) {
        // An offset exactly at a column boundary will act as being part of the former column in
        // question (i.e. no remaining space), rather than being part of the latter (i.e. one whole
        // column length of remaining space).
        remainingLogicalHeight = intMod(remainingLogicalHeight, pageLogicalHeight);
    }
    return remainingLogicalHeight;
}

bool LayoutMultiColumnSet::isPageLogicalHeightKnown() const
{
    return firstFragmentainerGroup().logicalHeight();
}

LayoutUnit LayoutMultiColumnSet::nextLogicalTopForUnbreakableContent(LayoutUnit flowThreadOffset, LayoutUnit contentLogicalHeight) const
{
    ASSERT(pageLogicalTopForOffset(flowThreadOffset) == flowThreadOffset);
    FragmentationContext* enclosingFragmentationContext = multiColumnFlowThread()->enclosingFragmentationContext();
    if (!enclosingFragmentationContext) {
        // If there's no enclosing fragmentation context, there'll ever be only one row, and all
        // columns there will have the same height.
        return flowThreadOffset;
    }

    // Assert the problematic situation. If we have no problem with the column height, why are we
    // even here?
    ASSERT(pageLogicalHeightForOffset(flowThreadOffset) < contentLogicalHeight);

    // There's a likelihood for subsequent rows to be taller than the first one.
    // TODO(mstensho): if we're doubly nested (e.g. multicol in multicol in multicol), we need to
    // look beyond the first row here.
    const MultiColumnFragmentainerGroup& firstRow = firstFragmentainerGroup();
    LayoutUnit firstRowLogicalBottomInFlowThread = firstRow.logicalTopInFlowThread() + firstRow.logicalHeight() * usedColumnCount();
    if (flowThreadOffset >= firstRowLogicalBottomInFlowThread)
        return flowThreadOffset; // We're not in the first row. Give up.
    LayoutUnit newLogicalHeight = enclosingFragmentationContext->fragmentainerLogicalHeightAt(firstRowLogicalBottomInFlowThread);
    if (contentLogicalHeight > newLogicalHeight) {
        // The next outer column or page doesn't have enough space either. Give up and stay where
        // we are.
        return flowThreadOffset;
    }
    return firstRowLogicalBottomInFlowThread;
}

LayoutMultiColumnSet* LayoutMultiColumnSet::nextSiblingMultiColumnSet() const
{
    for (LayoutObject* sibling = nextSibling(); sibling; sibling = sibling->nextSibling()) {
        if (sibling->isLayoutMultiColumnSet())
            return toLayoutMultiColumnSet(sibling);
    }
    return nullptr;
}

LayoutMultiColumnSet* LayoutMultiColumnSet::previousSiblingMultiColumnSet() const
{
    for (LayoutObject* sibling = previousSibling(); sibling; sibling = sibling->previousSibling()) {
        if (sibling->isLayoutMultiColumnSet())
            return toLayoutMultiColumnSet(sibling);
    }
    return nullptr;
}

bool LayoutMultiColumnSet::hasFragmentainerGroupForColumnAt(LayoutUnit offsetInFlowThread) const
{
    const MultiColumnFragmentainerGroup& lastRow = lastFragmentainerGroup();
    if (lastRow.logicalTopInFlowThread() > offsetInFlowThread)
        return true;
    return offsetInFlowThread - lastRow.logicalTopInFlowThread() < lastRow.logicalHeight() * usedColumnCount();
}

MultiColumnFragmentainerGroup& LayoutMultiColumnSet::appendNewFragmentainerGroup()
{
    MultiColumnFragmentainerGroup newGroup(*this);
    { // Extra scope here for previousGroup; it's potentially invalid once we modify the m_fragmentainerGroups Vector.
        MultiColumnFragmentainerGroup& previousGroup = m_fragmentainerGroups.last();

        // This is the flow thread block offset where |previousGroup| ends and |newGroup| takes over.
        LayoutUnit blockOffsetInFlowThread = previousGroup.logicalTopInFlowThread() + previousGroup.logicalHeight() * usedColumnCount();
        previousGroup.setLogicalBottomInFlowThread(blockOffsetInFlowThread);
        newGroup.setLogicalTopInFlowThread(blockOffsetInFlowThread);

        newGroup.setLogicalTop(previousGroup.logicalTop() + previousGroup.logicalHeight());
        newGroup.resetColumnHeight();
    }
    m_fragmentainerGroups.append(newGroup);
    return m_fragmentainerGroups.last();
}

LayoutUnit LayoutMultiColumnSet::logicalTopInFlowThread() const
{
    return firstFragmentainerGroup().logicalTopInFlowThread();
}

LayoutUnit LayoutMultiColumnSet::logicalBottomInFlowThread() const
{
    return lastFragmentainerGroup().logicalBottomInFlowThread();
}

LayoutRect LayoutMultiColumnSet::flowThreadPortionOverflowRect() const
{
    return overflowRectForFlowThreadPortion(flowThreadPortionRect(), !previousSiblingMultiColumnSet(), !nextSiblingMultiColumnSet());
}

LayoutRect LayoutMultiColumnSet::overflowRectForFlowThreadPortion(const LayoutRect& flowThreadPortionRect, bool isFirstPortion, bool isLastPortion) const
{
    if (hasOverflowClip())
        return flowThreadPortionRect;

    LayoutRect flowThreadOverflow = m_flowThread->visualOverflowRect();

    // Only clip along the flow thread axis.
    LayoutRect clipRect;
    if (m_flowThread->isHorizontalWritingMode()) {
        LayoutUnit minY = isFirstPortion ? flowThreadOverflow.y() : flowThreadPortionRect.y();
        LayoutUnit maxY = isLastPortion ? std::max(flowThreadPortionRect.maxY(), flowThreadOverflow.maxY()) : flowThreadPortionRect.maxY();
        LayoutUnit minX = std::min(flowThreadPortionRect.x(), flowThreadOverflow.x());
        LayoutUnit maxX = std::max(flowThreadPortionRect.maxX(), flowThreadOverflow.maxX());
        clipRect = LayoutRect(minX, minY, maxX - minX, maxY - minY);
    } else {
        LayoutUnit minX = isFirstPortion ? flowThreadOverflow.x() : flowThreadPortionRect.x();
        LayoutUnit maxX = isLastPortion ? std::max(flowThreadPortionRect.maxX(), flowThreadOverflow.maxX()) : flowThreadPortionRect.maxX();
        LayoutUnit minY = std::min(flowThreadPortionRect.y(), (flowThreadOverflow.y()));
        LayoutUnit maxY = std::max(flowThreadPortionRect.y(), (flowThreadOverflow.maxY()));
        clipRect = LayoutRect(minX, minY, maxX - minX, maxY - minY);
    }

    return clipRect;
}

bool LayoutMultiColumnSet::heightIsAuto() const
{
    LayoutMultiColumnFlowThread* flowThread = multiColumnFlowThread();
    if (!flowThread->isLayoutPagedFlowThread()) {
        // If support for the column-fill property isn't enabled, we want to behave as if
        // column-fill were auto, so that multicol containers with specified height don't get their
        // columns balanced (auto-height multicol containers will still get their columns balanced,
        // even if column-fill isn't 'balance' - in accordance with the spec). Pretending that
        // column-fill is auto also matches the old multicol implementation, which has no support
        // for this property.
        if (RuntimeEnabledFeatures::columnFillEnabled()) {
            if (multiColumnBlockFlow()->style()->columnFill() == ColumnFillBalance)
                return true;
        }
        if (LayoutBox* next = nextSiblingBox()) {
            if (next->isLayoutMultiColumnSpannerPlaceholder()) {
                // If we're followed by a spanner, we need to balance.
                return true;
            }
        }
    }
    return !flowThread->columnHeightAvailable();
}

LayoutSize LayoutMultiColumnSet::flowThreadTranslationAtOffset(LayoutUnit blockOffset) const
{
    return fragmentainerGroupAtFlowThreadOffset(blockOffset).flowThreadTranslationAtOffset(blockOffset);
}

LayoutPoint LayoutMultiColumnSet::visualPointToFlowThreadPoint(const LayoutPoint& visualPoint) const
{
    const MultiColumnFragmentainerGroup& row = fragmentainerGroupAtVisualPoint(visualPoint);
    return row.visualPointToFlowThreadPoint(visualPoint - row.offsetFromColumnSet());
}

LayoutUnit LayoutMultiColumnSet::pageLogicalTopForOffset(LayoutUnit offset) const
{
    return fragmentainerGroupAtFlowThreadOffset(offset).columnLogicalTopForOffset(offset);
}

bool LayoutMultiColumnSet::recalculateColumnHeight()
{
    if (m_oldLogicalTop != logicalTop() && multiColumnFlowThread()->enclosingFragmentationContext()) {
        // Preceding spanners or column sets have been moved or resized. This means that the
        // fragmentainer groups that we have inserted need to be re-inserted. Restart column
        // balancing.
        resetColumnHeight();
        return true;
    }

    bool changed = false;
    for (auto& group : m_fragmentainerGroups)
        changed = group.recalculateColumnHeight() || changed;
    m_initialHeightCalculated = true;
    return changed;
}

void LayoutMultiColumnSet::resetColumnHeight()
{
    m_fragmentainerGroups.deleteExtraGroups();
    m_fragmentainerGroups.first().resetColumnHeight();
    m_tallestUnbreakableLogicalHeight = LayoutUnit();
    m_initialHeightCalculated = false;
}

void LayoutMultiColumnSet::beginFlow(LayoutUnit offsetInFlowThread)
{
    // At this point layout is exactly at the beginning of this set. Store block offset from flow
    // thread start.
    m_fragmentainerGroups.first().setLogicalTopInFlowThread(offsetInFlowThread);
}

void LayoutMultiColumnSet::endFlow(LayoutUnit offsetInFlowThread)
{
    // At this point layout is exactly at the end of this set. Store block offset from flow thread
    // start. This set is now considered "flowed", although we may have to revisit it later (with
    // beginFlow()), e.g. if a subtree in the flow thread has to be laid out over again because the
    // initial margin collapsing estimates were wrong.
    m_fragmentainerGroups.last().setLogicalBottomInFlowThread(offsetInFlowThread);
}

void LayoutMultiColumnSet::layout()
{
    if (recalculateColumnHeight())
        multiColumnFlowThread()->setColumnHeightsChanged();
    LayoutBlockFlow::layout();
}

void LayoutMultiColumnSet::computeIntrinsicLogicalWidths(LayoutUnit& minLogicalWidth, LayoutUnit& maxLogicalWidth) const
{
    minLogicalWidth = m_flowThread->minPreferredLogicalWidth();
    maxLogicalWidth = m_flowThread->maxPreferredLogicalWidth();
}

void LayoutMultiColumnSet::computeLogicalHeight(LayoutUnit, LayoutUnit logicalTop, LogicalExtentComputedValues& computedValues) const
{
    LayoutUnit logicalHeight;
    for (const auto& group : m_fragmentainerGroups)
        logicalHeight += group.logicalHeight();
    computedValues.m_extent = logicalHeight;
    computedValues.m_position = logicalTop;
}

PositionWithAffinity LayoutMultiColumnSet::positionForPoint(const LayoutPoint& point)
{
    // Convert the visual point to a flow thread point.
    const MultiColumnFragmentainerGroup& row = fragmentainerGroupAtVisualPoint(point);
    LayoutPoint flowThreadPoint = row.visualPointToFlowThreadPoint(point + row.offsetFromColumnSet());
    // Then drill into the flow thread, where we'll find the actual content.
    return flowThread()->positionForPoint(flowThreadPoint);
}

LayoutUnit LayoutMultiColumnSet::columnGap() const
{
    LayoutBlockFlow* parentBlock = multiColumnBlockFlow();
    if (parentBlock->style()->hasNormalColumnGap())
        return parentBlock->style()->fontDescription().computedPixelSize(); // "1em" is recommended as the normal gap setting. Matches <p> margins.
    return parentBlock->style()->columnGap();
}

unsigned LayoutMultiColumnSet::actualColumnCount() const
{
    // FIXME: remove this method. It's a meaningless question to ask the set "how many columns do
    // you actually have?", since that may vary for each row.
    return firstFragmentainerGroup().actualColumnCount();
}

void LayoutMultiColumnSet::paintObject(const PaintInfo& paintInfo, const LayoutPoint& paintOffset) const
{
    MultiColumnSetPainter(*this).paintObject(paintInfo, paintOffset);
}

LayoutRect LayoutMultiColumnSet::fragmentsBoundingBox(const LayoutRect& boundingBoxInFlowThread) const
{
    LayoutRect result;
    for (const auto& group : m_fragmentainerGroups)
        result.unite(group.fragmentsBoundingBox(boundingBoxInFlowThread));
    return result;
}

void LayoutMultiColumnSet::collectLayerFragments(PaintLayerFragments& fragments, const LayoutRect& layerBoundingBox, const LayoutRect& dirtyRect)
{
    for (const auto& group : m_fragmentainerGroups)
        group.collectLayerFragments(fragments, layerBoundingBox, dirtyRect);
}

void LayoutMultiColumnSet::addOverflowFromChildren()
{
    LayoutRect overflowRect;
    for (const auto& group : m_fragmentainerGroups) {
        LayoutRect rect = group.calculateOverflow();
        rect.move(group.offsetFromColumnSet());
        overflowRect.unite(rect);
    }
    addLayoutOverflow(overflowRect);
    if (!hasOverflowClip())
        addVisualOverflow(overflowRect);
}

void LayoutMultiColumnSet::insertedIntoTree()
{
    LayoutBlockFlow::insertedIntoTree();
    attachToFlowThread();
}

void LayoutMultiColumnSet::willBeRemovedFromTree()
{
    LayoutBlockFlow::willBeRemovedFromTree();
    detachFromFlowThread();
}

void LayoutMultiColumnSet::attachToFlowThread()
{
    if (documentBeingDestroyed())
        return;

    if (!m_flowThread)
        return;

    m_flowThread->addColumnSetToThread(this);
}

void LayoutMultiColumnSet::detachFromFlowThread()
{
    if (m_flowThread) {
        m_flowThread->removeColumnSetFromThread(this);
        m_flowThread = 0;
    }
}

LayoutRect LayoutMultiColumnSet::flowThreadPortionRect() const
{
    LayoutRect portionRect(LayoutUnit(), logicalTopInFlowThread(), pageLogicalWidth(), logicalHeightInFlowThread());
    if (!isHorizontalWritingMode())
        return portionRect.transposedRect();
    return portionRect;
}

}
