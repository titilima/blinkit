// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ScriptLoaderClient.h
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

#include "core/CoreExport.h"
#include "wtf/text/WTFString.h"

namespace blink {

class CORE_EXPORT ScriptLoaderClient {
public:
    virtual ~ScriptLoaderClient() { }

    virtual void dispatchLoadEvent() = 0;

    virtual String sourceAttributeValue() const = 0;
    virtual String charsetAttributeValue() const = 0;
    virtual String typeAttributeValue() const = 0;
    virtual String languageAttributeValue() const = 0;
#ifdef BLINKIT_CRAWLER_ENABLED
    virtual String forAttributeValue() const = 0;
    virtual String eventAttributeValue() const = 0;
#endif
    virtual bool asyncAttributeValue() const = 0;
    virtual bool deferAttributeValue() const = 0;
    virtual bool hasSourceAttribute() const = 0;
};

}

#endif
