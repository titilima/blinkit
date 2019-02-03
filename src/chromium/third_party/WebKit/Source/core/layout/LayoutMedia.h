/*
 * Copyright (C) 2007, 2008, 2009, 2010 Apple Inc. All rights reserved.
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

#ifndef LayoutMedia_h
#define LayoutMedia_h

#include "core/layout/LayoutImage.h"

namespace blink {

class HTMLMediaElement;

class LayoutMedia : public LayoutImage {
public:
    explicit LayoutMedia(HTMLMediaElement*);
    ~LayoutMedia() override;

    LayoutObject* firstChild() const { ASSERT(children() == virtualChildren()); return children()->firstChild(); }
    LayoutObject* lastChild() const { ASSERT(children() == virtualChildren()); return children()->lastChild(); }

    // If you have a LayoutMedia, use firstChild or lastChild instead.
    void slowFirstChild() const = delete;
    void slowLastChild() const = delete;

    const LayoutObjectChildList* children() const { return &m_children; }
    LayoutObjectChildList* children() { return &m_children; }

    HTMLMediaElement* mediaElement() const;

    const char* name() const override { return "LayoutMedia"; }

    // Temporary callback for crbug.com/587345,402044
    void notifyPositionMayHaveChanged(const IntRect&);

    // Change whether we want or don't want to receive position change
    // notifications.  This will cause us to start / stop receiving change
    // notifications if possible.
    // Temporary method for crbug.com/587345,402044
    void setRequestPositionUpdates(bool);

protected:
    // Temporary overrides for crbug.com/587345,402044
    void willBeDestroyed() override;
    void insertedIntoTree() override;

    void layout() override;

    bool isOfType(LayoutObjectType type) const override { return type == LayoutObjectMedia || LayoutImage::isOfType(type); }

private:
    LayoutObjectChildList* virtualChildren() final { return children(); }
    const LayoutObjectChildList* virtualChildren() const final { return children(); }

    PaintLayerType layerTypeRequired() const override { return NormalPaintLayer; }

    bool canHaveChildren() const final { return true; }
    bool isChildAllowed(LayoutObject*, const ComputedStyle&) const final;

    bool isImage() const final { return false; }
    void paintReplaced(const PaintInfo&, const LayoutPoint&) const override;

    bool backgroundShouldAlwaysBeClipped() const final { return false; }

    LayoutObjectChildList m_children;
};

DEFINE_LAYOUT_OBJECT_TYPE_CASTS(LayoutMedia, isMedia());

} // namespace blink

#endif // LayoutMedia_h
