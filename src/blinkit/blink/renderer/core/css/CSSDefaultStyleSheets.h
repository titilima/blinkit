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
#if 0 // BKTODO:
    RuleSet* defaultPrintStyle() { return m_defaultPrintStyle.get(); }
    RuleSet* defaultViewSourceStyle();
    RuleSet* defaultMobileViewportStyle();

    // FIXME: Remove WAP support.
    RuleSet* defaultXHTMLMobileProfileStyle();
#endif

    StyleSheetContents* defaultStyleSheet() { return m_defaultStyleSheet.get(); }
    StyleSheetContents* quirksStyleSheet() { return m_quirksStyleSheet.get(); }
#if 0 // BKTODO:
    StyleSheetContents* svgStyleSheet() { return m_svgStyleSheet.get(); }
    StyleSheetContents* mathmlStyleSheet() { return m_mathmlStyleSheet.get(); }
    StyleSheetContents* mediaControlsStyleSheet() { return m_mediaControlsStyleSheet.get(); }
    StyleSheetContents* fullscreenStyleSheet() { return m_fullscreenStyleSheet.get(); }
#endif

    DECLARE_TRACE();

private:
    CSSDefaultStyleSheets();

    BlinKit::GCMember<RuleSet> m_defaultStyle;
    // BKTODO: OwnPtrWillBeMember<RuleSet> m_defaultMobileViewportStyle;
    BlinKit::GCMember<RuleSet> m_defaultQuirksStyle;
#if 0 // BKTODO:
    OwnPtrWillBeMember<RuleSet> m_defaultPrintStyle;
    OwnPtrWillBeMember<RuleSet> m_defaultViewSourceStyle;
    OwnPtrWillBeMember<RuleSet> m_defaultXHTMLMobileProfileStyle;
#endif

    BlinKit::GCMember<StyleSheetContents> m_defaultStyleSheet;
    // BKTODO: RefPtrWillBeMember<StyleSheetContents> m_mobileViewportStyleSheet;
    BlinKit::GCMember<StyleSheetContents> m_quirksStyleSheet;
#if 0 // BKTODO:
    RefPtrWillBeMember<StyleSheetContents> m_svgStyleSheet;
    RefPtrWillBeMember<StyleSheetContents> m_mathmlStyleSheet;
    RefPtrWillBeMember<StyleSheetContents> m_mediaControlsStyleSheet;
    RefPtrWillBeMember<StyleSheetContents> m_fullscreenStyleSheet;
#endif
};

} // namespace blink

#endif // CSSDefaultStyleSheets_h
