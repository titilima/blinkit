// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: DeleteFromTextNodeCommand.h
// Description: DeleteFromTextNodeCommand Class
//      Author: Ziming Li
//     Created: 2021-10-10
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2005, 2006, 2008 Apple Inc. All rights reserved.
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

#ifndef DeleteFromTextNodeCommand_h
#define DeleteFromTextNodeCommand_h

#include "core/editing/commands/EditCommand.h"

namespace blink {

class Text;

class DeleteFromTextNodeCommand final : public SimpleEditCommand {
public:
    static PassRefPtrWillBeRawPtr<DeleteFromTextNodeCommand> create(PassRefPtrWillBeRawPtr<Text> node, unsigned offset, unsigned count)
    {
        return adoptRefWillBeNoop(new DeleteFromTextNodeCommand(node, offset, count));
    }

    DECLARE_VIRTUAL_TRACE();

private:
    DeleteFromTextNodeCommand(PassRefPtrWillBeRawPtr<Text>, unsigned offset, unsigned count);

    void doApply() override;
    void doUnapply() override;

    GCRefPtr<Text> m_node;
    unsigned m_offset;
    unsigned m_count;
    String m_text;
};

} // namespace blink

#endif // DeleteFromTextNodeCommand_h
