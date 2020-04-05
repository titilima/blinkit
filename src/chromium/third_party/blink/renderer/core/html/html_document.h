// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_document.h
// Description: HTMLDocument Class
//      Author: Ziming Li
//     Created: 2020-03-30
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 * Copyright (C) 2004, 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
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

#ifndef BLINKIT_BLINK_HTML_DOCUMENT_H
#define BLINKIT_BLINK_HTML_DOCUMENT_H

#pragma once

#include "third_party/blink/renderer/core/dom/document.h"

namespace blink {

class HTMLDocument : public Document
{
public:
    static bool IsCaseSensitiveAttribute(const QualifiedName &attributeName);
};

} // namespace blink

#endif // BLINKIT_BLINK_HTML_DOCUMENT_H
