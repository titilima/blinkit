// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: document_init.cpp
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

#include "document_init.h"

#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/loader/document_loader.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "third_party/blink/renderer/core/html/imports/html_imports_controller.h"
#endif
namespace blink {

#ifndef BLINKIT_CRAWLER_ONLY
DocumentInit::DocumentInit(const std::shared_ptr<HTMLImportsController> &importsController) : m_importsController(importsController)
{
}
#endif

DocumentInit::DocumentInit(const DocumentInit &) = default;

DocumentInit::~DocumentInit(void) = default;

LocalFrame* DocumentInit::GetFrame(void) const
{
    return m_documentLoader ? m_documentLoader->GetFrame() : nullptr;
}

bool DocumentInit::ShouldSetURL(void) const
{
    return !m_URL.is_empty();
}

DocumentInit& DocumentInit::WithDocumentLoader(DocumentLoader *loader)
{
    ASSERT(!m_documentLoader);
    m_documentLoader = loader;
    return *this;
}

DocumentInit& DocumentInit::WithURL(const GURL &URL)
{
    ASSERT(m_URL.is_empty());
    m_URL = URL;
    return *this;
}

}  // namespace blink
