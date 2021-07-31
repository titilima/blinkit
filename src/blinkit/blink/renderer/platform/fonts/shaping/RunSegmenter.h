// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RunSegmenter_h
#define RunSegmenter_h

#include "platform/fonts/FontOrientation.h"
#include "platform/fonts/FontTraits.h"
#include "platform/fonts/OrientationIterator.h"
#include "platform/fonts/ScriptRunIterator.h"
#include "platform/fonts/SmallCapsIterator.h"
#include "platform/fonts/UTF16TextIterator.h"
#include "wtf/Allocator.h"
#include "wtf/Noncopyable.h"

#include <unicode/uscript.h>

namespace blink {

// A tool for segmenting runs prior to shaping, combining ScriptIterator,
// OrientationIterator and SmallCapsIterator, depending on orientaton and
// font-variant of the text run.
class PLATFORM_EXPORT RunSegmenter {
    STACK_ALLOCATED();
    WTF_MAKE_NONCOPYABLE(RunSegmenter);
public:

    // Indices into the UTF-16 buffer that is passed in
    struct RunSegmenterRange {
        DISALLOW_NEW();
        unsigned start;
        unsigned end;
        UScriptCode script;
        OrientationIterator::RenderOrientation renderOrientation;
        SmallCapsIterator::SmallCapsBehavior smallCapsBehavior;
    };

    RunSegmenter(const UChar* buffer, unsigned bufferSize, FontOrientation, FontVariant);

    bool consume(RunSegmenterRange*);

private:

    void consumeOrientationIteratorPastLastSplit();
    void consumeSmallCapsIteratorPastLastSplit();
    void consumeScriptIteratorPastLastSplit();

    unsigned m_bufferSize;
    RunSegmenterRange m_candidateRange;
    OwnPtr<ScriptRunIterator> m_scriptRunIterator;
    OwnPtr<OrientationIterator> m_orientationIterator;
    OwnPtr<SmallCapsIterator> m_smallCapsIterator;
    unsigned m_lastSplit;
    unsigned m_scriptRunIteratorPosition;
    unsigned m_orientationIteratorPosition;
    unsigned m_smallCapsIteratorPosition;
    bool m_atEnd;
};

} // namespace blink

#endif
