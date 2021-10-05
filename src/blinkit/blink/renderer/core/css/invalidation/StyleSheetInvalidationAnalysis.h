// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: StyleSheetInvalidationAnalysis.h
// Description: StyleSheetInvalidationAnalysis Class
//      Author: Ziming Li
//     Created: 2021-08-21
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2012 Apple Inc. All rights reserved.
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

#ifndef StyleSheetInvalidationAnalysis_h
#define StyleSheetInvalidationAnalysis_h

#include <unordered_set>
#include "platform/heap/Handle.h"
// BKTODO: #include "wtf/HashSet.h"
// BKTODO: #include "wtf/Vector.h"
#include "wtf/text/StringImpl.h"

namespace blink {

class Document;
class Node;
class StyleSheetContents;
class TreeScope;

class StyleSheetInvalidationAnalysis {
    STACK_ALLOCATED();
public:
    StyleSheetInvalidationAnalysis(const TreeScope&, const std::vector<GCRefPtr<StyleSheetContents>>&);

    bool dirtiesAllStyle() const { return m_dirtiesAllStyle; }
    void invalidateStyle();

private:

    void analyzeStyleSheet(StyleSheetContents*);

    const TreeScope *m_treeScope;
    std::unordered_set<StringImpl*> m_idScopes;
    std::unordered_set<StringImpl*> m_classScopes;

    bool m_dirtiesAllStyle = false;
};

}

#endif
