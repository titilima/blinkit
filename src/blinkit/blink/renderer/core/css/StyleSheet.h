#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: StyleSheet.h
// Description: StyleSheet Class
//      Author: Ziming Li
//     Created: 2021-08-21
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2006, 2008, 2012 Apple Inc. All rights reserved.
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

#ifndef StyleSheet_h
#define StyleSheet_h

#include "blinkit/blink/renderer/bindings/core/script_wrappable.h"
#include "blinkit/blink/renderer/wtf/Forward.h"
#include "blinkit/blink/renderer/wtf/RefCounted.h"

namespace blink {

class CSSRule;
class KURL;
class MediaList;
class Node;
class StyleSheet;

class StyleSheet : public BlinKit::GCObject, public ScriptWrappable
{
    DEFINE_WRAPPERTYPEINFO();
public:
    StyleSheet() { }
    virtual ~StyleSheet();

    virtual bool disabled() const = 0;
    virtual void setDisabled(bool) = 0;
    virtual Node* ownerNode() const = 0;
    virtual StyleSheet* parentStyleSheet() const { return 0; }
    virtual String href() const = 0;
    virtual String title() const = 0;
    virtual MediaList* media() const { return 0; }
    virtual String type() const = 0;

    virtual CSSRule* ownerRule() const { return 0; }
    virtual void clearOwnerNode() = 0;
    virtual KURL baseURL() const = 0;
    virtual bool isLoading() const = 0;
    virtual bool isCSSStyleSheet() const { return false; }

    DEFINE_INLINE_VIRTUAL_TRACE() { }
};

} // namespace blink

#endif
