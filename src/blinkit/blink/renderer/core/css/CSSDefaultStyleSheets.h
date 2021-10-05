// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSDefaultStyleSheets.h
// Description: CSSDefaultStyleSheets Class
//      Author: Ziming Li
//     Created: 2021-08-15
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011 Apple Inc. All rights reserved.
 * Copyright (C) 2013 Google Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef CSSDefaultStyleSheets_h
#define CSSDefaultStyleSheets_h

 // -------------------------------------------------
 // BlinKit - BlinKit Library
 // -------------------------------------------------
 //   File Name: CSSDefaultStyleSheets.h
 // Description: CSSDefaultStyleSheets Class
 //      Author: Ziming Li
 //     Created: 2021-07-29
 // -------------------------------------------------
 // Copyright (C) 2021 MingYang Software Technology.
 // -------------------------------------------------

#include "platform/heap/Handle.h"
#include "wtf/Allocator.h"

namespace blink {

class Element;
class RuleSet;
class StyleSheetContents;

class CSSDefaultStyleSheets : public BlinKit::GCObject {
    USING_FAST_MALLOC_WILL_BE_REMOVED(CSSDefaultStyleSheets);
    WTF_MAKE_NONCOPYABLE(CSSDefaultStyleSheets);
public:
    static CSSDefaultStyleSheets& instance();

    void ensureDefaultStyleSheetsForElement(const Element&, bool& changedDefaultStyle);

    RuleSet* defaultStyle() { return m_defaultStyle.get(); }
    RuleSet* defaultQuirksStyle() { return m_defaultQuirksStyle.get(); }

    StyleSheetContents* defaultStyleSheet(void);
    StyleSheetContents* quirksStyleSheet(void);

    DECLARE_TRACE();

private:
    CSSDefaultStyleSheets();

    GCRefPtr<RuleSet> m_defaultStyle;
    GCRefPtr<RuleSet> m_defaultQuirksStyle;

    GCRefPtr<StyleSheetContents> m_defaultStyleSheet;
    GCRefPtr<StyleSheetContents> m_quirksStyleSheet;
};

} // namespace blink

#endif // CSSDefaultStyleSheets_h
