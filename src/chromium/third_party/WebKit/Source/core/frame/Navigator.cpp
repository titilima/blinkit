// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: Navigator.cpp
// Description: Navigator Class
//      Author: Ziming Li
//     Created: 2019-05-14
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
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "core/frame/Navigator.h"

#include "bindings/core/v8/ScriptController.h"
#include "core/dom/Document.h"
#include "core/frame/FrameHost.h"
#include "core/frame/LocalFrame.h"
#include "core/frame/NavigatorID.h"
#include "core/frame/Settings.h"
#include "core/loader/CookieJar.h"
#include "core/loader/FrameLoader.h"
#include "core/page/ChromeClient.h"
#include "platform/Language.h"

namespace blink {

Navigator::Navigator(LocalFrame* frame)
    : DOMWindowProperty(frame)
{
}

Navigator::~Navigator()
{
}

String Navigator::productSub() const
{
    return "20030107";
}

String Navigator::vendor() const
{
    // Do not change without good cause. History:
    // https://code.google.com/p/chromium/issues/detail?id=276813
    // https://www.w3.org/Bugs/Public/show_bug.cgi?id=27786
    // https://groups.google.com/a/chromium.org/forum/#!topic/blink-dev/QrgyulnqvmE
    return "Google Inc.";
}

String Navigator::vendorSub() const
{
    return "";
}

String Navigator::userAgent() const
{
    if (m_frame)
    {
#ifdef BLINKIT_CRAWLER_ONLY
        ASSERT(m_frame->IsCrawlerFrame());
        return m_frame->loader().userAgent();
#else
        if (m_frame->IsCrawlerFrame() || nullptr != m_frame->page())
            return m_frame->loader().userAgent();
#endif
    }

    // If the frame is already detached it no longer has a meaningful useragent.
    return String();
}

bool Navigator::cookieEnabled() const
{
    if (!m_frame)
        return false;

    Settings* settings = m_frame->settings();
    if (!settings || !settings->cookieEnabled())
        return false;

    return cookiesEnabled(m_frame->document());
}

Vector<String> Navigator::languages()
{
    Vector<String> languages;

#ifdef BLINKIT_CRAWLER_ONLY
    languages.append(defaultLanguage());
#else
    if (!m_frame || !m_frame->host()) {
        languages.append(defaultLanguage());
        return languages;
    }

    String acceptLanguages = m_frame->host()->chromeClient().acceptLanguages();
    acceptLanguages.split(',', languages);

    // Sanitizing tokens. We could do that more extensively but we should assume
    // that the accept languages are already sane and support BCP47. It is
    // likely a waste of time to make sure the tokens matches that spec here.
    for (size_t i = 0; i < languages.size(); ++i) {
        String& token = languages[i];
        token = token.stripWhiteSpace();
        if (token.length() >= 3 && token[2] == '_')
            token.replace(2, 1, "-");
    }
#endif

    return languages;
}

DEFINE_TRACE(Navigator)
{
    HeapSupplementable<Navigator>::trace(visitor);
    DOMWindowProperty::trace(visitor);
}

} // namespace blink
