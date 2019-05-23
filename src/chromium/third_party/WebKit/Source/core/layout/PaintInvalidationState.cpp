// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: PaintInvalidationState.cpp
// Description: PaintInvalidationState Class
//      Author: Ziming Li
//     Created: 2019-05-23
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/layout/PaintInvalidationState.h"

#include "core/layout/LayoutInline.h"
#include "core/layout/LayoutView.h"
#include "core/paint/PaintLayer.h"

namespace blink {

PaintInvalidationState::PaintInvalidationState(const LayoutView& layoutView, Vector<LayoutObject*>& pendingDelayedPaintInvalidations, PaintInvalidationState* ownerPaintInvalidationState)
    : m_clipped(false)
    , m_cachedOffsetsEnabled(true)
    , m_forcedSubtreeInvalidationWithinContainer(false)
    , m_forcedSubtreeInvalidationRectUpdateWithinContainer(false)
    , m_viewClippingAndScrollOffsetDisabled(false)
    , m_paintInvalidationContainer(layoutView.containerForPaintInvalidation())
    , m_pendingDelayedPaintInvalidations(pendingDelayedPaintInvalidations)
{
    bool establishesPaintInvalidationContainer = layoutView == m_paintInvalidationContainer;
    if (!establishesPaintInvalidationContainer) {
        if ((ownerPaintInvalidationState && !ownerPaintInvalidationState->m_cachedOffsetsEnabled)
            || !layoutView.supportsPaintInvalidationStateCachedOffsets()) {
            m_cachedOffsetsEnabled = false;
            return;
        }
        if (ownerPaintInvalidationState && ownerPaintInvalidationState->m_forcedSubtreeInvalidationWithinContainer)
            m_forcedSubtreeInvalidationWithinContainer = true;
        FloatPoint point = layoutView.localToAncestorPoint(FloatPoint(), &m_paintInvalidationContainer, TraverseDocumentBoundaries);
        m_paintOffset = LayoutSize(point.x(), point.y());
    }
    m_clipRect = layoutView.viewRect();
    m_clipRect.move(m_paintOffset);
    m_clipped = true;
}

PaintInvalidationState::PaintInvalidationState(PaintInvalidationState& next, LayoutBoxModelObject& layoutObject, const LayoutBoxModelObject& paintInvalidationContainer)
    : m_clipped(false)
    , m_cachedOffsetsEnabled(true)
    , m_forcedSubtreeInvalidationWithinContainer(next.m_forcedSubtreeInvalidationWithinContainer)
    , m_forcedSubtreeInvalidationRectUpdateWithinContainer(next.m_forcedSubtreeInvalidationRectUpdateWithinContainer)
    , m_viewClippingAndScrollOffsetDisabled(false)
    , m_paintInvalidationContainer(paintInvalidationContainer)
    , m_pendingDelayedPaintInvalidations(next.pendingDelayedPaintInvalidationTargets())
{
    // FIXME: SVG could probably benefit from a stack-based optimization like html does. crbug.com/391054
    bool establishesPaintInvalidationContainer = layoutObject == m_paintInvalidationContainer;
    bool fixed = layoutObject.style()->position() == FixedPosition;

    if (!layoutObject.supportsPaintInvalidationStateCachedOffsets() || !next.m_cachedOffsetsEnabled)
        m_cachedOffsetsEnabled = false;
    if (establishesPaintInvalidationContainer) {
        // When we hit a new paint invalidation container, we don't need to
        // continue forcing a check for paint invalidation, since we're
        // descending into a different invalidation container. (For instance if
        // our parents were moved, the entire container will just move.)
        m_forcedSubtreeInvalidationWithinContainer = false;
    } else {
        if (m_cachedOffsetsEnabled) {
            if (fixed) {
                FloatPoint fixedOffset = layoutObject.localToAncestorPoint(FloatPoint(), &m_paintInvalidationContainer, TraverseDocumentBoundaries);
                m_paintOffset = LayoutSize(fixedOffset.x(), fixedOffset.y());
            } else {
                LayoutSize offset = layoutObject.isBox() && !layoutObject.isTableRow() ? toLayoutBox(layoutObject).locationOffset() : LayoutSize();
                m_paintOffset = next.m_paintOffset + offset;
            }

            if (layoutObject.isOutOfFlowPositioned() && !fixed) {
                if (LayoutObject* container = layoutObject.container()) {
                    if (container->style()->hasInFlowPosition() && container->isLayoutInline())
                        m_paintOffset += toLayoutInline(container)->offsetForInFlowPositionedInline(toLayoutBox(layoutObject));
                }
            }

            if (layoutObject.style()->hasInFlowPosition() && layoutObject.hasLayer())
                m_paintOffset += layoutObject.layer()->offsetForInFlowPosition();
        }

        m_clipped = !fixed && next.m_clipped;
        if (m_clipped)
            m_clipRect = next.m_clipRect;
    }

    applyClipIfNeeded(layoutObject);

    // FIXME: <http://bugs.webkit.org/show_bug.cgi?id=13443> Apply control clip if present.
}

void PaintInvalidationState::addClipRectRelativeToPaintOffset(const LayoutSize& clipSize)
{
    LayoutRect clipRect(toPoint(m_paintOffset), clipSize);
    if (m_clipped) {
        m_clipRect.intersect(clipRect);
    } else {
        m_clipRect = clipRect;
        m_clipped = true;
    }
}

void PaintInvalidationState::applyClipIfNeeded(const LayoutObject& layoutObject)
{
    if (!layoutObject.hasOverflowClip())
        return;

    const LayoutBox& box = toLayoutBox(layoutObject);

    // Do not clip scroll layer contents because the compositor expects the whole layer
    // to be always invalidated in-time.
    if (box.usesCompositedScrolling())
        ASSERT(!m_clipped); // The box should establish paint invalidation container, so no m_clipped inherited.
    else
        addClipRectRelativeToPaintOffset(LayoutSize(box.layer()->size()));

    m_paintOffset -= box.scrolledContentOffset();
}

} // namespace blink
