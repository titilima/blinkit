// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_document.cpp
// Description: HTMLDocument Class
//      Author: Ziming Li
//     Created: 2020-03-30
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 * Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights
 * reserved.
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
 * Portions are Copyright (C) 2002 Netscape Communications Corporation.
 * Other contributors: David Baron <dbaron@fas.harvard.edu>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * Alternatively, the document type parsing portions of this file may be used
 * under the terms of either the Mozilla Public License Version 1.1, found at
 * http://www.mozilla.org/MPL/ (the "MPL") or the GNU General Public
 * License Version 2.0, found at http://www.fsf.org/copyleft/gpl.html
 * (the "GPL"), in which case the provisions of the MPL or the GPL are
 * applicable instead of those above.  If you wish to allow use of your
 * version of this file only under the terms of one of those two
 * licenses (the MPL or the GPL) and not to allow others to use your
 * version of this file under the LGPL, indicate your decision by
 * deleting the provisions above and replace them with the notice and
 * other provisions required by the MPL or the GPL, as the case may be.
 * If you do not delete the provisions above, a recipient may use your
 * version of this file under any of the LGPL, the MPL or the GPL.
 */

#include "html_document.h"

#include <iterator>
#include <unordered_set>
#include "third_party/blink/renderer/core/html_names.h"

namespace blink {

#ifndef BLINKIT_CRAWLER_ONLY
HTMLDocument::HTMLDocument(const DocumentInit &init) : Document(init)
{
#if 0 // BKTODO:
    if (IsSrcdocDocument() || initializer.ImportsController()) {
        DCHECK(InNoQuirksMode());
        LockCompatibilityMode();
    }
#endif
}

Element* HTMLDocument::CreateElement(const AtomicString &localName, CreateElementFlags flags)
{
    ASSERT(false); // BKTODO:
    return nullptr;
}
#endif

static std::unordered_set<StringImpl *>* CreateHtmlCaseInsensitiveAttributesSet(void)
{
    // This is the list of attributes in HTML 4.01 with values marked as "[CI]" or
    // case-insensitive.  Mozilla treats all other values as case-sensitive, thus
    // so do we.
    std::unordered_set<StringImpl *> *attrSet = new std::unordered_set<StringImpl *>;

    using namespace html_names;
    const QualifiedName *caseInsensitiveAttributes[] = {
        &kAcceptCharsetAttr, &kAcceptAttr,    &kAlignAttr,    &kAlinkAttr,
        &kAxisAttr,          &kBgcolorAttr,   &kCharsetAttr,  &kCheckedAttr,
        &kClearAttr,         &kCodetypeAttr,  &kColorAttr,    &kCompactAttr,
        &kDeclareAttr,       &kDeferAttr,     &kDirAttr,      &kDirectionAttr,
        &kDisabledAttr,      &kEnctypeAttr,   &kFaceAttr,     &kFrameAttr,
        &kHreflangAttr,      &kHttpEquivAttr, &kLangAttr,     &kLanguageAttr,
        &kLinkAttr,          &kMediaAttr,     &kMethodAttr,   &kMultipleAttr,
        &kNohrefAttr,        &kNoresizeAttr,  &kNoshadeAttr,  &kNowrapAttr,
        &kReadonlyAttr,      &kRelAttr,       &kRevAttr,      &kRulesAttr,
        &kScopeAttr,         &kScrollingAttr, &kSelectedAttr, &kShapeAttr,
        &kTargetAttr,        &kTextAttr,      &kTypeAttr,     &kValignAttr,
        &kValuetypeAttr,     &kVlinkAttr
    };

    attrSet->reserve(std::size(caseInsensitiveAttributes));
    for (const QualifiedName *attr : caseInsensitiveAttributes)
        attrSet->insert(attr->LocalName().Impl());

    return attrSet;
}

bool HTMLDocument::IsCaseSensitiveAttribute(const QualifiedName &attributeName)
{
    bool isPossibleHtmlAttr = !attributeName.HasPrefix() && (attributeName.NamespaceURI() == g_null_atom);
    if (!isPossibleHtmlAttr)
        return true;

    static std::unordered_set<StringImpl *> *htmlCaseInsensitiveAttributesSet = CreateHtmlCaseInsensitiveAttributesSet();
    auto it = htmlCaseInsensitiveAttributesSet->find(attributeName.LocalName().Impl());
    return std::end(*htmlCaseInsensitiveAttributesSet) == it;
}

}  // namespace blink
