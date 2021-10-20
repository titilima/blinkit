// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: UndoStack.cpp
// Description: UndoStack Class
//      Author: Ziming Li
//     Created: 2021-10-18
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006, 2007 Apple, Inc.  All rights reserved.
 * Copyright (C) 2012 Google, Inc.  All rights reserved.
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

#include "core/editing/commands/UndoStack.h"

#include "core/dom/ContainerNode.h"
#include "core/editing/commands/UndoStep.h"
#include "platform/EventDispatchForbiddenScope.h"
#include "wtf/TemporaryChange.h"

namespace blink {

// Arbitrary depth limit for the undo stack, to keep it from using
// unbounded memory. This is the maximum number of distinct undoable
// actions -- unbroken stretches of typed characters are coalesced
// into a single action.
static const size_t maximumUndoStackDepth = 1000;

UndoStack::UndoStack()
    : m_inRedo(false)
{
}

std::unique_ptr<UndoStack> UndoStack::create()
{
    return zed::wrap_unique(new UndoStack());
}

void UndoStack::registerUndoStep(PassRefPtrWillBeRawPtr<UndoStep> step)
{
    if (m_undoStack.size() == maximumUndoStackDepth)
        m_undoStack.pop_front(); // drop oldest item off the far end
    if (!m_inRedo)
        m_redoStack.clear();
    m_undoStack.emplace_back(step);
}

void UndoStack::registerRedoStep(PassRefPtrWillBeRawPtr<UndoStep> step)
{
    m_redoStack.emplace_back(step);
}

void UndoStack::didUnloadFrame(const LocalFrame& frame)
{
    EventDispatchForbiddenScope assertNoEventDispatch;
    filterOutUndoSteps(m_undoStack, frame);
    filterOutUndoSteps(m_redoStack, frame);
}

void UndoStack::filterOutUndoSteps(UndoStepStack& stack, const LocalFrame& frame)
{
    UndoStepStack newStack;
    while (!stack.empty()) {
        UndoStep* step = stack.front().get();
        if (!step->belongsTo(frame))
            newStack.emplace_back(step);
        stack.pop_front();
    }
    stack.swap(newStack);
}

bool UndoStack::canUndo() const
{
    return !m_undoStack.empty();
}

bool UndoStack::canRedo() const
{
    return !m_redoStack.empty();
}

void UndoStack::undo()
{
    if (canUndo()) {
        UndoStepStack::iterator back = --m_undoStack.end();
        GCRefPtr<UndoStep> step(back->get());
        m_undoStack.erase(back);
        step->unapply();
        // unapply will call us back to push this command onto the redo stack.
    }
}

void UndoStack::redo()
{
    if (canRedo()) {
        UndoStepStack::iterator back = --m_redoStack.end();
        GCRefPtr<UndoStep> step(back->get());
        m_redoStack.erase(back);

        ASSERT(!m_inRedo);
        TemporaryChange<bool> redoScope(m_inRedo, true);
        step->reapply();
        // reapply will call us back to push this command onto the undo stack.
    }
}

} // namespace blink
