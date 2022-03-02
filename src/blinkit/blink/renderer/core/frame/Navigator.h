#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: Navigator.h
// Description: Navigator Class
//      Author: Ziming Li
//     Created: 2021-07-17
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
    Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef Navigator_h
#define Navigator_h

#include "blinkit/blink/renderer/bindings/core/script_wrappable.h"
#include "blinkit/blink/renderer/core/frame/DOMWindowProperty.h"
#include "blinkit/blink/renderer/platform/Supplementable.h"
#include "blinkit/blink/renderer/wtf/Forward.h"

namespace blink {

class LocalFrame;

typedef int ExceptionCode;

class Navigator final
    : public ScriptWrappable
    , public DOMWindowProperty
    , public HeapSupplementable<Navigator> {
public:
    static std::unique_ptr<Navigator> create(LocalFrame* frame)
    {
        return zed::wrap_unique(new Navigator(frame));
    }

    virtual ~Navigator();

    bool cookieEnabled() const;

    String productSub() const;
    String vendor() const;
    String vendorSub() const;

    String userAgent() const;

    // NavigatorLanguage
    Vector<String> languages();

    DECLARE_VIRTUAL_TRACE();

private:
    explicit Navigator(LocalFrame*);

    BlinKit::GCObject* ObjectForGC(void) override { return nullptr; }
};

} // namespace blink

#endif // Navigator_h
