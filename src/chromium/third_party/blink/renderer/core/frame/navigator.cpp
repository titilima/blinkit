// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: navigator.cpp
// Description: Navigator Class
//      Author: Ziming Li
//     Created: 2020-03-25
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 *  Copyright (C) 2000 Harri Porten (porten@kde.org)
 *  Copyright (c) 2000 Daniel Molkentin (molkentin@kde.org)
 *  Copyright (c) 2000 Stefan Schimanski (schimmi@kde.org)
 *  Copyright (C) 2003, 2004, 2005, 2006 Apple Computer, Inc.
 *  Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA 02110-1301, USA
 */

#include "navigator.h"

#include "blinkit/crawler/crawler_impl.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/platform/language.h"

namespace blink {

Navigator::Navigator(LocalFrame *frame) : m_frame(frame) {}

bool Navigator::cookieEnabled(void) const
{
    if (CrawlerImpl *crawler = ToCrawlerImpl(m_frame->Client()))
    {
        if (nullptr != crawler->GetCookieJar(false))
            return true;
    }
    return false;
}

Vector<String> Navigator::languages(void)
{
    Vector<String> ret;
    ret.push_back(DefaultLanguage());
    return ret;
}

String Navigator::userAgent(void) const
{
    return m_frame->Loader().UserAgent();
}

} // namespace blink
