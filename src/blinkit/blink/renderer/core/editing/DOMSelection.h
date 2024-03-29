#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: DOMSelection.h
// Description: DOMSelection Class
//      Author: Ziming Li
//     Created: 2021-10-10
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2007 Apple Inc.  All rights reserved.
 * Copyright (C) 2012 Google Inc.  All rights reserved.
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


#ifndef DOMSelection_h
#define DOMSelection_h

#include "blinkit/blink/renderer/bindings/core/script_wrappable.h"
#include "blinkit/blink/renderer/core/editing/Position.h"
#include "blinkit/blink/renderer/core/editing/VisibleSelection.h"
#include "blinkit/blink/renderer/core/frame/DOMWindowProperty.h"
#include "blinkit/blink/renderer/wtf/Forward.h"
#include "blinkit/blink/renderer/wtf/PassRefPtr.h"
#include "blinkit/blink/renderer/wtf/RefCounted.h"

namespace blink {

class ExceptionState;
class Node;
class Range;
class TreeScope;

class DOMSelection final : public ScriptWrappable, public DOMWindowProperty {
public:
    static std::unique_ptr<DOMSelection> create(const TreeScope* treeScope)
    {
        return zed::wrap_unique(new DOMSelection(treeScope));
    }

    void clearTreeScope();

    // Safari Selection Object API
    // These methods return the valid equivalents of internal editing positions.
    Node* baseNode() const;
    int baseOffset() const;
    Node* extentNode() const;
    int extentOffset() const;
    String type() const;
    void setBaseAndExtent(Node* baseNode, int baseOffset, Node* extentNode, int extentOffset, ExceptionState&);
    void modify(const String& alter, const String& direction, const String& granularity);

    // Mozilla Selection Object API
    // In Firefox, anchor/focus are the equal to the start/end of the selection,
    // but reflect the direction in which the selection was made by the user. That does
    // not mean that they are base/extent, since the base/extent don't reflect
    // expansion.
    // These methods return the valid equivalents of internal editing positions.
    Node* anchorNode() const;
    int anchorOffset() const;
    Node* focusNode() const;
    int focusOffset() const;
    bool isCollapsed() const;
    int rangeCount() const;
    void collapse(Node*, int offset, ExceptionState&);
    void collapseToEnd(ExceptionState&);
    void collapseToStart(ExceptionState&);
    void extend(Node*, int offset, ExceptionState&);
    GCRefPtr<Range> getRangeAt(int, ExceptionState&);
    void removeAllRanges();
    void addRange(Range*);
    void deleteFromDocument();
    bool containsNode(const Node*, bool partlyContained) const;
    void selectAllChildren(Node*, ExceptionState&);

    String toString();

    // Microsoft Selection Object API
    void empty();

private:
    explicit DOMSelection(const TreeScope*);

    // Convenience method for accessors, does not check m_frame present.
    const VisibleSelection& visibleSelection() const;

    Node* shadowAdjustedNode(const Position&) const;
    int shadowAdjustedOffset(const Position&) const;

    bool isValidForPosition(Node*) const;

    void addConsoleError(const String& message);

    const TreeScope *m_treeScope;
};

} // namespace blink

#endif // DOMSelection_h
