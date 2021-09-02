// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: DocumentStyleSheetCollector.cpp
// Description: DocumentStyleSheetCollector Class
//      Author: Ziming Li
//     Created: 2021-07-29
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 *           (C) 2006 Alexey Proskuryakov (ap@webkit.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2012 Apple Inc. All rights reserved.
 * Copyright (C) 2008, 2009 Torch Mobile Inc. All rights reserved. (http://www.torchmobile.com/)
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies)
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
 */

#include "core/dom/DocumentStyleSheetCollector.h"

#include "core/css/CSSStyleSheet.h"
#include "core/css/StyleSheet.h"
#include "core/dom/DocumentStyleSheetCollection.h"

using namespace BlinKit;

namespace blink {

DocumentStyleSheetCollector::DocumentStyleSheetCollector(std::vector<GCMember<StyleSheet>>& sheetsForList, std::vector<GCMember<CSSStyleSheet>>& activeList, std::unordered_set<Document *>& visitedDocuments)
    : m_styleSheetsForStyleSheetList(sheetsForList)
    , m_activeAuthorStyleSheets(activeList)
    , m_visitedDocuments(visitedDocuments)
{
}

DocumentStyleSheetCollector::~DocumentStyleSheetCollector()
{
}

void DocumentStyleSheetCollector::appendActiveStyleSheets(const WillBeHeapVector<RefPtrWillBeMember<CSSStyleSheet>>& sheets)
{
    ASSERT(false); // BKTODO: m_activeAuthorStyleSheets.appendVector(sheets);
}

void DocumentStyleSheetCollector::appendActiveStyleSheet(CSSStyleSheet* sheet)
{
    m_activeAuthorStyleSheets.emplace_back(sheet);
}

void DocumentStyleSheetCollector::appendSheetForList(StyleSheet* sheet)
{
    m_styleSheetsForStyleSheetList.emplace_back(sheet);
}

ActiveDocumentStyleSheetCollector::ActiveDocumentStyleSheetCollector(StyleSheetCollection& collection)
    : DocumentStyleSheetCollector(collection.m_styleSheetsForStyleSheetList, collection.m_activeAuthorStyleSheets, m_visitedDocuments)
{
}

ImportedDocumentStyleSheetCollector::ImportedDocumentStyleSheetCollector(DocumentStyleSheetCollector& collector, std::vector<GCMember<StyleSheet>>& sheetForList)
    : DocumentStyleSheetCollector(sheetForList, collector.m_activeAuthorStyleSheets, collector.m_visitedDocuments)
{
}

}
