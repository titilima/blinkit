// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: SizesAttributeParser.h
// Description: SizesAttributeParser Class
//      Author: Ziming Li
//     Created: 2021-10-23
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SizesAttributeParser_h
#define SizesAttributeParser_h

#include "core/CoreExport.h"
#include "core/css/MediaValues.h"
#include "core/css/parser/MediaQueryBlockWatcher.h"
#include "core/css/parser/MediaQueryParser.h"
#include "platform/heap/Handle.h"
#include "wtf/text/WTFString.h"

namespace blink {

class CORE_EXPORT SizesAttributeParser {
    STACK_ALLOCATED();
public:
    SizesAttributeParser(PassRefPtrWillBeRawPtr<MediaValues>, const String&);

    float length();

private:
    bool parse(CSSParserTokenRange);
    float effectiveSize();
    bool calculateLengthInPixels(CSSParserTokenRange, float& result);
    bool mediaConditionMatches(const MediaQuerySet *mediaCondition);
    unsigned effectiveSizeDefaultValue();

    RefPtrWillBeMember<MediaValues> m_mediaValues;
    float m_length;
    bool m_lengthWasSet;
    bool m_isValid;
};

} // namespace

#endif

