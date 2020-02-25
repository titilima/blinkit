// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: document_init.h
// Description: DocumentInit Class
//      Author: Ziming Li
//     Created: 2019-10-09
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 *           (C) 2006 Alexey Proskuryakov (ap@webkit.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2012 Apple Inc. All
 * rights reserved.
 * Copyright (C) 2008, 2009 Torch Mobile Inc. All rights reserved.
 * (http://www.torchmobile.com/)
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
 *
 */

#ifndef BLINKIT_BLINK_DOCUMENT_INIT_H
#define BLINKIT_BLINK_DOCUMENT_INIT_H

#pragma once

#include "third_party/blink/renderer/platform/heap/handle.h"
#include "url/bk_url.h"

namespace blink {

class DocumentLoader;
class LocalFrame;

class DocumentInit final
{
    STACK_ALLOCATED();
public:
    DocumentInit(const DocumentInit &init);
    ~DocumentInit(void);

    static DocumentInit Create(void)
    {
        return DocumentInit();
    }

    DocumentInit& WithDocumentLoader(DocumentLoader *loader);
    LocalFrame* GetFrame(void) const;

    DocumentInit& WithURL(const BlinKit::BkURL &URL);
    const BlinKit::BkURL& Url(void) const { return m_URL; }

    bool ShouldSetURL(void) const;
private:
    DocumentInit(void) = default;

    Member<DocumentLoader> m_documentLoader;
    BlinKit::BkURL m_URL;
};

}  // namespace blink

#endif  // BLINKIT_BLINK_DOCUMENT_INIT_H
