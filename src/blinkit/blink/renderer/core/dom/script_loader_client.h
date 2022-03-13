#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: script_loader_client.h
// Description: ScriptLoaderClient Class
//      Author: Ziming Li
//     Created: 2021-11-06
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2008 Nikolas Zimmermann <zimmermann@kde.org>
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
#ifndef ScriptLoaderClient_h
#define ScriptLoaderClient_h

#include "blinkit/blink/renderer/wtf/text/WTFString.h"

namespace blink {

class ScriptLoaderClient
{
public:
    virtual ~ScriptLoaderClient(void) = default;

    virtual void dispatchLoadEvent(void) = 0;

    virtual String sourceAttributeValue(void) const = 0;
    virtual String charsetAttributeValue(void) const = 0;
    virtual String typeAttributeValue(void) const = 0;
    virtual String languageAttributeValue(void) const = 0;
#ifdef BLINKIT_CRAWLER_ENABLED
    String forAttributeValue(void) const {
        return String();
    }
    String eventAttributeValue(void) const {
        return String();
    }
#endif
    constexpr bool asyncAttributeValue(void) const {
        return false;
    }
    const bool deferAttributeValue(void) const {
        return false;
    }
    virtual bool hasSourceAttribute(void) const = 0;
};

}

#endif
