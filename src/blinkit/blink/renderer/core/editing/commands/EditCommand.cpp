// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: EditCommand.cpp
// Description: EditCommand Class
//      Author: Ziming Li
//     Created: 2021-10-09
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2005, 2006, 2007 Apple, Inc.  All rights reserved.
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

#include "./EditCommand.h"

#include "blinkit/blink/renderer/core/dom/Document.h"
#include "blinkit/blink/renderer/core/dom/node_traversal.h"
#include "blinkit/blink/renderer/core/editing/FrameSelection.h"
#include "blinkit/blink/renderer/core/editing/commands/CompositeEditCommand.h"
#include "blinkit/blink/renderer/core/frame/LocalFrame.h"
#include "blinkit/blink/renderer/core/layout/LayoutText.h"

namespace blink {

EditCommand::EditCommand(Document& document)
    : m_document(document)
    , m_parent(nullptr)
{
    ASSERT(m_document.frame());
    setStartingSelection(m_document.frame()->selection().selection());
    setEndingSelection(m_startingSelection);
}

EditCommand::~EditCommand()
{
}

EditAction EditCommand::editingAction() const
{
    return EditActionUnspecified;
}

static inline EditCommandComposition* compositionIfPossible(EditCommand* command)
{
    if (!command->isCompositeEditCommand())
        return 0;
    return toCompositeEditCommand(command)->composition();
}

void EditCommand::setStartingSelection(const VisibleSelection& selection)
{
    for (EditCommand* command = this; ; command = command->m_parent) {
        if (EditCommandComposition* composition = compositionIfPossible(command)) {
            ASSERT(command->isTopLevelCommand());
            composition->setStartingSelection(selection);
        }
        command->m_startingSelection = selection;
        if (!command->m_parent || command->m_parent->isFirstCommand(command))
            break;
    }
}

void EditCommand::setEndingSelection(const VisibleSelection& selection)
{
    for (EditCommand* command = this; command; command = command->m_parent) {
        if (EditCommandComposition* composition = compositionIfPossible(command)) {
            ASSERT(command->isTopLevelCommand());
            composition->setEndingSelection(selection);
        }
        command->m_endingSelection = selection;
    }
}

void EditCommand::setEndingSelection(const VisiblePosition& position)
{
    setEndingSelection(VisibleSelection(position));
}

bool EditCommand::isRenderedCharacter(const Position& position)
{
    if (position.isNull())
        return false;
    ASSERT(position.isOffsetInAnchor());
    if (!position.anchorNode()->isTextNode())
        return false;

    LayoutObject* layoutObject = position.anchorNode()->layoutObject();
    if (!layoutObject)
        return false;

    return toLayoutText(layoutObject)->isRenderedCharacter(position.offsetInContainerNode());
}

void EditCommand::setParent(CompositeEditCommand* parent)
{
    ASSERT((parent && !m_parent) || (!parent && m_parent));
    ASSERT(!parent || !isCompositeEditCommand() || !toCompositeEditCommand(this)->composition());
    m_parent = parent;
    if (parent) {
        m_startingSelection = parent->m_endingSelection;
        m_endingSelection = parent->m_endingSelection;
    }
}

void SimpleEditCommand::doReapply()
{
    doApply();
}

DEFINE_TRACE(EditCommand)
{
    visitor->trace(m_startingSelection);
    visitor->trace(m_endingSelection);
}

} // namespace blink
